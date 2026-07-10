#!/usr/bin/env bash
cd "$(dirname "${BASH_SOURCE[0]}")"/.. || exit 1

if [[ ! -d "${PROJECT_ROOT}/build" ]]; then
    echo "[cmake_build.sh] Build directory missing"
    exit 2
fi

args=(
    --parallel
)

cmake \
    --build "${PROJECT_ROOT}/build" \
    "${args[@]}"
