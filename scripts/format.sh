#!/usr/bin/env bash
cd "$(dirname "${BASH_SOURCE[0]}")"/.. || exit 1

dirs=("samples" "experimental")
extensions=("*.h" "*.cpp")

for dir in "${dirs[@]}"; do
    [[ -d "$dir" ]] || continue
    for ext in "${extensions[@]}"; do
        while IFS= read -r -d '' file; do
            clang-format -i "$file"
        done < <(find "$dir" -name "$ext" -print0)
    done
done
