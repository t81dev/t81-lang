#!/usr/bin/env bash
set -euo pipefail

# Sync language-owned assets from a local t81-foundation checkout.
# Usage:
#   ./scripts/migrate-from-foundation.sh /path/to/t81-foundation
#
# Note:
#   This script intentionally does not overwrite local contract-layer headers
#   under include/t81/tisc/ that are maintained in t81-lang for split isolation.

SRC_ROOT="${1:-/Users/t81dev/Code/t81-foundation}"
DST_ROOT="$(cd "$(dirname "${BASH_SOURCE[0]}")/.." && pwd)"

if [[ ! -d "$SRC_ROOT" ]]; then
  echo "error: source repo not found: $SRC_ROOT" >&2
  exit 1
fi

copy_file() {
  local rel="$1"
  mkdir -p "$DST_ROOT/$(dirname "$rel")"
  cp "$SRC_ROOT/$rel" "$DST_ROOT/$rel"
}

copy_glob() {
  local src_dir="$1"
  local pattern="$2"
  local dst_dir="$3"
  mkdir -p "$DST_ROOT/$dst_dir"
  find "$SRC_ROOT/$src_dir" -maxdepth 1 -type f -name "$pattern" -exec cp {} "$DST_ROOT/$dst_dir/" \;
}

# Core frontend implementation
mkdir -p "$DST_ROOT/include/t81" "$DST_ROOT/src"
rm -rf "$DST_ROOT/include/t81/frontend" "$DST_ROOT/src/frontend"
cp -R "$SRC_ROOT/include/t81/frontend" "$DST_ROOT/include/t81/"
cp -R "$SRC_ROOT/src/frontend" "$DST_ROOT/src/"

# Legacy language API declarations currently living in foundation headers
mkdir -p "$DST_ROOT/include/t81" "$DST_ROOT/src/lang"
rm -rf "$DST_ROOT/include/t81/lang"
cp -R "$SRC_ROOT/include/t81/lang" "$DST_ROOT/include/t81/"
copy_file "src/lang/test.md"

# Language examples
mkdir -p "$DST_ROOT/examples"
find "$DST_ROOT/examples" -maxdepth 1 -type f -name '*.t81' -delete
copy_glob "examples" '*.t81' "examples"

# Language docs and spec
copy_file "docs/guides/adding-a-language-feature.md"
copy_file "docs/guides/match-example.md"
copy_file "docs/guides/match-patterns.md"
copy_file "spec/t81lang-spec.md"
copy_file "spec/rfcs/RFC-0007-t81lang-standard-library.md"
copy_file "spec/rfcs/RFC-0011-t81lang-grammar-update.md"

# Language-focused tests
mkdir -p "$DST_ROOT/tests/common" "$DST_ROOT/tests/syntax" "$DST_ROOT/tests/semantics" "$DST_ROOT/tests/roundtrip"
for f in \
  frontend_fuzz_test.cpp \
  frontend_ir_generator_relation_test.cpp \
  frontend_ir_generator_test.cpp \
  frontend_lexer_test.cpp \
  frontend_parser_generics_test.cpp \
  frontend_parser_legacy_rejection_test.cpp \
  frontend_parser_test.cpp \
  semantic_analyzer_equality_test.cpp \
  semantic_analyzer_generic_test.cpp \
  semantic_analyzer_loop_test.cpp \
  semantic_analyzer_match_test.cpp \
  semantic_analyzer_numeric_test.cpp \
  semantic_analyzer_option_result_test.cpp \
  semantic_analyzer_record_enum_test.cpp \
  semantic_analyzer_vector_literal_test.cpp \
  e2e_advanced_features_test.cpp \
  e2e_arithmetic_test.cpp \
  e2e_if_statement_test.cpp \
  e2e_let_statement_test.cpp \
  e2e_loop_statement_test.cpp \
  e2e_match_expression_test.cpp \
  e2e_option_result_function_test.cpp \
  e2e_option_result_test.cpp \
  e2e_option_type_test.cpp \
  cli_option_result_test.cpp \
  cli_record_enum_test.cpp \
  cli_structural_types_test.cpp \
  lang_literal_pool_test.cpp; do
  copy_file "tests/cpp/$f"
done

# Re-home tests into the split suite layout.
mv "$DST_ROOT/tests/cpp/test_utils.hpp" "$DST_ROOT/tests/common/test_utils.hpp"
for f in frontend_lexer_test.cpp frontend_parser_test.cpp frontend_parser_generics_test.cpp frontend_parser_legacy_rejection_test.cpp frontend_fuzz_test.cpp; do
  mv "$DST_ROOT/tests/cpp/$f" "$DST_ROOT/tests/syntax/$f"
