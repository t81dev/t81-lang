#include "t81/frontend/ast_printer.hpp"
#include "t81/frontend/ir_generator.hpp"
#include "t81/frontend/lexer.hpp"
#include "t81/frontend/parser.hpp"
#include "t81/frontend/semantic_analyzer.hpp"
#include "t81/tisc/pretty_printer.hpp"

#include <algorithm>
#include <cstdint>
#include <filesystem>
#include <fstream>
#include <functional>
#include <iostream>
#include <memory>
#include <optional>
#include <sstream>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <vector>

namespace {

constexpr int kUsageExitCode = 64;

void print_usage(std::ostream& os) {
    os << "Usage:\n"
       << "  t81-lang parse <file.t81>\n"
       << "  t81-lang check <file.t81>\n"
       << "  t81-lang emit-ir <file.t81> [-o out.ir]\n"
       << "  t81-lang emit-bytecode <file.t81> [-o out.tisc.json]\n"
       << "  t81-lang build <file.t81> [-o out.tisc.json]\n";
}

std::optional<std::string> read_file(const std::string& path) {
    std::ifstream in(path);
    if (!in) {
        return std::nullopt;
    }
    std::ostringstream content;
    content << in.rdbuf();
    return content.str();
}

bool write_file(const std::filesystem::path& path, const std::string& content) {
    std::ofstream out(path);
    if (!out) {
        return false;
    }
    out << content;
    return out.good();
}

int run_parse(const std::string& path) {
    const auto source = read_file(path);
    if (!source.has_value()) {
        std::cerr << "error: unable to read source file: " << path << "\n";
        return 1;
    }

    t81::frontend::Lexer lexer(*source);
    t81::frontend::Parser parser(lexer, path);
    auto statements = parser.parse();
    if (parser.had_error()) {
        return 1;
    }

    t81::frontend::CanonicalAstPrinter printer;
    for (const auto& stmt : statements) {
        if (!stmt) {
            std::cerr << "error: null AST statement encountered\n";
            return 1;
        }
        std::cout << printer.print(*stmt) << "\n";
    }

    return 0;
}

std::optional<std::string> parse_optional_output_path(int argc, char** argv, int start_index) {
    const int remaining = argc - start_index;
    if (remaining == 0) {
        return std::nullopt;
    }
    if (remaining == 2 && std::string_view(argv[start_index]) == "-o") {
        return std::string(argv[start_index + 1]);
    }
    return std::string();
}

struct ModuleUnit {
    std::filesystem::path path;
    std::shared_ptr<std::string> source;
    std::optional<std::string> module_decl;
    std::vector<std::string> imports;
    std::vector<std::unique_ptr<t81::frontend::Stmt>> statements;
};

std::optional<ModuleUnit> parse_unit(const std::filesystem::path& path) {
    auto source = read_file(path.string());
    if (!source.has_value()) {
        std::cerr << "error: unable to read source file: " << path << "\n";
        return std::nullopt;
    }

    ModuleUnit unit;
    unit.path = path;
    unit.source = std::make_shared<std::string>(std::move(*source));

    t81::frontend::Lexer lexer(*unit.source);
    t81::frontend::Parser parser(lexer, path.string());
    auto statements = parser.parse();
    if (parser.had_error()) {
        return std::nullopt;
    }

    for (const auto& stmt : statements) {
        if (!stmt) {
            continue;
        }
        if (auto* module = dynamic_cast<const t81::frontend::ModuleDecl*>(stmt.get())) {
            unit.module_decl = module->path;
        } else if (auto* import_stmt = dynamic_cast<const t81::frontend::ImportDecl*>(stmt.get())) {
            unit.imports.push_back(import_stmt->path);
        }
    }
    unit.statements = std::move(statements);
    return unit;
}

std::vector<std::string> split_segments(const std::string& value, char sep) {
    std::vector<std::string> out;
    std::string current;
    for (char c : value) {
        if (c == sep) {
            if (!current.empty()) {
                out.push_back(current);
                current.clear();
            }
            continue;
        }
        current.push_back(c);
    }
    if (!current.empty()) {
        out.push_back(current);
    }
    return out;
}

std::filesystem::path resolve_import_path(const std::filesystem::path& importer,
                                          const std::optional<std::string>& module_decl,
                                          const std::string& import_path) {
    std::string relative = import_path;
    for (char& c : relative) {
        if (c == '.') {
            c = '/';
        }
    }

    std::filesystem::path direct = importer.parent_path() / (relative + ".t81");
    if (std::filesystem::exists(direct)) {
        return direct;
    }

    if (module_decl.has_value()) {
        auto module_segments = split_segments(*module_decl, '.');
        std::filesystem::path module_root = importer.parent_path();
        // module app.main at app/main.t81 => pop one segment to repo-local module root
        for (size_t i = 1; i < module_segments.size(); ++i) {
            module_root = module_root.parent_path();
        }
        return module_root / (relative + ".t81");
    }

    return direct;
}

int run_check(const std::string& entry_file) {
    namespace fs = std::filesystem;
    const fs::path entry = fs::absolute(fs::path(entry_file));
    if (!fs::exists(entry)) {
        std::cerr << "error: entry file does not exist: " << entry << "\n";
        return 1;
    }

    enum class VisitState { Unseen, Visiting, Done };
    std::unordered_map<std::string, VisitState> state;
    std::unordered_map<std::string, ModuleUnit> units;
    std::vector<std::string> stack;
    bool graph_error = false;

    std::function<void(const fs::path&)> load_module = [&](const fs::path& current) {
        const std::string key = fs::weakly_canonical(current).string();
        auto it = state.find(key);
        if (it != state.end() && it->second == VisitState::Done) {
            return;
        }
        if (it != state.end() && it->second == VisitState::Visiting) {
            graph_error = true;
            std::cerr << "error: import cycle detected:\n";
            auto cycle_start = std::find(stack.begin(), stack.end(), key);
            if (cycle_start != stack.end()) {
                for (auto c = cycle_start; c != stack.end(); ++c) {
                    std::cerr << "  -> " << *c << "\n";
                }
            }
            std::cerr << "  -> " << key << "\n";
            return;
        }

        state[key] = VisitState::Visiting;
        stack.push_back(key);

        auto unit = parse_unit(current);
        if (!unit.has_value()) {
            graph_error = true;
            stack.pop_back();
            state[key] = VisitState::Done;
            return;
        }

        for (const auto& imp : unit->imports) {
            fs::path dep = fs::weakly_canonical(resolve_import_path(current, unit->module_decl, imp));
            if (!fs::exists(dep)) {
                graph_error = true;
                std::cerr << "error: missing import '" << imp << "' referenced from " << current << "\n";
                continue;
            }
            load_module(dep);
        }

        units.emplace(key, std::move(*unit));
        stack.pop_back();
        state[key] = VisitState::Done;
    };

    load_module(entry);
    if (graph_error) {
        return 1;
    }

    bool semantic_error = false;
    for (auto& [path, unit] : units) {
        t81::frontend::SemanticAnalyzer analyzer(unit.statements, path);
        analyzer.analyze();
        if (analyzer.had_error()) {
            semantic_error = true;
            for (const auto& diag : analyzer.diagnostics()) {
                std::cerr << diag.file << ":" << diag.line << ":" << diag.column
                          << ": error: " << diag.message << "\n";
            }
        }
    }

    return semantic_error ? 1 : 0;
}

std::optional<t81::tisc::ir::IntermediateProgram> compile_entry_to_ir(const std::string& path) {
    const auto source = read_file(path);
    if (!source.has_value()) {
        std::cerr << "error: unable to read source file: " << path << "\n";
        return std::nullopt;
    }

    t81::frontend::Lexer lexer(*source);
    t81::frontend::Parser parser(lexer, path);
    auto statements = parser.parse();
    if (parser.had_error()) {
        return std::nullopt;
    }

    t81::frontend::SemanticAnalyzer analyzer(statements, path);
    analyzer.analyze();
    if (analyzer.had_error()) {
        for (const auto& diag : analyzer.diagnostics()) {
            std::cerr << diag.file << ":" << diag.line << ":" << diag.column
                      << ": error: " << diag.message << "\n";
        }
        return std::nullopt;
    }

    t81::frontend::IRGenerator generator;
    generator.attach_semantic_analyzer(&analyzer);
    return generator.generate(statements);
}

std::optional<std::string> map_opcode_name(t81::tisc::ir::Opcode opcode) {
    using t81::tisc::ir::Opcode;
    switch (opcode) {
        case Opcode::ADD: return "Add";
        case Opcode::SUB: return "Sub";
        case Opcode::MUL: return "Mul";
        case Opcode::DIV: return "Div";
        case Opcode::MOD: return "Mod";
        case Opcode::NEG: return "Neg";
        case Opcode::FADD: return "FAdd";
        case Opcode::FSUB: return "FSub";
        case Opcode::FMUL: return "FMul";
        case Opcode::FDIV: return "FDiv";
        case Opcode::FRACADD: return "FracAdd";
        case Opcode::FRACSUB: return "FracSub";
        case Opcode::FRACMUL: return "FracMul";
        case Opcode::FRACDIV: return "FracDiv";
        case Opcode::CMP: return "Cmp";
        case Opcode::MOV: return "Mov";
        case Opcode::LOADI: return "LoadImm";
        case Opcode::LOAD: return "Load";
        case Opcode::STORE: return "Store";
        case Opcode::PUSH: return "Push";
        case Opcode::POP: return "Pop";
        case Opcode::JMP: return "Jump";
        case Opcode::JZ: return "JumpIfZero";
        case Opcode::JNZ: return "JumpIfNotZero";
        case Opcode::JN: return "JumpIfNegative";
        case Opcode::JP: return "JumpIfPositive";
        case Opcode::CALL: return "Call";
        case Opcode::RET: return "Ret";
        case Opcode::I2F: return "I2F";
        case Opcode::F2I: return "F2I";
        case Opcode::I2FRAC: return "I2Frac";
        case Opcode::FRAC2I: return "Frac2I";
        case Opcode::MAKE_OPTION_SOME: return "MakeOptionSome";
        case Opcode::MAKE_OPTION_NONE: return "MakeOptionNone";
        case Opcode::MAKE_RESULT_OK: return "MakeResultOk";
        case Opcode::MAKE_RESULT_ERR: return "MakeResultErr";
        case Opcode::OPTION_IS_SOME: return "OptionIsSome";
        case Opcode::OPTION_UNWRAP: return "OptionUnwrap";
        case Opcode::RESULT_IS_OK: return "ResultIsOk";
        case Opcode::RESULT_UNWRAP_OK: return "ResultUnwrapOk";
        case Opcode::RESULT_UNWRAP_ERR: return "ResultUnwrapErr";
        case Opcode::MAKE_ENUM_VARIANT: return "MakeEnumVariant";
        case Opcode::MAKE_ENUM_VARIANT_PAYLOAD: return "MakeEnumVariantPayload";
        case Opcode::ENUM_IS_VARIANT: return "EnumIsVariant";
        case Opcode::ENUM_UNWRAP_PAYLOAD: return "EnumUnwrapPayload";
        case Opcode::NOP: return "Nop";
        case Opcode::HALT: return "Halt";
        case Opcode::TRAP: return "Trap";
        case Opcode::WEIGHTS_LOAD: return "WeightsLoad";
        case Opcode::LABEL: return std::nullopt;
    }
    return std::nullopt;
}

struct EncodedInstruction {
    std::string opcode;
    std::int64_t a = 0;
    std::int64_t b = 0;
    std::int64_t c = 0;
};

std::optional<std::vector<EncodedInstruction>> encode_program(const t81::tisc::ir::IntermediateProgram& program) {
    using t81::tisc::ir::Instruction;
    using t81::tisc::ir::Label;
    using t81::tisc::ir::Opcode;
    using t81::tisc::ir::Register;

    std::unordered_map<int, std::int64_t> label_pc;
    std::int64_t pc = 0;
    for (const auto& instr : program.instructions()) {
        if (instr.opcode == Opcode::LABEL) {
            if (instr.operands.size() != 1 || !std::holds_alternative<Label>(instr.operands[0])) {
                std::cerr << "error: malformed LABEL instruction\n";
                return std::nullopt;
            }
            label_pc[std::get<Label>(instr.operands[0]).id] = pc;
            continue;
        }
        ++pc;
    }

    auto resolve_operand = [&label_pc](const Instruction& instr, size_t index) -> std::optional<std::int64_t> {
        if (index >= instr.operands.size()) {
            return 0;
        }
        const auto& op = instr.operands[index];
        if (std::holds_alternative<Register>(op)) {
            return std::get<Register>(op).index;
        }
        if (std::holds_alternative<t81::tisc::ir::Immediate>(op)) {
            return std::get<t81::tisc::ir::Immediate>(op).value;
        }
        if (std::holds_alternative<Label>(op)) {
            const int id = std::get<Label>(op).id;
            auto it = label_pc.find(id);
            if (it == label_pc.end()) {
                return std::nullopt;
            }
            return it->second;
        }
        return std::nullopt;
    };

    std::vector<EncodedInstruction> out;
    out.reserve(program.instructions().size());

    for (const auto& instr : program.instructions()) {
        if (instr.opcode == Opcode::LABEL) {
            continue;
        }
        if (instr.operands.size() > 3) {
            std::cerr << "error: opcode carries more than 3 operands; not encodable in tisc-json-v1\n";
            return std::nullopt;
        }
        auto opcode_name = map_opcode_name(instr.opcode);
        if (!opcode_name.has_value()) {
            std::cerr << "error: unsupported opcode in bytecode emitter\n";
            return std::nullopt;
        }

        EncodedInstruction encoded;
        encoded.opcode = *opcode_name;

        if ((instr.opcode == Opcode::JZ ||
             instr.opcode == Opcode::JNZ ||
             instr.opcode == Opcode::JN ||
             instr.opcode == Opcode::JP) &&
            instr.operands.size() >= 2 &&
            std::holds_alternative<Label>(instr.operands[0]) &&
            std::holds_alternative<Register>(instr.operands[1])) {
            auto target = resolve_operand(instr, 0);
            auto cond = resolve_operand(instr, 1);
            if (!target.has_value() || !cond.has_value()) {
                std::cerr << "error: unresolved jump operand\n";
                return std::nullopt;
            }
            encoded.a = *cond;
            encoded.b = *target;
            encoded.c = 0;
        } else {
            auto a = resolve_operand(instr, 0);
            auto b = resolve_operand(instr, 1);
            auto c = resolve_operand(instr, 2);
            if (!a.has_value() || !b.has_value() || !c.has_value()) {
                std::cerr << "error: unresolved instruction operand\n";
                return std::nullopt;
            }
            encoded.a = *a;
            encoded.b = *b;
            encoded.c = *c;
        }

        out.push_back(std::move(encoded));
    }

    if (out.empty()) {
        out.push_back({"Halt", 0, 0, 0});
    }

    return out;
}

std::string render_tisc_json(const std::vector<EncodedInstruction>& instructions) {
    std::ostringstream out;
    out << "{\n";
    out << "  \"format_version\": \"tisc-json-v1\",\n";
    out << "  \"axion_policy_text\": \"(policy (tier 1))\",\n";
    out << "  \"insns\": [\n";
    for (size_t i = 0; i < instructions.size(); ++i) {
        const auto& insn = instructions[i];
        out << "    {\"opcode\": \"" << insn.opcode << "\", \"a\": " << insn.a
            << ", \"b\": " << insn.b << ", \"c\": " << insn.c << "}";
        if (i + 1 != instructions.size()) {
            out << ",";
        }
        out << "\n";
    }
    out << "  ]\n";
    out << "}\n";
    return out.str();
}

int run_emit_ir(const std::string& path, const std::optional<std::string>& output_path) {
    if (run_check(path) != 0) {
        return 1;
    }
    auto program = compile_entry_to_ir(path);
    if (!program.has_value()) {
        return 1;
    }

    std::string text = t81::tisc::pretty_print(*program);
    text.push_back('\n');

    if (!output_path.has_value()) {
        std::cout << text;
        return 0;
    }
    if (!write_file(*output_path, text)) {
        std::cerr << "error: unable to write output file: " << *output_path << "\n";
        return 1;
    }
    std::cout << *output_path << "\n";
    return 0;
}

int run_emit_bytecode(const std::string& path, const std::string& output_path) {
    if (run_check(path) != 0) {
        return 1;
    }
    auto program = compile_entry_to_ir(path);
    if (!program.has_value()) {
        return 1;
    }

    auto encoded = encode_program(*program);
    if (!encoded.has_value()) {
        return 1;
    }
    const std::string json = render_tisc_json(*encoded);
    if (!write_file(output_path, json)) {
        std::cerr << "error: unable to write output file: " << output_path << "\n";
        return 1;
    }
    std::cout << output_path << "\n";
    return 0;
}

} // namespace

