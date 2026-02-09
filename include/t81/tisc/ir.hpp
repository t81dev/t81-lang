#ifndef T81_TISC_IR_HPP
#define T81_TISC_IR_HPP

#include <optional>
#include <cstdint>
#include <string>
#include <utility>
#include <variant>
#include <vector>

#include "t81/tensor.hpp"
#include "t81/tisc/program.hpp"
#include "t81/tisc/type_alias.hpp"

namespace t81::tisc::ir {

enum class PrimitiveKind {
  Unknown,
  Integer,
  Float,
  Fraction,
  Boolean,
};

enum class ComparisonRelation {
  None,
  Less,
  LessEqual,
  Greater,
  GreaterEqual,
  Equal,
  NotEqual,
};

enum class Opcode {
  ADD, SUB, MUL, DIV, MOD, NEG,
  FADD, FSUB, FMUL, FDIV,
  FRACADD, FRACSUB, FRACMUL, FRACDIV,
  CMP,
  MOV, LOADI,
  LOAD, STORE, PUSH, POP,
  JMP, JZ, JNZ, JN, JP,
  CALL, RET,
  I2F, F2I, I2FRAC, FRAC2I,
  MAKE_OPTION_SOME, MAKE_OPTION_NONE,
  MAKE_RESULT_OK, MAKE_RESULT_ERR,
  OPTION_IS_SOME, OPTION_UNWRAP,
  RESULT_IS_OK, RESULT_UNWRAP_OK, RESULT_UNWRAP_ERR,
  MAKE_ENUM_VARIANT, MAKE_ENUM_VARIANT_PAYLOAD,
  ENUM_IS_VARIANT, ENUM_UNWRAP_PAYLOAD,
  NOP, HALT, TRAP,
  WEIGHTS_LOAD,
  LABEL,
};

struct Register {
  int index;
};

struct Immediate {
  long long value;
};

struct Label {
  int id;
};

using Operand = std::variant<Register, Immediate, Label>;

struct Instruction {
  Opcode opcode;
  std::vector<Operand> operands;
  PrimitiveKind primitive = PrimitiveKind::Unknown;
  bool boolean_result = false;
  bool is_conversion = false;
  ComparisonRelation relation = ComparisonRelation::None;
  tisc::LiteralKind literal_kind = tisc::LiteralKind::Int;
  std::optional<std::string> text_literal;

  Instruction(Opcode opcode_ = Opcode::NOP, std::vector<Operand> operands_ = {})
      : opcode(opcode_), operands(std::move(operands_)) {}
};

struct FunctionMetadata {
  std::string name;
  bool is_effectful = false;
  std::optional<std::int64_t> tier;
};

class IntermediateProgram {
public:
  void add_instruction(Instruction instr) {
    instructions_.push_back(std::move(instr));
  }

  const std::vector<Instruction>& instructions() const {
    return instructions_;
  }

  void add_type_alias(TypeAliasMetadata meta) {
    type_aliases_.push_back(std::move(meta));
  }

  const std::vector<TypeAliasMetadata>& type_aliases() const {
    return type_aliases_;
  }

  void add_function_metadata(FunctionMetadata meta) {
    function_metadata_.push_back(std::move(meta));
  }

  const std::vector<FunctionMetadata>& function_metadata() const {
    return function_metadata_;
  }

  int add_tensor(t81::T729Tensor tensor) {
    tensor_pool_.push_back(std::move(tensor));
    return static_cast<int>(tensor_pool_.size());
  }

  const std::vector<t81::T729Tensor>& tensor_pool() const {
    return tensor_pool_;
  }

private:
  std::vector<Instruction> instructions_;
  std::vector<TypeAliasMetadata> type_aliases_;
  std::vector<FunctionMetadata> function_metadata_;
  std::vector<t81::T729Tensor> tensor_pool_;
};

using TypeAliasMetadata = t81::tisc::TypeAliasMetadata;

} // namespace t81::tisc::ir

#endif
