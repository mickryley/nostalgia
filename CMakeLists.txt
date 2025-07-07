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

# === Executable ===
add_executable(${PROJECT_NAME}
    ${SRC_FILES}
    ${IMGUI_SRC}
    ${IMPLOT_SRC}
)

# === Include Paths ===
target_include_directories(${PROJECT_NAME} PRIVATE
    "${CMAKE_SOURCE_DIR}/src"
    "${Vulkan_INCLUDE_DIRS}"
    ${imgui_SOURCE_DIR}
    ${imgui_SOURCE_DIR}/backends
    ${implot_SOURCE_DIR}
    "${SDL3_DIR}/include"
)

# === Debug Flag Support ===
target_compile_definitions(${PROJECT_NAME} PRIVATE
    $<$<CONFIG:Debug>:BUILD_DEBUG>
    $<$<CONFIG:RelWithDebInfo>:BUILD_DEV>
    $<$<CONFIG:Release>:BUILD_RELEASE>
)

# === Compiler Strictness Flags ===
target_compile_options(${PROJECT_NAME} PRIVATE
    $<$<CXX_COMPILER_ID:GNU,Clang>:-Wall;-Wextra;-pedantic;-Werror>
    $<$<CXX_COMPILER_ID:MSVC>:/W4;/WX>
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
elseif(UNIX)
    # Add Unix specific SDL linkage if needed
endif()

# === Copy Assets After Build ===
set(ASSETS_SOURCE_DIR "${CMAKE_SOURCE_DIR}/assets")
set(ASSETS_TARGET_DIR "$<TARGET_FILE_DIR:${PROJECT_NAME}>/assets")
add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E make_directory "${ASSETS_TARGET_DIR}"
    COMMAND ${CMAKE_COMMAND} -E copy_directory "${ASSETS_SOURCE_DIR}" "${ASSETS_TARGET_DIR}"
)