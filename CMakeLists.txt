cmake_minimum_required(VERSION 3.21)

# standards & flags
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_INTERPROCEDURAL_OPTIMIZATION ON)
set(CMAKE_INTERPROCEDURAL_OPTIMIZATION_DEBUG OFF)
set_property(GLOBAL PROPERTY USE_FOLDERS ON)
set(CMAKE_TOOLCHAIN_FILE "$ENV{VCPKG_ROOT}/scripts/buildsystems/vcpkg.cmake" CACHE FILEPATH "")
set(SDL2_DIR ${CMAKE_CURRENT_SOURCE_DIR}/extern/SDL2/cmake)

include(GNUInstallDirs)

# info
project(
    BG3WASD
    VERSION 1.9.5
    LANGUAGES CXX
)

# out-of-source builds only
if(${PROJECT_SOURCE_DIR} STREQUAL ${PROJECT_BINARY_DIR})
    message(FATAL_ERROR "In-source builds are not allowed.")
endif()

# in-place configuration
configure_file(
    ${CMAKE_CURRENT_SOURCE_DIR}/cmake/Plugin.h.in
    ${CMAKE_CURRENT_BINARY_DIR}/include/Plugin.h
    @ONLY
)

configure_file(
    ${CMAKE_CURRENT_SOURCE_DIR}/cmake/version.rc.in
    ${CMAKE_CURRENT_BINARY_DIR}/version.rc
    @ONLY
)

# source files
execute_process(COMMAND powershell -ExecutionPolicy Bypass -File "${CMAKE_CURRENT_SOURCE_DIR}/!Update.ps1" "SOURCEGEN" "${PROJECT_VERSION}" "${CMAKE_CURRENT_BINARY_DIR}")
include(${CMAKE_CURRENT_BINARY_DIR}/sourcelist.cmake)
source_group(
    TREE ${CMAKE_CURRENT_SOURCE_DIR}
    FILES ${SOURCES}
)

source_group(
    TREE ${CMAKE_CURRENT_BINARY_DIR}
    FILES ${CMAKE_CURRENT_BINARY_DIR}/include/Plugin.h
)

# dependency macros
macro(find_dependency_path DEPENDENCY FILE)
    # searches extern for dependencies and if not checks the environment variable
    if(NOT ${DEPENDENCY} STREQUAL "")
        # Check extern
        message(
            STATUS
            "Searching for ${DEPENDENCY} using file ${FILE}"
        )
        find_path("${DEPENDENCY}Path"
            ${FILE}
            PATHS "extern/${DEPENDENCY}")

        if("${${DEPENDENCY}Path}" STREQUAL "${DEPENDENCY}Path-NOTFOUND")
            # Check path
            message(
                STATUS
                "Getting environment for ${DEPENDENCY}Path: $ENV{${DEPENDENCY}Path}"
            )
            set("${DEPENDENCY}Path" "$ENV{${DEPENDENCY}Path}")
        endif()

        message(
            STATUS
            "Found ${DEPENDENCY} in ${${DEPENDENCY}Path}; adding"
        )
        add_subdirectory("${${DEPENDENCY}Path}" ${DEPENDENCY})
    endif()
endmacro()

# dependencies
find_package(spdlog CONFIG REQUIRED)
find_package(nlohmann_json REQUIRED)
find_package(SDL2 REQUIRED)
find_dependency_path(DKUtil include/DKUtil/Logger.hpp)

# cmake target
# runtime
add_library(
    ${PROJECT_NAME}
    SHARED
    ${SOURCES}
    ${CMAKE_CURRENT_BINARY_DIR}/include/Plugin.h
    ${CMAKE_CURRENT_BINARY_DIR}/version.rc
    .clang-format
    vcpkg.json
)

# include dir
target_include_directories(
    ${PROJECT_NAME}
    PRIVATE
    ${CMAKE_CURRENT_BINARY_DIR}/include
    ${CMAKE_CURRENT_SOURCE_DIR}/include
    ${CMAKE_CURRENT_SOURCE_DIR}/src
    ${SDL2_INCLUDE_DIRS}
)

# linkage
target_link_libraries(
    ${PROJECT_NAME}
    PRIVATE
    DKUtil::DKUtil
    spdlog::spdlog
    nlohmann_json::nlohmann_json
    SDL2::SDL2
)

# compiler def
if(MSVC)
    add_compile_definitions(_UNICODE)

    target_compile_options(
        ${PROJECT_NAME}
        PRIVATE
        /MP
        /await
        /W0
        /WX
        /permissive-
        /Zc:alignedNew
        /Zc:auto
        /Zc:__cplusplus
        /Zc:externC
        /Zc:externConstexpr
        /Zc:forScope
        /Zc:hiddenFriend
        /Zc:implicitNoexcept
        /Zc:lambda
        /Zc:noexceptTypes
        /Zc:preprocessor
        /Zc:referenceBinding
        /Zc:rvalueCast
        /Zc:sizedDealloc
        /Zc:strictStrings
        /Zc:ternary
        /Zc:threadSafeInit
        /Zc:trigraphs
        /Zc:wchar_t
        /wd4200 # nonstandard extension used : zero-sized array in struct/union
        /FI${CMAKE_BINARY_DIR}/CMakeFiles/${PROJECT_NAME}.dir/$<CONFIG>/cmake_pch.hxx
    )
endif()

# PCH
target_precompile_headers(
    ${PROJECT_NAME}
    PRIVATE
    src/PCH.hpp
)

set_property(
    TARGET
    ${PROJECT_NAME}
    PROPERTY VS_USER_PROPS
    "${CMAKE_CURRENT_SOURCE_DIR}/cmake/build_stl_modules.props")

# post build event
add_custom_command(
    TARGET
    ${PROJECT_NAME}
    POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy
    ${CMAKE_CURRENT_BINARY_DIR}/$<CONFIG>/${PROJECT_NAME}.dll
    "$ENV{BG3PATH}/bin/NativeMods/${PROJECT_NAME}.dll"
    COMMAND ${CMAKE_COMMAND} -E copy
    ${CMAKE_CURRENT_BINARY_DIR}/$<CONFIG>/${PROJECT_NAME}.dll
    "$ENV{BG3PATH2}/bin/NativeMods/${PROJECT_NAME}.dll"
)
