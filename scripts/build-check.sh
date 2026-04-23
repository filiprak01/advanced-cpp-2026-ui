#!/usr/bin/env bash
# build-check.sh — Configure and compile the project to catch errors early.
# Used by the pre-commit hook and can be run manually.
set -euo pipefail

REPO_ROOT="$(cd "$(dirname "${BASH_SOURCE[0]}")/.." && pwd)"
BUILD_DIR="$REPO_ROOT/build/check"

echo "==> Configuring CMake..."
cmake -S "$REPO_ROOT" \
      -B "$BUILD_DIR" \
      -DCMAKE_BUILD_TYPE=Debug \
      -G Ninja \
      -DCMAKE_EXPORT_COMPILE_COMMANDS=ON \
      --log-level=WARNING

echo "==> Building..."
cmake --build "$BUILD_DIR" --parallel

echo "==> Build check passed."
