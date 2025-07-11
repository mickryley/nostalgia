cmake_minimum_required(VERSION 3.20)
project(Nostalgia)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON) # VS Code Compat
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

find_package(Vulkan REQUIRED)

message(STATUS "VULKAN_SDK = $ENV{VULKAN_SDK}")
message(STATUS "Detected platform: ${CMAKE_SYSTEM_NAME}")
message(STATUS "CMake version: ${CMAKE_VERSION}")

# === Fetch Content Dependencies ===
include(FetchContent)

FetchContent_Declare(
  json
  GIT_REPOSITORY https://github.com/nlohmann/json.git
  GIT_TAG v3.11.2
)

FetchContent_Declare(
  imgui
  GIT_REPOSITORY https://github.com/ocornut/imgui.git
  GIT_TAG docking
)

FetchContent_Declare(
  implot
  GIT_REPOSITORY https://github.com/epezent/implot.git
  GIT_TAG master
)

FetchContent_MakeAvailable(json imgui implot)

# === Platform-Specific SDL Paths ===
if(WIN32)
    set(SDL3_DIR "${CMAKE_SOURCE_DIR}/libs/SDL3/windows")
elseif(APPLE)
    set(SDL3_DIR "${CMAKE_SOURCE_DIR}/libs/SDL3/mac")
else()
    # Todo: Add support for Linux and other platforms
    #set(SDL3_DIR "${CMAKE_SOURCE_DIR}/libs/SDL3/linux")
    message(FATAL_ERROR "Unsupported platform: ${CMAKE_SYSTEM_NAME}")
endif()

# === Source Files ===
file(GLOB_RECURSE SRC_FILES CONFIGURE_DEPENDS "${CMAKE_SOURCE_DIR}/src/*.cpp")
list(FILTER SRC_FILES EXCLUDE REGEX ".*/legacy/.*")
list(FILTER SRC_FILES EXCLUDE REGEX "[/\\]legacy[/\\]")


# === ImGui + ImPlot Source Files ===
set(IMGUI_SRC
    ${imgui_SOURCE_DIR}/imgui.cpp
    ${imgui_SOURCE_DIR}/imgui_draw.cpp
    ${imgui_SOURCE_DIR}/imgui_widgets.cpp
    ${imgui_SOURCE_DIR}/imgui_tables.cpp
    ${imgui_SOURCE_DIR}/imgui_demo.cpp
    ${imgui_SOURCE_DIR}/backends/imgui_impl_sdl3.cpp
    ${imgui_SOURCE_DIR}/backends/imgui_impl_vulkan.cpp
)
set(IMPLOT_SRC
    ${implot_SOURCE_DIR}/implot.cpp
    ${implot_SOURCE_DIR}/implot_items.cpp
    ${implot_SOURCE_DIR}/implot_demo.cpp
)

# === Source-Scoped Strict Compiler Flags ===
add_library(project_obj OBJECT ${SRC_FILES})
target_compile_options(project_obj PRIVATE
    $<$<CXX_COMPILER_ID:GNU,Clang>:
        -Wall -Wextra -Wpedantic -Werror
        -Wshadow -Wconversion -Wsign-conversion
        -Wnull-dereference -Wdouble-promotion -Wformat=2 -Wcast-align
    >
    $<$<CXX_COMPILER_ID:MSVC>:
        /W4 /WX /permissive-
    >
)
target_include_directories(project_obj
    PRIVATE
        "${CMAKE_SOURCE_DIR}/src"
    SYSTEM
        "${Vulkan_INCLUDE_DIRS}"
        "${SDL3_DIR}/include"
        ${imgui_SOURCE_DIR}
        ${imgui_SOURCE_DIR}/backends
        ${implot_SOURCE_DIR}
        ${json_SOURCE_DIR}/include  # ← Add this!
)
# === Include ImGui and ImPlot without Strict Flags ===
add_library(imgui_obj OBJECT ${IMGUI_SRC})
target_include_directories(imgui_obj
    PRIVATE
        ${imgui_SOURCE_DIR}
        ${imgui_SOURCE_DIR}/backends
        # All dependencies needed by imgui sources, e.g. SDL, Vulkan
        "${SDL3_DIR}/include"
        "${Vulkan_INCLUDE_DIRS}"
)
target_compile_options(imgui_obj PRIVATE -w)
add_library(implot_obj OBJECT ${IMPLOT_SRC})
target_include_directories(implot_obj
    PRIVATE
        ${implot_SOURCE_DIR}
        ${imgui_SOURCE_DIR}         # so ImPlot can see ImGui headers!
        ${imgui_SOURCE_DIR}/backends
        "${SDL3_DIR}/include"
        "${Vulkan_INCLUDE_DIRS}"
)
target_compile_options(implot_obj PRIVATE -w)

# === Lower Strictness for ImGui and ImPlot inclusion ===
set_source_files_properties(src/benchmarking/visualiser/benchmark_visualiser.cpp
    PROPERTIES
    COMPILE_OPTIONS "-Wno-implicit-int-float-conversion"
)

# === Main Executable ===
add_executable(${PROJECT_NAME}
    $<TARGET_OBJECTS:project_obj>
    $<TARGET_OBJECTS:imgui_obj>
    $<TARGET_OBJECTS:implot_obj>
)

# === Include Paths ===
target_include_directories(${PROJECT_NAME} PRIVATE
    "${CMAKE_SOURCE_DIR}/src"
)

# === Debug Flag Support ===
target_compile_definitions(${PROJECT_NAME} PRIVATE
    $<$<CONFIG:Debug>:BUILD_DEBUG>
    $<$<CONFIG:RelWithDebInfo>:BUILD_DEV>
    $<$<CONFIG:Release>:BUILD_RELEASE>
)

# === Windows-Specific Compiler Flags ===
if(MSVC)
    target_compile_options(${PROJECT_NAME} PRIVATE /std:c++latest /Zc:__cplusplus)
endif()

# === Link Libraries ===
target_link_libraries(${PROJECT_NAME} PRIVATE
    Vulkan::Vulkan
    nlohmann_json
)

# === Platform-Specific SDL Linking ===
if(WIN32)
    target_link_libraries(${PROJECT_NAME} PRIVATE "${SDL3_DIR}/SDL3.lib")
    add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
        "${SDL3_DIR}/SDL3.dll"
        "$<TARGET_FILE_DIR:${PROJECT_NAME}>/SDL3.dll"
    )
elseif(APPLE)
    target_link_libraries(${PROJECT_NAME} PRIVATE "${SDL3_DIR}/libSDL3.dylib")
else()
    # Add Unix specific SDL linkage if needed
endif()

# === Copy Assets After Build ===
set(ASSETS_SOURCE_DIR "${CMAKE_SOURCE_DIR}/assets")
set(ASSETS_TARGET_DIR "$<TARGET_FILE_DIR:${PROJECT_NAME}>/assets")
add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E make_directory "${ASSETS_TARGET_DIR}"
    COMMAND ${CMAKE_COMMAND} -E copy_directory "${ASSETS_SOURCE_DIR}" "${ASSETS_TARGET_DIR}"
)