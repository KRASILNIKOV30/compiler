#!/usr/bin/env bash

set -o errexit
set -o nounset
set -o pipefail
IFS=$'\n\t'

readonly SCRIPT_DIR=$(dirname "$(readlink -f "$0")")
readonly PROJECT_DIR=$(dirname "$SCRIPT_DIR")

readonly RELEASE_DIR_NAME="cmake-build-release"
readonly DEBUG_DIR_NAME="cmake-build-debug"
readonly COMPILER_EXECUTABLE="compiler"
readonly VM_EXECUTABLE="PVM"
readonly GRAMMAR_FILE="grammar.txt"
readonly BYTE_CODE_FILE="byte-code.prmbc"

readonly RED='\033[0;31m'
readonly GREEN='\033[0;32m'
readonly YELLOW='\033[1;33m'
readonly NC='\033[0m'

error() { echo -e "${RED}[ERROR]${NC} $*" >&2; }
success() { echo -e "${GREEN}[SUCCESS]${NC} $*"; }
info() { echo -e "${YELLOW}[INFO]${NC} $*"; }

usage() {
    echo "Usage: $0 [action] [--debug] [-h|--help] [input_file] [compiler_args...]"
    echo ""
    echo "A script to build, run, and clean the C++ project."
    echo ""
    echo "Actions (optional):"
    echo "  (no action)           Builds and then runs the project (default behavior)."
    echo "  --build-only          Only configures and builds the project."
    echo "  --run-only            Only runs the already built project."
    echo "  clean                 Removes build directories."
    echo ""
    echo "Options:"
    echo "  --debug               Use the Debug configuration. Default is Release."
    echo "  -h, --help            Show this help message."
    echo ""
    echo "Arguments:"
    echo "  input_file            Optional path to the source code file for the compiler."
    echo "  compiler_args...      Additional arguments to pass to the compiler."
}

run_cmake_configure() {
    local build_dir=$1
    local build_type=$2

    if ! command -v cmake &> /dev/null; then
        error "CMake is not installed. Please install it to build the project."
        exit 1
    fi

    info "Configuring project in '${build_dir}' with build type '${build_type}'..."
    if ! cmake -S "${PROJECT_DIR}" -B "${build_dir}" -DCMAKE_BUILD_TYPE="${build_type}"; then
        error "CMake configuration failed."
        exit 1
    fi
    success "CMake configuration successful."
}

run_cmake_build() {
    local build_dir=$1
    info "Building project in '${build_dir}'..."
    if ! cmake --build "${build_dir}" --parallel; then
        error "Project build failed."
        exit 1
    fi
    success "Project built successfully."
}

execute_program() {
    local build_dir=$1
    shift
    local program_args=("$@")

    local compiler_path="${build_dir}/${COMPILER_EXECUTABLE}"
    local vm_path="${build_dir}/${VM_EXECUTABLE}"

    if [[ ! -x "${compiler_path}" || ! -x "${vm_path}" ]]; then
        error "Executable files not found in '${build_dir}'. Did you build the project?"
        return 1
    fi

    info "Running compiler..."
    local input_file=""
    local compiler_args=()
    if [[ ${#program_args[@]} -gt 0 && -f "${program_args[0]}" ]]; then
        input_file="${program_args[0]}"
        compiler_args=("${program_args[@]:1}")
    else
        compiler_args=("${program_args[@]}")
    fi

    local args_to_pass=("${PROJECT_DIR}/${GRAMMAR_FILE}")
    [[ -n "${input_file}" ]] && args_to_pass+=("${input_file}")
    [[ ${#compiler_args[@]} -gt 0 ]] && args_to_pass+=("${compiler_args[@]}")

    if ! "${compiler_path}" "${args_to_pass[@]}"; then
        error "Compiler failed. Aborting."
        return 1
    fi
    success "Compiler finished."

    local byte_code_path="${build_dir}/${BYTE_CODE_FILE}"
    mv -f "${PROJECT_DIR}/${BYTE_CODE_FILE}" "${byte_code_path}"

    if ! "${vm_path}" "${byte_code_path}"; then
        error "Virtual Machine execution failed."
        return 1
    fi
}

run_clean() {
    local build_mode=$1

    info "Cleaning build directories..."
    if [[ "${build_mode}" == "debug" || "${build_mode}" == "all" ]]; then
        info "Removing ${PROJECT_DIR}/${DEBUG_DIR_NAME}"
        rm -rf "${PROJECT_DIR}/${DEBUG_DIR_NAME}"
    fi
    if [[ "${build_mode}" == "release" || "${build_mode}" == "all" ]]; then
        info "Removing ${PROJECT_DIR}/${RELEASE_DIR_NAME}"
        rm -rf "${PROJECT_DIR}/${RELEASE_DIR_NAME}"
    fi
    success "Cleanup complete."
}

main() {
    local build_mode="release"
    local do_build=true
    local do_run=true
    local do_clean=false
    local program_args=()

    while [[ $# -gt 0 ]]; do
        case "$1" in
            --debug)       build_mode="debug"; shift ;;
            --build-only)  do_run=false; shift ;;
            --run-only)    do_build=false; shift ;;
            clean)         do_clean=true; shift ;;
            -h|--help)     usage; exit 0 ;;
            -*)            error "Unknown option: $1"; usage; exit 1 ;;
            *)             program_args+=("$1"); shift ;;
        esac
    done

    if [[ "${do_clean}" == true ]]; then
        local clean_mode="all"
        [[ "${#program_args[@]}" -eq 0 && "${build_mode}" != "release" ]] && clean_mode="debug"
        run_clean "${clean_mode}"
        exit 0
    fi

    local build_dir_name=$([[ "$build_mode" == "debug" ]] && echo "$DEBUG_DIR_NAME" || echo "$RELEASE_DIR_NAME")
    local build_dir="${PROJECT_DIR}/${build_dir_name}"
    local cmake_build_type=$([[ "$build_mode" == "debug" ]] && echo "Debug" || echo "Release")

    info "Selected mode: ${build_mode}"

    if [[ "${do_build}" == true ]]; then
        run_cmake_configure "${build_dir}" "${cmake_build_type}"
        run_cmake_build "${build_dir}"
    fi

    if [[ "${do_run}" == true ]]; then
        execute_program "${build_dir}" "${program_args[@]}"
    fi
}

main "$@"