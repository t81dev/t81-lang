#include "t81/frontend/ast_printer.hpp"
#include "t81/frontend/lexer.hpp"
#include "t81/frontend/parser.hpp"
#include "t81/frontend/semantic_analyzer.hpp"

#include <algorithm>
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
       << "  t81-lang check <file.t81>\n";
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

struct ModuleUnit {
    std::filesystem::path path;
    std::optional<std::string> module_decl;
    std::vector<std::string> imports;
    std::vector<std::unique_ptr<t81::frontend::Stmt>> statements;
};

std::optional<ModuleUnit> parse_unit(const std::filesystem::path& path) {
    const auto source = read_file(path.string());
    if (!source.has_value()) {
        std::cerr << "error: unable to read source file: " << path << "\n";
        return std::nullopt;
    }
    t81::frontend::Lexer lexer(*source);
    t81::frontend::Parser parser(lexer, path.string());
    auto statements = parser.parse();
    if (parser.had_error()) {
        return std::nullopt;
    }

    ModuleUnit unit;
    unit.path = path;
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

    std::cerr << "error: unknown command: " << command << "\n";
    print_usage(std::cerr);
    return kUsageExitCode;
}
