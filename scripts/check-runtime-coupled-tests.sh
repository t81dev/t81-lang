#!/usr/bin/env bash
set -euo pipefail

ROOT="$(cd "$(dirname "${BASH_SOURCE[0]}")/.." && pwd)"
MANIFEST="${ROOT}/tests/roundtrip/runtime-coupled-tests.txt"
JUSTIFICATION="${ROOT}/docs/migration/runtime-coupled-justification.md"

if [[ ! -f "${MANIFEST}" ]]; then
  echo "missing manifest: ${MANIFEST}" >&2
  exit 1
fi
if [[ ! -f "${JUSTIFICATION}" ]]; then
  echo "missing justification ledger: ${JUSTIFICATION}" >&2
  exit 1
fi

fail=0

while IFS= read -r rel; do
  [[ -z "${rel}" ]] && continue
  file="${ROOT}/${rel}"
  if [[ ! -f "${file}" ]]; then
    echo "missing runtime-coupled test file: ${rel}" >&2
    fail=1
    continue
  fi

  if ! grep -Eq '#include[[:space:]]*[<"]t81/(vm|cli)/|#include[[:space:]]*[<"]t81/tisc/(binary_emitter|binary_io)\.hpp' "${file}"; then
    echo "manifested file has no runtime-coupled include markers: ${rel}" >&2
    fail=1
  fi

  if ! grep -Fq -- "- ${rel}:" "${JUSTIFICATION}"; then
    echo "manifested file missing explicit justification: ${rel}" >&2
    fail=1
  fi
done < "${MANIFEST}"

# Language-only suites must stay runtime-decoupled.
if grep -RInE '#include[[:space:]]*[<"]t81/(vm|cli)/' "${ROOT}/tests/syntax" "${ROOT}/tests/semantics"; then
  echo "language-only suites contain runtime includes; move these tests to integration lanes" >&2
  fail=1
fi

if [[ "${fail}" -ne 0 ]]; then
  exit 1
fi

echo "runtime-coupled manifest check: ok"
