#!/usr/bin/env bash
cd "$(dirname "${BASH_SOURCE[0]}")"/.. || exit 1

if [[ -z "$PROJECT_ROOT" ]]; then
    echo "[cargo_build.sh] PROJECT_ROOT not set"
    exit 1
fi

. "scripts/utils.sh"
manifest_path="$(rust_get_manifest_path)" || exit $?

cargo build --manifest-path "$manifest_path"
