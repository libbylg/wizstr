#!/usr/bin/env bash
set -x

SELFDIR=$(cd $(dirname "${BASH_SOURCE[0]}"); pwd)

PROJECT_ROOT="${SELFDIR}"
while [[ 0 -eq 0 ]]; do
    if [[ "${PROJECT_ROOT}/PROJECT" ]]; then
        break
    fi

    if [[ -z "${PROJECT_ROOT}" || "${PROJECT_ROOT}" == "/" ]]; then
        exit    1
    fi

    PROJECT_ROOT=$(dirname "${PROJECT_ROOT}")
done

function error()
{
    echo "\033[31m$1\033[0m" >& 2
}
export -f error

function warning()
{
    echo "\033[33m$1\033[0m" >& 2
}
export -f warning

function cmd_help()
{
    echo    "Usage:"
    echo    "    build.sh [compile] [release|debug]"
    echo    "    build.sh test"
    echo    "    build.sh clean"
    echo    "    build.sh help|-h|--help  Show this help"

    return  0
}

function cmd_clean()
{
    local range="$2"
    if [[ "${range}" == "" ]]; then
        range="all"
    fi

    if [[ "${range}" == "all" || "${range}" == "compile" || "${range}" == "debug" ]]; then
        echo   "Clean: ${PROJECT_ROOT}/output-compile-debug"
        rm -rf "${PROJECT_ROOT}/output-compile-debug"
    fi
    
    if [[ "${range}" == "all" || "${range}" == "compile" || "${range}" == "release" ]]; then
        echo   "Clean: ${PROJECT_ROOT}/output-compile-release"
        rm -rf "${PROJECT_ROOT}/output-compile-release"
    fi
    
    if [[ "${range}" == "all" || "${range}" == "cdb" || "${range}" == "debug" ]]; then
        echo   "Clean: ${PROJECT_ROOT}/output-cdb-debug"
        rm -rf "${PROJECT_ROOT}/output-cdb-debug"
    fi


    if [[ "${range}" == "all" || "${range}" == "cdb" || "${range}" == "release" ]]; then
        echo   "Clean: ${PROJECT_ROOT}/output-cdb-release"
        rm -rf "${PROJECT_ROOT}/output-cdb-release"
    fi

    return  0
}

function cmd_compile()
{
    local compile_mode="$2"

    if [[ "${compile_mode}" == "" ]]; then
        compile_mode="release"
    fi

    # If compile directory is exist, clean it
    local compile_dir="${PROJECT_ROOT}/output-compile-${compile_mode}"
    if [[ -d "${compile_dir}" ]]; then
        rm -rf "${compile_dir}"
    fi

    mkdir -p "${compile_dir}"
    if [[ ! -d "${compile_dir}" ]]; then
        error   "Can not clean and recreate compile directory: '${compile_dir}'"
    fi

    # If compile directory is exist, clean it
    local install_dir="${PROJECT_ROOT}/dist"
    if [[ -d "${install_dir}" ]]; then
        rm -rf "${install_dir}"
    fi

    mkdir -p "${install_dir}"
    if [[ ! -d "${install_dir}" ]]; then
        error   "Can not clean and recreate install directory: '${install_dir}'"
    fi

    #   如果指定了只生成编译数据库
    if [[ "${BUILD_COMPILE_DATABASE}" != "" ]]; then
        cd "${compile_dir}" \
            && cmake .. -DCMAKE_EXPORT_COMPILE_COMMANDS=1   \
            && cp "${compile_dir}/compile_commands.json" "${BUILD_COMPILE_DATABASE}"
        return  $?
    fi

    cd "${compile_dir}" \
        && cmake .. -DCMAKE_INSTALL_PREFIX="${install_dir}"   \
        && make \
        && echo  "Compile success: '${install_dir}'"
    RESULT=$?
    if [[ ${RESULT} -ne 0 ]]; then
        error   "Compile failed(${RESULT})"
        return  1
    fi

    return  0
}

function cmd_test()
{
    local testcase_pattern="$2"

    cmd_compile "compile" "debug"
    RESULT=$?
    if [[ ${RESULT} -ne 0 ]]; then
        return  1
    fi

    local test_output_dir="${PROJECT_ROOT}/output-test"
    cd "${test_output_dir}" && ${PROJECT_ROOT}/dist/str_test ${testcase_pattern}
    RESULT=$?
    if [[ ${RESULT} -ne 0 ]]; then
        return  1
    fi

    return  0
}

# cdb debug|release
function cmd_cdb()
{
    local compile_mode="$2"
    if [[ $# -lt 2 || "${compile_mode}" == "" ]]; then
        error   "Missing parameter for cdb, try -h for help"
        return  1
    fi

    local cdb_output_dir="${PROJECT_ROOT}/output-cdb-${compile_mode}"
    if [[ -d "${cdb_output_dir}" ]]; then
        rm -rf "${cdb_output_dir}"
    fi

    mkdir -p "${cdb_output_dir}"

    export BUILD_COMPILE_DATABASE="${cdb_output_dir}/compile-cdb-${compile_mode}.json"
    rm -rf "${BUILD_COMPILE_DATABASE}"

    cmd_compile "$@"
    RESULT=$?
    if [[ ${RESULT} -ne 0 ]]; then
        error   "Generate compile database file failed(${RESULT})"
        return  1
    fi

    if [[ ! -f "${BUILD_COMPILE_DATABASE}" ]]; then
        error   "Generate compile database file failed, no compile database file generated."
        return  1
    fi

    echo    "Generate compile database file success: ${BUILD_COMPILE_DATABASE}"
    return  0
}

function main()
{
    local action="$1"
    if [[ "${action}" == "" ]]; then
        action="compile"
    fi

    case "${action}" in
        help|-h|--help)
            cmd_help        "$@"
            return          "$?"
            ;;
        clean)
            cmd_clean       "$@"
            return          "$?"
            ;;
        compile)
            cmd_compile     "$@"
            return          "$?"
            ;;
        test)
            cmd_test        "$@"
            return          "$?"
            ;;
        cdb)
            cmd_cdb         "$@"
            return          "$?"
            ;;
        *)
            error   "Unsupported subcommand '${action}'"
            return  1
            ;;
    esac
}

main "$@"
exit "$?"
