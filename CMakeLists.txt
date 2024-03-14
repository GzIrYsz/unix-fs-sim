cmake_minimum_required(VERSION 3.27)
project(unix_fs_sim
        VERSION 0.1.0
        DESCRIPTION ""
        HOMEPAGE_URL "https://github.com/GzIrYsz/unix-fs-sim"
        LANGUAGES C)

set(CMAKE_C_STANDARD 17)

set(PROJECT_DOC_DIR ${PROJECT_SOURCE_DIR}/docs)

# Adds the directory containing the library
add_subdirectory(src)

# Adds the directory containing the examples
add_subdirectory(examples)

# Looks if doxygen is installed on the system
find_package(Doxygen)
if (DOXYGEN_FOUND)
    message("Doxygen is correctly installed.")

    set(DOXYGEN_OUTPUT_DIRECTORY ${PROJECT_DOC_DIR})
    doxygen_add_docs(doc ${PROJECT_SOURCE_DIR}/includes)
else (DOXYGEN_FOUND)
    message("Doxygen is not installed on this machine or is not in the PATH")
endif (DOXYGEN_FOUND)