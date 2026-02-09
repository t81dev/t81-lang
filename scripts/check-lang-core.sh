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
  "${ROOT}/src/frontend/ast_printer.cpp"
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
run_test "${ROOT}/tests/syntax/frontend_parser_module_import_effect_test.cpp" "${BUILD_DIR}/frontend_parser_module_import_effect_test"
run_test "${ROOT}/tests/syntax/frontend_fuzz_test.cpp" "${BUILD_DIR}/frontend_fuzz_test"
run_test "${ROOT}/tests/syntax/frontend_lexer_test.cpp" "${BUILD_DIR}/frontend_lexer_test"
run_test "${ROOT}/tests/semantics/semantic_analyzer_equality_test.cpp" "${BUILD_DIR}/semantic_analyzer_equality_test"
run_test "${ROOT}/tests/semantics/semantic_analyzer_generic_test.cpp" "${BUILD_DIR}/semantic_analyzer_generic_test"
run_test "${ROOT}/tests/semantics/semantic_analyzer_loop_test.cpp" "${BUILD_DIR}/semantic_analyzer_loop_test"
run_test "${ROOT}/tests/semantics/semantic_analyzer_match_test.cpp" "${BUILD_DIR}/semantic_analyzer_match_test"
run_test "${ROOT}/tests/semantics/semantic_analyzer_module_import_effect_test.cpp" "${BUILD_DIR}/semantic_analyzer_module_import_effect_test"
run_test "${ROOT}/tests/semantics/semantic_analyzer_numeric_test.cpp" "${BUILD_DIR}/semantic_analyzer_numeric_test"
run_test "${ROOT}/tests/semantics/semantic_analyzer_option_result_test.cpp" "${BUILD_DIR}/semantic_analyzer_option_result_test"
run_test "${ROOT}/tests/semantics/semantic_analyzer_record_enum_test.cpp" "${BUILD_DIR}/semantic_analyzer_record_enum_test"
run_test "${ROOT}/tests/semantics/semantic_analyzer_vector_literal_test.cpp" "${BUILD_DIR}/semantic_analyzer_vector_literal_test"
run_test "${ROOT}/tests/roundtrip/lang_literal_pool_test.cpp" "${BUILD_DIR}/lang_literal_pool_test"
run_test "${ROOT}/tests/roundtrip/frontend_ir_generator_logical_short_circuit_test.cpp" "${BUILD_DIR}/frontend_ir_generator_logical_short_circuit_test"

echo "lang core checks: ok"
