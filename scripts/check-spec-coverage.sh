#!/usr/bin/env bash
set -euo pipefail

ROOT="$(cd "$(dirname "${BASH_SOURCE[0]}")/.." && pwd)"
MATRIX="${ROOT}/docs/spec/coverage-matrix.md"

if [[ ! -f "${MATRIX}" ]]; then
  echo "missing spec coverage matrix: ${MATRIX}" >&2
  exit 1
fi

required_rows=(
  "Logical precedence"
  "Module/import declarations"
  "Module graph loading"
  "Function annotations"
  "Pure/effect call boundary"
  "Effect/tier metadata emission"
  "Cross-module symbol resolution through imports"
)

for row in "${required_rows[@]}"; do
  if ! rg -q "${row}" "${MATRIX}"; then
    echo "spec coverage matrix missing required row: ${row}" >&2
    exit 1
  fi
done

if ! rg -q "Snapshot date:" "${MATRIX}"; then
  echo "spec coverage matrix missing snapshot date" >&2
  exit 1
fi

echo "spec coverage matrix: ok"
