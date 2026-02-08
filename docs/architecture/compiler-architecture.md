# Compiler Architecture

## Pipeline

1. Source loader + module graph normalizer.
2. Lexer/parser -> concrete syntax tree.
3. AST lowering + symbol resolution.
4. Type/effect checking -> typed IR.
5. Typed IR lowering -> TISC IR.
6. TISC IR -> HanoiVM bytecode + provenance manifest.

## Deterministic Requirements

- Canonical parse tree normalization.
- Stable traversal order in every compiler pass.
- Content-hash identity for intermediate artifacts.
- Reproducible diagnostics (`code`, `message`, `span`, `fix`) ordering.

## Runtime Contract

- Bytecode format and VM op semantics align with `t81-foundation`/HanoiVM.
- Axion safety policy handoff is explicit in emitted metadata.
- No silent fallback to non-ternary semantics.

## Interfaces

- CLI: `t81-lang parse|check|build|emit-ir|emit-bytecode`.
- Library API: parser, checker, and codegen as embeddable modules.
- Future FFI: bindings for `t81-python` and research tooling.
