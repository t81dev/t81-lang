# T81Lang Specification (Living)

This file is the top-level, living specification entry point for T81Lang.

## Canonical Sources

- Language scope: `docs/spec/language-scope.md`
- Full migrated language spec: `spec/t81lang-spec.md`
- Architecture constraints: `docs/architecture/compiler-architecture.md`
- Compatibility contract: `docs/architecture/compatibility-matrix.md`

## Update Rule

When behavior changes in parser, semantics, or codegen, update this file first with the normative intent, then update detailed spec/RFC documents.

## Current Focus Areas

- Deterministic parsing and canonical AST shape.
- Explicit ternary and structural type semantics.
- Stable TISC IR lowering contract.
- Reproducible diagnostics and artifact metadata.
