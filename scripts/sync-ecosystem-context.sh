#!/usr/bin/env bash
set -euo pipefail

OWNER="${1:-t81dev}"
OUT_DIR="${2:-docs/architecture}"
OUT_FILE="$OUT_DIR/ecosystem-repos.json"

mkdir -p "$OUT_DIR"

if ! command -v curl >/dev/null 2>&1; then
  echo "error: curl is required" >&2
  exit 1
fi

if ! command -v jq >/dev/null 2>&1; then
  echo "error: jq is required" >&2
  exit 1
fi

curl -fsSL "https://api.github.com/users/${OWNER}/repos?per_page=100" \
  | jq 'map({
      name,
      full_name,
      html_url,
      description,
      language,
      default_branch,
      archived,
      fork,
      pushed_at,
      updated_at
    }) | sort_by(.name)' > "$OUT_FILE"

echo "wrote $OUT_FILE"
