#!/usr/bin/env bash

rust_get_manifest_path()
{
    if [[ -f "${PROJECT_ROOT}/Cargo.toml" ]]; then
        printf '%s\n' "${PROJECT_ROOT}/Cargo.toml"
    elif [[ -f "${PROJECT_ROOT}/../Cargo.toml" ]]; then
        printf '%s\n' "${PROJECT_ROOT}/../Cargo.toml"
    else
        printf '%s\n' "Failed to find Cargo.toml" >&2
        return 2
    fi
}
