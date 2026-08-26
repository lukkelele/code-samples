#!/usr/bin/env bash
cd "$(dirname "${BASH_SOURCE[0]}")"/.. || exit 1

verbose=0

name_after_samples="${PROJECT_ROOT##*samples/}"
sample_language="${name_after_samples%%/*}"
sample_name="${name_after_samples#*/}"
sample_name="${sample_name%/src}"
sample_target="${sample_name##*/}"
relative_project_dir="${PROJECT_ROOT#$REPO_ROOT/}"

echo "---------------------------------------------------------------------------------
 [build.sh] ${sample_name} [${sample_language}]
---------------------------------------------------------------------------------"
[ $verbose -eq 1 ] && echo " SAMPLE_DIR:   ${relative_project_dir}"
[ $verbose -eq 1 ] && echo " SAMPLE_NAME:  ${sample_name}"
[ $verbose -eq 1 ] && echo " REPO_ROOT:    ${REPO_ROOT}"
[ $verbose -eq 1 ] && echo " PROJECT_ROOT: ${PROJECT_ROOT}"
[ $verbose -eq 1 ] && echo "---------------------------------------------------------------------------------"

cmake_build()
{
    if [[ ! -d "${PROJECT_ROOT}/build" ]]; then
        echo "[build.sh] Build directory missing"
        ./scripts/cmake_configure.sh
        conf_ret=$?
        if [ $conf_ret -ne 0 ]; then
            echo "[build.sh] Configuration failed (${conf_ret})"
            return 1
        fi
    fi

    ./scripts/cmake_build.sh
    build_ret=$?
    if [ $build_ret -ne 0 ]; then
        echo "[build.sh] Build failed (${build_ret})"
        return 2
    fi

    return 0
}

case "$PROJECT_ROOT" in
    *samples/rust|*samples/c|*samples/cpp)
        echo "[build.sh] Not in a sample directory: ${PROJECT_ROOT}"
        exit 1
        ;;
    *samples/rust|*samples/rust/*)
        ./scripts/cargo_build.sh || exit $?
        ;;
    *samples/c/*|*samples/cpp/*)
        cmake_build || exit $?
        ./scripts/merge_compile_commands.sh
        ;;
    *)
        echo "[build.sh] Cannot build in: ${PROJECT_ROOT}"
        exit 1
        ;;
esac
