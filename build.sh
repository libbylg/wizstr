#!/usr/bin/env bash
# Copyright (c) 2021-2025  libbylg@126.com
# str is licensed under Mulan PSL v2.
# You can use this software according to the terms and conditions of the Mulan PSL v2.
# You may obtain a copy of Mulan PSL v2 at:
#          http://license.coscl.org.cn/MulanPSL2
# THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER
# EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY OR
# FIT FOR A PARTICULAR PURPOSE.
# See the Mulan PSL v2 for more details.

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
    echo    "    build.sh install [<InstalTargetDirectory>]"
    echo    "    build.sh test"
    echo    "    build.sh clean all|compile|debug|release|cdb|test|install|ide|doc||cmake"
    echo    "    build.sh help|-h|--help  Show this help"

    return  0
}

function cmd_clean()
{
    local range="$2"
    if [[ "${range}" == "" ]]; then
        range="all"
    fi

    if [[ "${range}" == "all" || "${range}" == "output" ]]; then
        echo   "Clean: output-*"
        rm -rf "${PROJECT_ROOT}"/output-*
    fi

    if [[ "${range}" == "all" || "${range}" == "compile" || "${range}" == "debug" || "${range}" == "release" ]]; then
        echo   "Clean: output-compile"
        echo   "Clean: build"
        rm -rf "${PROJECT_ROOT}/output-compile"
        rm -rf "${PROJECT_ROOT}/build"
    fi
    
    if [[ "${range}" == "all" || "${range}" == "cdb" || "${range}" == "debug" ]]; then
        echo   "Clean: output-cdb"
        rm -rf "${PROJECT_ROOT}/output-cdb"
    fi
    
    if [[ "${range}" == "all" || "${range}" == "test" || "${range}" == "ut" ]]; then
        echo   "Clean: output-test"
        rm -rf "${PROJECT_ROOT}/output-test"
    fi
    
    if [[ "${range}" == "all" || "${range}" == "install" || "${range}" == "dist" ]]; then
        echo   "Clean: output-install"
        rm -rf "${PROJECT_ROOT}/output-install"
    fi
    
    if [[ "${range}" == "all" || "${range}" == "ide" || "${range}" == "cmake" ]]; then
        echo   "Clean: cmake-*"
        rm -rf "${PROJECT_ROOT}"/cmake-*
    fi

    if [[ "${range}" == "all" || "${range}" == "doc" ]]; then
        echo   "Clean: templates/gendoc"
        echo   "Clean: templates"/example*
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
        if [[ -f "/proc/cpuinfo" ]]; then
            opt_parallel_num=$(cat "/proc/cpuinfo" | grep -E '^processor(\s)*:' | wc -l)
        fi
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

    #   Clean metadata files
    local compile_metadata_file="${compile_dir}/compile-metadata.properties"
    rm -rf "${compile_metadata_file}"
    if [[ -f "${compile_metadata_file}" ]]; then
        error   "Can not remove compile-metadata-file: '${compile_metadata_file}'"
        return  1
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

    #   Write compile metadata file
    local compile_metadata_file_writing="${compile_metadata_file}.writing"
    rm -rf  "${compile_metadata_file_writing}"
    echo    "compile.mode  ${opt_compile_mode}"    >>  "${compile_metadata_file_writing}"
    mv -f   "${compile_metadata_file_writing}"    "${compile_metadata_file}"
    if [[ ! -f "${compile_metadata_file}" ]]; then
        return 1
    fi

    return  0
}

#   install <InstallTargetDirectory>
function cmd_install()
{
    local opt_install_dir="$2"
    if [[ -z "${opt_install_dir}" ]]; then
        opt_install_dir="${PROJECT_ROOT}/output-install"
    fi

    mkdir -p "${opt_install_dir}"
    if [[ ! -d "${opt_install_dir}" ]]; then
        error   "Can not create the install target directory: '${opt_install_dir}'"
        return  1
    fi

    local compile_dir="${PROJECT_ROOT}/output-compile"
    if [[ ! -f "${compile_dir}/compile-metadata.properties" ]]; then
        error   "Need execute 'sh build.sh compile' first: '${compile_dir}/compile-metadata.properties'"
        return  1
    fi

    cd "${compile_dir}" \
        && cmake .. -DCMAKE_INSTALL_DIR="${opt_install_dir}"    \
        && make install
    RESULT=$?
    if [[ ${RESULT} -ne 0 ]]; then
        error   "Install failed(${RESULT}): '${opt_install_dir}'"
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

#  $1   doc
function cmd_doc()
{
    rm -rf   "${PROJECT_ROOT}/output-doc"
    mkdir -p "${PROJECT_ROOT}/output-doc"  \
        &&  cd "${PROJECT_ROOT}/output-doc" \
        &&  cmake ..    \
        &&  make doc    \
        &&  echo ""
    RESULT=$?
    return ${RESULT}
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
        doc)
            cmd_doc         "${action}" "$@"
            return          "$?"
            ;;
        install)
            cmd_install     "${action}" "$@"
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
