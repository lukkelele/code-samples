#!/usr/bin/env bash
cd "$(dirname "${BASH_SOURCE[0]}")"/.. || exit 1

executable_path="build/app/app"
if [[ ! -f "$executable_path" ]]; then
    echo "[run.sh] Executable not found: $executable_path"
    exit 1
fi

eval "$executable_path"
