#!/usr/bin/env bash
# lint.sh — Run clang-format check on all C++ sources.
# Usage: ./scripts/lint.sh [--fix]   (--fix applies formatting in-place)
set -euo pipefail

REPO_ROOT="$(cd "$(dirname "${BASH_SOURCE[0]}")/.." && pwd)"
FIX=0

for arg in "$@"; do
    [[ "$arg" == "--fix" ]] && FIX=1
done

mapfile -t FILES < <(find "$REPO_ROOT/shared" "$REPO_ROOT/client-ui" "$REPO_ROOT/admin-ui" \
    \( -name "*.h" -o -name "*.cpp" -o -name "*.hpp" \) -not -path "*/build/*")

if [[ ${#FILES[@]} -eq 0 ]]; then
    echo "No C++ files found."
    exit 0
fi

if [[ $FIX -eq 1 ]]; then
    echo "Applying clang-format to ${#FILES[@]} files..."
    clang-format -i --style=file "${FILES[@]}"
    echo "Done."
else
    echo "Checking ${#FILES[@]} files with clang-format..."
    clang-format --dry-run --Werror --style=file "${FILES[@]}"
    echo "All files pass clang-format."
fi
