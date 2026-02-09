#!/usr/bin/env bash
set -euo pipefail

ROOT="$(cd "$(dirname "${BASH_SOURCE[0]}")/.." && pwd)"
CLI_PATH="$("${ROOT}/scripts/build-t81-lang-cli.sh")"

run_expect_success() {
  local file="$1"
  if ! "${CLI_PATH}" check "${file}" >/dev/null 2>"${ROOT}/build/module-graph.err"; then
    echo "module graph check unexpectedly failed for ${file}" >&2
    sed -n '1,80p' "${ROOT}/build/module-graph.err" >&2 || true
    exit 1
  fi
}

run_expect_failure() {
  local file="$1"
  local needle="$2"
  if "${CLI_PATH}" check "${file}" >/dev/null 2>"${ROOT}/build/module-graph.err"; then
    echo "module graph check unexpectedly succeeded for ${file}" >&2
    exit 1
  fi
  if ! rg -q "${needle}" "${ROOT}/build/module-graph.err"; then
    echo "module graph error output did not include expected pattern '${needle}'" >&2
    sed -n '1,80p' "${ROOT}/build/module-graph.err" >&2 || true
    exit 1
  fi
}

mkdir -p "${ROOT}/build"

run_expect_success "${ROOT}/tests/harness/module_graph/ok/app/main.t81"
run_expect_failure "${ROOT}/tests/harness/module_graph/missing/app/main.t81" "missing import"
run_expect_failure "${ROOT}/tests/harness/module_graph/cycle/app/a.t81" "import cycle detected"

echo "module graph checks: ok"
