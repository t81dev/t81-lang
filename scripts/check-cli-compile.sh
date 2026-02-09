#!/usr/bin/env bash
set -euo pipefail

ROOT="$(cd "$(dirname "${BASH_SOURCE[0]}")/.." && pwd)"
CLI_PATH="$("${ROOT}/scripts/build-t81-lang-cli.sh")"
OUT_DIR="${ROOT}/build/cli-compile"
SRC="${ROOT}/tests/harness/test_vectors/lang_samples/hello_world.t81"
ENTRY="${ROOT}/tests/harness/module_graph/ok/app/main.t81"

mkdir -p "${OUT_DIR}"

"${CLI_PATH}" parse "${SRC}" >/dev/null
"${CLI_PATH}" check "${ENTRY}" >/dev/null
"${CLI_PATH}" emit-ir "${SRC}" -o "${OUT_DIR}/hello.ir" >/dev/null
"${CLI_PATH}" emit-bytecode "${SRC}" -o "${OUT_DIR}/hello.tisc.json" >/dev/null
"${CLI_PATH}" build "${SRC}" -o "${OUT_DIR}/hello.build.tisc.json" >/dev/null

for artifact in "${OUT_DIR}/hello.tisc.json" "${OUT_DIR}/hello.build.tisc.json"; do
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

echo "cli compile checks: ok"