int main(int argc, char** argv) {
    if (argc < 2) {
        print_usage(std::cerr);
        return kUsageExitCode;
    }

    const std::string command = argv[1];
    if (command == "parse") {
        if (argc != 3) {
            print_usage(std::cerr);
            return kUsageExitCode;
        }
        return run_parse(argv[2]);
    }
    if (command == "check") {
        if (argc != 3) {
            print_usage(std::cerr);
            return kUsageExitCode;
        }
        return run_check(argv[2]);
    }
    if (command == "emit-ir") {
        if (argc != 3 && argc != 5) {
            print_usage(std::cerr);
            return kUsageExitCode;
        }
        auto output_path = parse_optional_output_path(argc, argv, 3);
        if (argc == 5 && (!output_path.has_value() || output_path->empty())) {
            print_usage(std::cerr);
            return kUsageExitCode;
        }
        return run_emit_ir(argv[2], output_path);
    }
    if (command == "emit-bytecode" || command == "build") {
        if (argc != 3 && argc != 5) {
            print_usage(std::cerr);
            return kUsageExitCode;
        }
        auto output_path = parse_optional_output_path(argc, argv, 3);
        if (argc == 5 && (!output_path.has_value() || output_path->empty())) {
            print_usage(std::cerr);
            return kUsageExitCode;
        }

        std::filesystem::path out;
        if (output_path.has_value() && !output_path->empty()) {
            out = *output_path;
        } else {
            out = std::filesystem::path(argv[2]);
            out.replace_extension(".tisc.json");
        }
        return run_emit_bytecode(argv[2], out.string());
    }

    std::cerr << "error: unknown command: " << command << "\n";
    print_usage(std::cerr);
    return kUsageExitCode;
}
