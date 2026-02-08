#include "t81/tisc/pretty_printer.hpp"

#include <sstream>

namespace t81::tisc {

std::string pretty_print(const ir::IntermediateProgram& program) {
  std::ostringstream out;
  out << "instructions=" << program.instructions().size();
  out << ", type_aliases=" << program.type_aliases().size();
  out << ", tensors=" << program.tensor_pool().size();
  return out.str();
}

} // namespace t81::tisc
