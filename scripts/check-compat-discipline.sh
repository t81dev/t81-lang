#!/usr/bin/env bash
set -euo pipefail

BASE_REF="${BASE_REF:-main}"
REMOTE_BASE="origin/${BASE_REF}"
MATRIX_FILE="docs/architecture/compatibility-matrix.md"
CHANGELOG_FILE="CHANGELOG.md"

if ! git show-ref --verify --quiet "refs/remotes/${REMOTE_BASE}"; then
  git fetch origin "${BASE_REF}:${BASE_REF}" >/dev/null 2>&1 || true
  if ! git show-ref --verify --quiet "refs/remotes/${REMOTE_BASE}"; then
    echo "warning: missing ${REMOTE_BASE}; skipping discipline check"
    exit 0
  fi
fi

changed="$(git diff --name-only "${REMOTE_BASE}...HEAD")"
matrix_changed=0
changelog_changed=0

if printf '%s\n' "${changed}" | rg -qx "${MATRIX_FILE}"; then
  matrix_changed=1
fi
if printf '%s\n' "${changed}" | rg -qx "${CHANGELOG_FILE}"; then
  changelog_changed=1
fi

if [[ "${matrix_changed}" -eq 1 && "${changelog_changed}" -ne 1 ]]; then
  echo "error: ${MATRIX_FILE} changed but ${CHANGELOG_FILE} was not updated"
  exit 1
fi

echo "compat discipline: ok"
