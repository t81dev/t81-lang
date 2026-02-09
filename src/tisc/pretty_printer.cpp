#include "t81/tisc/pretty_printer.hpp"

#include <sstream>
#include <string>

namespace t81::tisc {

namespace {

std::string opcode_name(ir::Opcode opcode) {
  switch (opcode) {
    case ir::Opcode::ADD: return "ADD";
    case ir::Opcode::SUB: return "SUB";
    case ir::Opcode::MUL: return "MUL";
    case ir::Opcode::DIV: return "DIV";
    case ir::Opcode::MOD: return "MOD";
    case ir::Opcode::NEG: return "NEG";
    case ir::Opcode::FADD: return "FADD";
    case ir::Opcode::FSUB: return "FSUB";
    case ir::Opcode::FMUL: return "FMUL";
    case ir::Opcode::FDIV: return "FDIV";
    case ir::Opcode::FRACADD: return "FRACADD";
    case ir::Opcode::FRACSUB: return "FRACSUB";
    case ir::Opcode::FRACMUL: return "FRACMUL";
    case ir::Opcode::FRACDIV: return "FRACDIV";
    case ir::Opcode::CMP: return "CMP";
    case ir::Opcode::MOV: return "MOV";
    case ir::Opcode::LOADI: return "LOADI";
    case ir::Opcode::LOAD: return "LOAD";
    case ir::Opcode::STORE: return "STORE";
    case ir::Opcode::PUSH: return "PUSH";
    case ir::Opcode::POP: return "POP";
    case ir::Opcode::JMP: return "JMP";
    case ir::Opcode::JZ: return "JZ";
    case ir::Opcode::JNZ: return "JNZ";
    case ir::Opcode::JN: return "JN";
    case ir::Opcode::JP: return "JP";
    case ir::Opcode::CALL: return "CALL";
    case ir::Opcode::RET: return "RET";
    case ir::Opcode::I2F: return "I2F";
    case ir::Opcode::F2I: return "F2I";
    case ir::Opcode::I2FRAC: return "I2FRAC";
    case ir::Opcode::FRAC2I: return "FRAC2I";
    case ir::Opcode::MAKE_OPTION_SOME: return "MAKE_OPTION_SOME";
    case ir::Opcode::MAKE_OPTION_NONE: return "MAKE_OPTION_NONE";
    case ir::Opcode::MAKE_RESULT_OK: return "MAKE_RESULT_OK";
    case ir::Opcode::MAKE_RESULT_ERR: return "MAKE_RESULT_ERR";
    case ir::Opcode::OPTION_IS_SOME: return "OPTION_IS_SOME";
    case ir::Opcode::OPTION_UNWRAP: return "OPTION_UNWRAP";
    case ir::Opcode::RESULT_IS_OK: return "RESULT_IS_OK";
    case ir::Opcode::RESULT_UNWRAP_OK: return "RESULT_UNWRAP_OK";
    case ir::Opcode::RESULT_UNWRAP_ERR: return "RESULT_UNWRAP_ERR";
    case ir::Opcode::MAKE_ENUM_VARIANT: return "MAKE_ENUM_VARIANT";
    case ir::Opcode::MAKE_ENUM_VARIANT_PAYLOAD: return "MAKE_ENUM_VARIANT_PAYLOAD";
    case ir::Opcode::ENUM_IS_VARIANT: return "ENUM_IS_VARIANT";
    case ir::Opcode::ENUM_UNWRAP_PAYLOAD: return "ENUM_UNWRAP_PAYLOAD";
    case ir::Opcode::NOP: return "NOP";
    case ir::Opcode::HALT: return "HALT";
    case ir::Opcode::TRAP: return "TRAP";
    case ir::Opcode::WEIGHTS_LOAD: return "WEIGHTS_LOAD";
    case ir::Opcode::LABEL: return "LABEL";
  }
  return "UNKNOWN";
}

std::string operand_to_string(const ir::Operand& operand) {
  if (std::holds_alternative<ir::Register>(operand)) {
    return "r" + std::to_string(std::get<ir::Register>(operand).index);
  }
  if (std::holds_alternative<ir::Immediate>(operand)) {
    return "#" + std::to_string(std::get<ir::Immediate>(operand).value);
  }
  if (std::holds_alternative<ir::Label>(operand)) {
    return "L" + std::to_string(std::get<ir::Label>(operand).id);
  }
  return "?";
}

} // namespace

std::string pretty_print(const ir::IntermediateProgram& program) {
  std::ostringstream out;
  out << "program {\n";
  out << "  function_metadata:\n";
  for (const auto& fn : program.function_metadata()) {
    out << "    - name=\"" << fn.name << "\"";
    out << " effect=" << (fn.is_effectful ? "true" : "false");
    if (fn.tier.has_value()) {
      out << " tier=" << *fn.tier;
    }
    out << "\n";
  }
  out << "  type_aliases=" << program.type_aliases().size() << "\n";
  out << "  tensors=" << program.tensor_pool().size() << "\n";
  out << "  instructions:\n";
  std::size_t index = 0;
  for (const auto& insn : program.instructions()) {
    out << "    " << index++ << ": " << opcode_name(insn.opcode);
    if (!insn.operands.empty()) {
      out << " ";
      for (std::size_t i = 0; i < insn.operands.size(); ++i) {
        out << operand_to_string(insn.operands[i]);
        if (i + 1 != insn.operands.size()) {
          out << ", ";
        }
      }
    }
    out << "\n";
  }
  out << "}";
  return out.str();
}

} // namespace t81::tisc
