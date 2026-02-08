#ifndef T81_TISC_PRETTY_PRINTER_HPP
#define T81_TISC_PRETTY_PRINTER_HPP

#include "t81/tisc/ir.hpp"
#include <string>

namespace t81::tisc {

std::string pretty_print(const ir::IntermediateProgram& program);

} // namespace t81::tisc

#endif
