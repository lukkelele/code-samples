#!/usr/bin/env bash
echo "[cmake_configure.sh] REPO_ROOT=${REPO_ROOT} PROJECT_ROOT=${PROJECT_ROOT}"

cmake_args=(
    --fresh
    -DCMAKE_EXPORT_COMPILE_COMMANDS=ON
    -DCMAKE_BUILD_TYPE=Debug
)

cmake \
    -S ${PROJECT_ROOT} \
    -B ${REPO_ROOT}/build \
    "${cmake_args[@]}"
