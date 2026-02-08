#!/usr/bin/env bash
set -euo pipefail

ROOT="$(cd "$(dirname "${BASH_SOURCE[0]}")/.." && pwd)"
BUILD_DIR="${ROOT}/build/lang-tests"
mkdir -p "${BUILD_DIR}"

CXX="${CXX:-c++}"
CXXFLAGS="${CXXFLAGS:--std=c++20 -O2 -Wall -Wextra -Wpedantic -I${ROOT}/include}"

COMMON_SRCS=(
  "${ROOT}/src/frontend/lexer.cpp"
  "${ROOT}/src/frontend/parser.cpp"
  "${ROOT}/src/frontend/symbol_table.cpp"
  "${ROOT}/src/frontend/semantic_analyzer.cpp"
  "${ROOT}/src/tisc/pretty_printer.cpp"
)

run_test() {
  local test_src="$1"
  local out_bin="$2"
  echo "[lang-core] building $(basename "$test_src")"
  "${CXX}" ${CXXFLAGS} "${test_src}" "${COMMON_SRCS[@]}" -o "${out_bin}"
  echo "[lang-core] running $(basename "$out_bin")"
  "${out_bin}" >/dev/null
}

run_test "${ROOT}/tests/syntax/frontend_parser_test.cpp" "${BUILD_DIR}/frontend_parser_test"
run_test "${ROOT}/tests/syntax/frontend_parser_generics_test.cpp" "${BUILD_DIR}/frontend_parser_generics_test"
run_test "${ROOT}/tests/syntax/frontend_parser_legacy_rejection_test.cpp" "${BUILD_DIR}/frontend_parser_legacy_rejection_test"
run_test "${ROOT}/tests/syntax/frontend_fuzz_test.cpp" "${BUILD_DIR}/frontend_fuzz_test"
run_test "${ROOT}/tests/semantics/semantic_analyzer_equality_test.cpp" "${BUILD_DIR}/semantic_analyzer_equality_test"

echo "lang core checks: ok"