done
for f in semantic_analyzer_equality_test.cpp semantic_analyzer_generic_test.cpp semantic_analyzer_loop_test.cpp semantic_analyzer_match_test.cpp semantic_analyzer_numeric_test.cpp semantic_analyzer_option_result_test.cpp semantic_analyzer_record_enum_test.cpp semantic_analyzer_vector_literal_test.cpp; do
  mv "$DST_ROOT/tests/cpp/$f" "$DST_ROOT/tests/semantics/$f"
done
for f in cli_option_result_test.cpp cli_record_enum_test.cpp cli_structural_types_test.cpp e2e_advanced_features_test.cpp e2e_arithmetic_test.cpp e2e_if_statement_test.cpp e2e_let_statement_test.cpp e2e_loop_statement_test.cpp e2e_match_expression_test.cpp e2e_option_result_function_test.cpp e2e_option_result_test.cpp e2e_option_type_test.cpp frontend_ir_generator_relation_test.cpp frontend_ir_generator_test.cpp lang_literal_pool_test.cpp; do
  mv "$DST_ROOT/tests/cpp/$f" "$DST_ROOT/tests/roundtrip/$f"
done

# Shared helper include path fix.
sed -i '' 's/#include "test_utils.hpp"/#include "..\/common\/test_utils.hpp"/' \
  "$DST_ROOT/tests/syntax/frontend_parser_test.cpp" \
  "$DST_ROOT/tests/syntax/frontend_parser_generics_test.cpp" \
  "$DST_ROOT/tests/semantics/semantic_analyzer_equality_test.cpp" \
  "$DST_ROOT/tests/semantics/semantic_analyzer_record_enum_test.cpp"

mkdir -p "$DST_ROOT/tests/harness/test_vectors/lang_samples"
copy_file "tests/harness/test_vectors/lang_samples/fib.t81"
copy_file "tests/harness/test_vectors/lang_samples/hello_world.t81"
copy_file "tests/harness/test_vectors/lang_samples/tensors.t81"

# Migration manifest
manifest="$DST_ROOT/docs/migration/migrated-from-t81-foundation.tsv"
mkdir -p "$(dirname "$manifest")"
{
  echo -e "category\tsource_path\ttarget_path\tnotes"
  while IFS= read -r rel; do
    case "$rel" in
      include/t81/frontend/*|src/frontend/*) category="frontend"; notes="core frontend implementation" ;;
      include/t81/lang/*|src/lang/*) category="lang-api"; notes="legacy language API declarations" ;;
      examples/*.t81) category="examples"; notes="language example" ;;
      tests/syntax/*|tests/semantics/*|tests/roundtrip/*|tests/common/*|tests/harness/test_vectors/lang_samples/*) category="tests"; notes="language-focused regression test/sample" ;;
      docs/guides/*) category="docs"; notes="language contributor guide" ;;
      spec/t81lang-spec.md|spec/rfcs/*) category="spec"; notes="language spec or RFC" ;;
      *) continue ;;
    esac
    echo -e "$category\t$SRC_ROOT/$rel\t$rel\t$notes"
  done < <(
    {
      find "$DST_ROOT/include/t81/frontend" -type f | sed "s#^$DST_ROOT/##"
      find "$DST_ROOT/src/frontend" -type f | sed "s#^$DST_ROOT/##"
      find "$DST_ROOT/include/t81/lang" -type f | sed "s#^$DST_ROOT/##"
      find "$DST_ROOT/src/lang" -type f | sed "s#^$DST_ROOT/##"
      find "$DST_ROOT/examples" -maxdepth 1 -type f -name '*.t81' | sed "s#^$DST_ROOT/##"
      find "$DST_ROOT/tests/syntax" -maxdepth 1 -type f -name '*.cpp' | sed "s#^$DST_ROOT/##"
      find "$DST_ROOT/tests/semantics" -maxdepth 1 -type f -name '*.cpp' | sed "s#^$DST_ROOT/##"
      find "$DST_ROOT/tests/roundtrip" -maxdepth 1 -type f -name '*.cpp' | sed "s#^$DST_ROOT/##"
      find "$DST_ROOT/tests/common" -maxdepth 1 -type f -name 'test_utils.hpp' | sed "s#^$DST_ROOT/##"
      find "$DST_ROOT/tests/harness/test_vectors/lang_samples" -type f | sed "s#^$DST_ROOT/##"
      find "$DST_ROOT/docs/guides" -maxdepth 1 -type f \( -name 'adding-a-language-feature.md' -o -name 'match-example.md' -o -name 'match-patterns.md' \) | sed "s#^$DST_ROOT/##"
      echo "spec/t81lang-spec.md"
      find "$DST_ROOT/spec/rfcs" -maxdepth 1 -type f \( -name 'RFC-0007-t81lang-standard-library.md' -o -name 'RFC-0011-t81lang-grammar-update.md' \) | sed "s#^$DST_ROOT/##"
    } | sort -u
  )
} > "$manifest"

echo "Migration sync complete."
echo "Manifest: $manifest"
