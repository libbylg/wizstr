#!/usr/bin/env bash
#set -x

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
    echo -e "\033[31m$1\033[0m" >& 2
}
export -f error

function warning()
{
    echo -e "\033[33m$1\033[0m" >& 2
}
export -f warning

function cmd_help()
{
    echo    "Usage:"
    echo    "    build.sh [compile] [release|debug] [-c] [-j N]"
    echo    "    build.sh cdb [release|debug]"
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

    if [[ "${range}" == "all" || "${range}" == "compile" || "${range}" == "debug" || "${range}" == "release" ]]; then
        echo   "Clean: ${PROJECT_ROOT}/output-compile"
        echo   "Clean: ${PROJECT_ROOT}/build"
        rm -rf "${PROJECT_ROOT}/output-compile"
        rm -rf "${PROJECT_ROOT}/build"
    fi
    
    if [[ "${range}" == "all" || "${range}" == "cdb" || "${range}" == "debug" ]]; then
        echo   "Clean: ${PROJECT_ROOT}/output-cdb"
        rm -rf "${PROJECT_ROOT}/output-cdb"
    fi
    
    if [[ "${range}" == "all" || "${range}" == "test" || "${range}" == "ut" ]]; then
        echo   "Clean: ${PROJECT_ROOT}/output-test"
        rm -rf "${PROJECT_ROOT}/output-test"
    fi
    
    if [[ "${range}" == "all" || "${range}" == "install" || "${range}" == "dist" ]]; then
        echo   "Clean: ${PROJECT_ROOT}/output-install"
        rm -rf "${PROJECT_ROOT}/output-install"
    fi
    
    if [[ "${range}" == "all" || "${range}" == "ide" || "${range}" == "cmake" ]]; then
        echo   "Clean: ${PROJECT_ROOT}/cmake-*"
        rm -rf "${PROJECT_ROOT}"/cmake-*
    fi

    return  0
}

function cmd_compile()
{
    local opt_compile_mode=""
    local opt_clean_old="no"
    local opt_parallel_num=""

    # Read all options
    local pos_num=1
    shift
    while [[ $# -gt 0 ]]; do
        if [[ "$1" == "-c" ]]; then
            opt_clean_old="yes"
            shift
            continue
        fi

        if [[ "$1" == "-j" ]]; then
            shift
            if [[ $# -gt 0 ]]; then
                shift
                opt_parallel_num="$1"
            fi
            continue
        fi 

        if [[ "${pos_num}" -eq 1 ]]; then
            pos_num=$((pos_num + 1))
            opt_compile_mode="debug"
            shift
            continue
        fi

        error "Unsupported option '$1'"
        return 1
    done

    # Check and rectify options
    if [[ -z "${opt_compile_mode}" ]]; then
        opt_compile_mode="debug"
    fi

    if [[ -z "${opt_parallel_num}" ]]; then
        opt_parallel_num=$(cat "/proc/cpuinfo" | grep -E '^processor(\s)*:' | wc -l)
        # if [[ ! -z "${opt_parallel_num}" ]]; then
        #     opt_parallel_num=$((opt_parallel_num / 2))
        # fi
    fi

    if [[ -z "${opt_parallel_num}" || "${opt_parallel_num}" -eq 0 ]]; then
        opt_parallel_num=1
    fi

    #   If compile directory is exist, clean it
    local compile_dir="${PROJECT_ROOT}/output-compile"

    #   -c to clean old compile output, that is full-compile
    if [[ "${opt_clean_old}" == "yes" ]]; then
        if [[ -d "${compile_dir}" ]]; then
            rm -rf "${compile_dir}"
        fi
    fi 

    mkdir -p "${compile_dir}"
    if [[ ! -d "${compile_dir}" ]]; then
        error   "Can not create the compile output directory: '${compile_dir}'"
        return  1
    fi

    #   Generate the compile database
    if [[ "${BUILD_COMPILE_DATABASE}" != "" ]]; then
        cd "${compile_dir}" \
            && cmake .. -DCMAKE_EXPORT_COMPILE_COMMANDS=1   \
            && cp "${compile_dir}/compile_commands.json" "${BUILD_COMPILE_DATABASE}" \
            && echo  "Generate compile database success: '${install_dir}'"
        RESULT=$?
        if [[ ${RESULT} -ne 0 ]]; then
            error   "Generate compile database failed(${RESULT})"
            return  1
        fi
        return  $?
    fi

    #   Normal compile project
    cd "${compile_dir}" \
        && cmake ..   \
        && make -j "${opt_parallel_num}" \
        && echo  "Compile success: '${opt_install_dir}'"
    RESULT=$?
    if [[ ${RESULT} -ne 0 ]]; then
        error   "Compile failed(${RESULT}): '${compile_dir}'"
        return  1
    fi

    return  0
}

function cmd_install()
{
    local opt_install_dir="$2"
    if [[ -z "${opt_install_dir}" ]]; then
        opt_install_dir="${PROJECT_ROOT}/output-install"
    fi

    if [[ -d "${opt_install_dir}" ]]; then
        rm -rf "${opt_install_dir}"
    fi

    mkdir -p "${opt_install_dir}"
    if [[ ! -d "${opt_install_dir}" ]]; then
        error   "Can not create the install target directory: '${opt_install_dir}'"
        return  1
    fi

    if [[ ! -d "${compile_dir}/str_test" ]]; then
        error   "Need to compile first"
        return  1
    fi
    
    cp "${compile_dir}/str_test" "${opt_install_dir}" \
        && echo "Install target success"
    RESULT=$?
    if [[ ${RESULT} -ne 0 ]]; then
        error   "Install target failed(${RESULT}): '${opt_install_dir}'"
        return  1
    fi

    return  0
}

function cmd_test()
{
    local testcase_pattern="$2"

    local test_output_dir="${PROJECT_ROOT}/output-test"
    if [[ ! -d "${test_output_dir}" ]]; then
        mkdir -p "${test_output_dir}"
    fi

    if [[ ! -f "${PROJECT_ROOT}/dist/str_test" ]]; then
        error   "Need to compile ut target first, try -h for help"
        return  1
    fi

    cd "${test_output_dir}" \
        && "${PROJECT_ROOT}/dist/str_test" ${testcase_pattern}
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

    local cdb_output_dir="${PROJECT_ROOT}/output-cdb"
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
    else
        action="$1"
        shift
    fi

    case "${action}" in
        help|-h|--help)
            cmd_help        "${action}" "$@"
            return          "$?"
            ;;
        clean)
            cmd_clean       "${action}" "$@"
            return          "$?"
            ;;
        compile)
            cmd_compile     "${action}" "$@"
            return          "$?"
            ;;
        debug)
            cmd_compile     "compile" "debug" "$@"
            return          "$?"
            ;;
        release)
            cmd_compile     "compile" "release" "$@"
            return          "$?"
            ;;
        test)
            cmd_test        "${action}" "$@"
            return          "$?"
            ;;
        cdb)
            cmd_cdb         "${action}" "$@"
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
