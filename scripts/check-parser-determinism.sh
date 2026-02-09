#!/usr/bin/env bash
set -euo pipefail

ROOT="$(cd "$(dirname "${BASH_SOURCE[0]}")/.." && pwd)"
TMP_DIR="${ROOT}/build/determinism"
mkdir -p "${TMP_DIR}"

CLI_PATH="$("${ROOT}/scripts/build-t81-lang-cli.sh")"
SNAPSHOT_DIR="${ROOT}/tests/harness/test_vectors/ast_snapshots"
mkdir -p "${SNAPSHOT_DIR}"

SAMPLES=(
  "hello_world"
  "fib"
  "tensors"
)

UPDATE="${UPDATE_GOLDENS:-0}"
FAIL=0

for sample in "${SAMPLES[@]}"; do
  src="${ROOT}/tests/harness/test_vectors/lang_samples/${sample}.t81"
  expected="${SNAPSHOT_DIR}/${sample}.ast"
  actual="${TMP_DIR}/${sample}.ast"

  if [[ ! -f "${src}" ]]; then
    echo "missing sample source: ${src}" >&2
    FAIL=1
    continue
  fi

  "${CLI_PATH}" parse "${src}" > "${actual}"

  if [[ "${UPDATE}" == "1" ]]; then
    cp "${actual}" "${expected}"
    echo "[determinism] updated ${expected}"
    continue
  fi

  if [[ ! -f "${expected}" ]]; then
    echo "missing snapshot: ${expected} (run UPDATE_GOLDENS=1 scripts/check-parser-determinism.sh)" >&2
    FAIL=1
    continue
  fi

  if ! diff -u "${expected}" "${actual}" >/dev/null; then
    echo "[determinism] snapshot mismatch: ${sample}" >&2
    diff -u "${expected}" "${actual}" >&2 || true
    FAIL=1
  fi
done

if [[ "${FAIL}" -ne 0 ]]; then
  exit 1
fi

echo "parser determinism snapshots: ok"
