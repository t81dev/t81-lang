#!/usr/bin/env bash
set -euo pipefail

ROOT="$(cd "$(dirname "${BASH_SOURCE[0]}")/.." && pwd)"
OUT_DIR="${ROOT}/build/bin"
mkdir -p "${OUT_DIR}"

CXX="${CXX:-c++}"
CXXFLAGS="${CXXFLAGS:--std=c++20 -O2 -Wall -Wextra -Wpedantic -I${ROOT}/include}"

"${CXX}" ${CXXFLAGS} \
  "${ROOT}/src/cli/t81_lang_main.cpp" \
  "${ROOT}/src/frontend/lexer.cpp" \
  "${ROOT}/src/frontend/parser.cpp" \
  "${ROOT}/src/frontend/ast_printer.cpp" \
  "${ROOT}/src/frontend/symbol_table.cpp" \
  "${ROOT}/src/frontend/semantic_analyzer.cpp" \
  "${ROOT}/src/tisc/pretty_printer.cpp" \
  -o "${OUT_DIR}/t81-lang"

echo "${OUT_DIR}/t81-lang"
