#!/usr/bin/env bash
set -euo pipefail

ROOT="$(cd "$(dirname "${BASH_SOURCE[0]}")/.." && pwd)"
CLI_PATH="$("${ROOT}/scripts/build-t81-lang-cli.sh")"
OUT_DIR="${ROOT}/build/examples"

mkdir -p "${OUT_DIR}"

SINGLE_FILE_EXAMPLES=(
  "examples/00_hello_world.t81"
  "examples/01_variables_and_arithmetic.t81"
  "examples/02_conditionals_and_loops.t81"
  "examples/03_functions_and_calls.t81"
  "examples/04_logical_short_circuit.t81"
  "examples/06_effect_and_tier.t81"
  "examples/07_records_enums.t81"
  "examples/08_bigint_basics.t81"
  "examples/09_float_basics.t81"
  "examples/10_fraction_basics.t81"
  "examples/11_vector_basics.t81"
  "examples/12_matrix_basics.t81"
  "examples/13_tensor_basics.t81"
  "examples/14_graph_basics.t81"
  "examples/15_all_core_datatypes_mvp.t81"
  "examples/16_trit_numeric_extensions.t81"
  "examples/17_symbol_bytes_base81_flow.t81"
  "examples/18_collections_flow_mvp.t81"
  "examples/19_maybe_result_promise_flow.t81"
  "examples/20_runtime_handles_flow_mvp.t81"
  "examples/string_demo.t81"
  "examples/option_result_match.t81"
)

for rel in "${SINGLE_FILE_EXAMPLES[@]}"; do
  src="${ROOT}/${rel}"
  base="$(basename "${rel}" .t81)"
  out="${OUT_DIR}/${base}.tisc.json"
  "${CLI_PATH}" build "${src}" -o "${out}" >/dev/null
done

MODULE_ENTRY="${ROOT}/examples/05_modules_imports/main.t81"
"${CLI_PATH}" check "${MODULE_ENTRY}" >/dev/null
"${CLI_PATH}" build "${MODULE_ENTRY}" -o "${OUT_DIR}/05_modules_imports_main.tisc.json" >/dev/null

for artifact in "${OUT_DIR}"/*.tisc.json; do
  if [[ ! -s "${artifact}" ]]; then
    echo "missing artifact: ${artifact}" >&2
    exit 1
  fi
  if ! rg -q '"format_version"[[:space:]]*:[[:space:]]*"tisc-json-v1"' "${artifact}"; then
    echo "bad format_version in ${artifact}" >&2
    exit 1
  fi
  if ! rg -q '"insns"[[:space:]]*:' "${artifact}"; then
    echo "missing insns in ${artifact}" >&2
    exit 1
  fi
done

echo "examples build checks: ok"
