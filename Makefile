SHELL := /bin/bash

.PHONY: help cli test-lang-core test-module-graph test-determinism test-runtime-coupled test-compat test-spec-coverage all

help:
	@echo "Targets:"
	@echo "  make cli                  Build build/bin/t81-lang"
	@echo "  make test-lang-core       Run language-only test lane"
	@echo "  make test-module-graph    Run import/module graph checks"
	@echo "  make test-determinism     Run parser AST snapshot checks"
	@echo "  make test-runtime-coupled Validate runtime-coupled manifest discipline"
	@echo "  make test-spec-coverage   Validate spec coverage matrix hygiene"
	@echo "  make test-compat          Run local compatibility gates (no VM checkout required)"
	@echo "  make all                  Run lang-core + module-graph + determinism + runtime-coupled + spec coverage checks"

cli:
	@scripts/build-t81-lang-cli.sh

test-lang-core:
	@scripts/check-lang-core.sh

test-module-graph:
	@scripts/check-module-graph.sh

test-determinism:
	@scripts/check-parser-determinism.sh

test-runtime-coupled:
	@scripts/check-runtime-coupled-tests.sh

test-spec-coverage:
	@scripts/check-spec-coverage.sh

test-compat: test-runtime-coupled

all: test-lang-core test-module-graph test-determinism test-runtime-coupled test-spec-coverage
