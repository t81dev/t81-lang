#!/usr/bin/env bash
set -euo pipefail

ROOT="$(cd "$(dirname "${BASH_SOURCE[0]}")/.." && pwd)"
VM_DIR="${T81_VM_DIR:-${ROOT}/../t81-vm}"
OUT_DIR="${ROOT}/build/roundtrip"

mkdir -p "${OUT_DIR}"

echo "[roundtrip] emit artifact from t81-lang"
python3 "${ROOT}/scripts/emit-canary-bytecode.py" "${OUT_DIR}/canary.tisc.json" >/dev/null

echo "[roundtrip] build runtime"
make -C "${VM_DIR}" build-check >/dev/null

echo "[roundtrip] execute artifact in t81-vm"
"${VM_DIR}/build/t81vm" --trace --snapshot "${OUT_DIR}/canary.tisc.json" > "${OUT_DIR}/vm.out"

if ! grep -q "STATE_HASH " "${OUT_DIR}/vm.out"; then
  echo "missing STATE_HASH in runtime output" >&2
  exit 1
fi

if ! grep -q "r3=10" "${OUT_DIR}/vm.out"; then
  echo "unexpected register result; expected r3=10" >&2
  exit 1
fi

echo "compiler/runtime roundtrip: ok"
