#!/usr/bin/env bash
set -euo pipefail

mapfile -t dbs < <(find "${REPO_ROOT}/samples" -type f -name compile_commands.json 2>/dev/null)
if [ ${#dbs[@]} -eq 0 ]; then
    echo "[merge_compile_commands.sh] No compile_commands.json found"
    exit 0
fi

mkdir -p "${REPO_ROOT}/build"
output_file="${REPO_ROOT}/build/compile_commands.json"
jq -s 'add | unique_by(.file)' "${dbs[@]}" > "${output_file}.tmp" && \
    mv "${output_file}.tmp" "${output_file}"
