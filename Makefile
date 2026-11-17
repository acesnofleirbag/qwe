# Copyright (c) The Quickware Editor, 2026

BIN = qwe

all: run

.PHONY: build
build:
	@go build -o build/$(BIN) .

.PHONY: run
run: build
	@./build/$(BIN)

.PHONY: fmt
fmt:
	go fmt ./...

