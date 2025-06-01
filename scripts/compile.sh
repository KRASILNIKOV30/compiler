#!/usr/bin/env bash

set -o errexit
set -o nounset
set -o pipefail
IFS=$'\n\t'

readonly CALLER_DIR=$(pwd)
readonly PARENT_DIR=$(dirname "$(readlink -f "$0")")
readonly PROJECT_DIR=$(dirname "$PARENT_DIR")
readonly BUILD_DIRS=("cmake-build-release" "cmake-build-debug")
readonly COMPILER_EXECUTABLE="compiler"
readonly VM_EXECUTABLE="PVM"
readonly GRAMMAR_FILE="grammar.txt"
readonly BYTE_CODE_FILE="byte-code.prmbc"

readonly RED='\033[0;31m'
readonly GREEN='\033[0;32m'
readonly YELLOW='\033[1;33m'
readonly NC='\033[0m'

error() {
    echo -e "${RED}[ERROR]${NC} $*" >&2
}

success() {
    echo -e "${GREEN}[SUCCESS]${NC} $*"
}

warning() {
    echo -e "${YELLOW}[WARNING]${NC} $*"
}

run_compile() {
    local dir=$1
    local executable_path="${dir}/${COMPILER_EXECUTABLE}"
    local input_file="${2:-}"

    if [[ -d "${dir}" ]]; then
        if [[ -f "${executable_path}" && -x "${executable_path}" ]]; then
            success "Found compiler in ${dir}, running..."

            local args=("${PROJECT_DIR}/${GRAMMAR_FILE}")
            if [[ -n "${input_file}" && -f "${input_file}" ]]; then
                args+=("${input_file}")
            fi
            if [[ $# -gt 2 ]]; then
                args+=("${@:3}")
            fi

            if ! "${executable_path}" "${args[@]}"; then
                error "Compile ${executable_path} failed with exit code $?"
                return 1
            fi

            success "Compile finished successfully"
            return 0
        else
            warning "Directory ${dir} exists but no executable found or it's not executable"
            return 1
        fi
    else
        warning "Directory ${dir} does not exist"
        return 1
    fi
}

run_code() {
    local dir=$1
    local executable_path="${dir}/${VM_EXECUTABLE}"
    local byte_code="${dir}/${BYTE_CODE_FILE}"

    if [[ -f "${executable_path}" && -x "${executable_path}" ]]; then
        if ! "${executable_path}" "${byte_code}"; then
            error "Run code ${executable_path} failed with exit code $?"
            return 1
        fi
    else
        error "Failed to find virtual machine: ${executable_path}"
        return 1
    fi
}

main() {
    if [[ $# -gt 0 && ($1 == "-h" || $1 == "--help") ]]; then
        local dirs_list=$(IFS=', '; echo "${missing_dirs[*]}")
        echo "Usage: $0 [input_file] [executable_args...]"
        echo "Runs compile in ${dirs_list} directories"
        echo "If input_file is provided and exists, it will be passed as first argument"
        echo "All additional arguments are passed to the executable"
        exit 0
    fi

    local found=0
    local input_file=""

    if [[ $# -gt 0 && -f "$1" ]]; then
        input_file="$1"
        shift
    fi

    for dir in "${BUILD_DIRS[@]}"; do
        local build_dir="${PROJECT_DIR}/${dir}";
        if run_compile "${build_dir}" "${input_file}" "$@"; then
            success "Compile finished"

            mv -f "${CALLER_DIR}/${BYTE_CODE_FILE}" "${build_dir}/"

            run_code "${build_dir}"

            found=1
            break
        fi
    done

    if [[ ${found} -eq 0 ]]; then
        local dirs_list=$(IFS=', '; echo "${missing_dirs[*]}")
        error "No valid directory with compiler found. Tried: ${dirs_list}"
        exit 1
    fi
}

main "$@"
