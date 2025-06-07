#!/usr/bin/env bash
# Copyright (c) 2021-2024 libbylg@126.com
# str is licensed under Mulan PSL v2.
# You can use this software according to the terms and conditions of the Mulan PSL v2.
# You may obtain a copy of Mulan PSL v2 at:
#          http://license.coscl.org.cn/MulanPSL2
# THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER
# EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY OR
# FIT FOR A PARTICULAR PURPOSE.
# See the Mulan PSL v2 for more details.
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

function error() {
    echo -e "\033[31m$1\033[0m" >& 2
}

function main() {
    if [[ ! -f "${SELFDIR}/gendoc" ]]; then
        error   "Document generation tool is not compiled: '${SELFDIR}/gendoc'"
        return  1
    fi

    if [[ ! -f "${SELFDIR}/example1" ]]; then
        error   "example1 is not compiled: '${SELFDIR}/example1'"
        return  1
    fi

    #   README.md
    echo    "Start generate README.md ..."
    rm -rf "${PROJECT_ROOT}/README.md"
    "${SELFDIR}/gendoc" "${SELFDIR}/README.md" -r "${PROJECT_ROOT}" -o "${PROJECT_ROOT}/README.md"
    RESULT=$?
    if [[ ${RESULT} -ne 0 ]]; then
        error   "Generate README.md failed: '${SELFDIR}/README.md' -> '${PROJECT_ROOT}/README.md'"
        return  1
    fi
    echo    "Generate README.md success: '${SELFDIR}/README.md' -> '${PROJECT_ROOT}/README.md'"

    #   README.en.md
    echo    "Start generate README.en.md"
    rm -rf "${PROJECT_ROOT}/README.en.md"
    "${SELFDIR}/gendoc" "${SELFDIR}/README.en.md" -r "${PROJECT_ROOT}" -o "${PROJECT_ROOT}/README.en.md"
    RESULT=$?
    if [[ ${RESULT} -ne 0 ]]; then
        error   "Generate README.en.md failed: '${SELFDIR}/README.en.md' -> '${PROJECT_ROOT}/README.en.md'"
        return  1
    fi
    echo   "Generate README.en.md success: '${SELFDIR}/README.en.md' -> '${PROJECT_ROOT}/README.en.md'"

    #   FAQ.md
    echo    "Start generate FAQ.md"
    rm -rf "${PROJECT_ROOT}/docs/FAQ.md"
    "${SELFDIR}/gendoc" "${SELFDIR}/FAQ.md" -r "${PROJECT_ROOT}" -o "${PROJECT_ROOT}/docs/FAQ.md"
    RESULT=$?
    if [[ ${RESULT} -ne 0 ]]; then
        error   "Generate FAQ.md failed: '${SELFDIR}/FAQ.md' -> '${PROJECT_ROOT}/docs/FAQ.md'"
        return  1
    fi
    echo   "Generate FAQ.md success: '${SELFDIR}/FAQ.md' -> '${PROJECT_ROOT}/docs/FAQ.md'"

    #   MANUAL.md
    echo    "Start generate MANUAL.md"
    rm -rf "${PROJECT_ROOT}/docs/MANUAL.md"
    "${SELFDIR}/gendoc" "${SELFDIR}/MANUAL.md" -r "${PROJECT_ROOT}" -o "${PROJECT_ROOT}/docs/MANUAL.md"
    RESULT=$?
    if [[ ${RESULT} -ne 0 ]]; then
        error   "Generate MANUAL.md failed: '${SELFDIR}/MANUAL.md' -> '${PROJECT_ROOT}/docs/MANUAL.md'"
        return  1
    fi
    echo   "Generate MANUAL.md success: '${SELFDIR}/MANUAL.md' -> '${PROJECT_ROOT}/MANUAL.md'"

    echo    "All documents generated successfully"
    return  0
}

main "$@"
exit "$?"
