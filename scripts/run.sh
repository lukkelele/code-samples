#!/usr/bin/env bash
cd "$(dirname "${BASH_SOURCE[0]}")"/.. || exit 1

. scripts/utils.sh
rust_manifest_path="$(rust_get_manifest_path)" || exit $?

case "$PROJECT_ROOT" in
    *samples/rust|*samples/rust/*|*experimental/rust|*experimental/rust/*)
        cargo run \
            --quiet \
            --manifest-path "$rust_manifest_path"
        ;;
    *)
        executable_path="${PROJECT_ROOT}/build/target"
        if [[ ! -f "$executable_path" ]]; then
            echo "[run.sh] Executable not found: $executable_path"
            exit 1
        fi
        eval "$executable_path"
        ;;
esac
