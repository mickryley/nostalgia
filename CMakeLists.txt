cmake_minimum_required(VERSION 3.20)

project(Nostalgia)

#set(CMAKE_CXX_COMPILER "/opt/homebrew/opt/llvm/bin/clang++")

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

find_package(Vulkan REQUIRED)

message(STATUS "VULKAN_SDK = $ENV{VULKAN_SDK}")
message(STATUS "Detected platform: ${CMAKE_SYSTEM_NAME}")
message (STATUS "CMake version: ${CMAKE_VERSION}")

# === Paths ===
set(APP_NAME Nostalgia)
set(SRC_DIR "${CMAKE_SOURCE_DIR}/src")
set(ImGui_Dir "${CMAKE_SOURCE_DIR}/libs/imgui") 
set(JSON_Dir "${CMAKE_SOURCE_DIR}/libs/json")

# === Platform-Specific Paths ===
if(WIN32)
    set(SDL3_DIR "${CMAKE_SOURCE_DIR}/libs/SDL3/windows")
elseif(APPLE)
    set(SDL3_DIR "${CMAKE_SOURCE_DIR}/libs/SDL3/mac") 
else()
    message(FATAL_ERROR "Unsupported platform: ${CMAKE_SYSTEM_NAME}")
endif()

# === Source Files ===
file(GLOB_RECURSE SRC_FILES CONFIGURE_DEPENDS ${CMAKE_SOURCE_DIR}/src/*.cpp)

# ImGui Sources
set(ImGui_Source_Files
    "${ImGui_Dir}/imgui.cpp"
    "${ImGui_Dir}/imgui_draw.cpp"
    "${ImGui_Dir}/imgui_widgets.cpp"
    "${ImGui_Dir}/imgui_tables.cpp"
    "${ImGui_Dir}/imgui_demo.cpp"

    "${ImGui_Dir}/implot.cpp"
    "${ImGui_Dir}/implot_items.cpp"
    "${ImGui_Dir}/implot_demo.cpp"

    "${ImGui_Dir}/backends/imgui_impl_sdl3.cpp"
    "${ImGui_Dir}/backends/imgui_impl_vulkan.cpp"
)

# === Executable ===
add_executable(
    ${APP_NAME}
    ${SRC_FILES}
    ${ImGui_Source_Files}
)

# === VS Flags ===
if (MSVC)
    target_compile_options(${APP_NAME} PRIVATE /std:c++latest /Zc:__cplusplus)
endif()

# === Debug Flag Support ===
target_compile_definitions(${APP_NAME} PRIVATE
    $<$<CONFIG:Debug>:BUILD_DEBUG>
    $<$<CONFIG:RelWithDebInfo>:BUILD_DEV>
    $<$<CONFIG:Release>:BUILD_RELEASE>
)

# === Include Directories ===
target_include_directories(${APP_NAME} PRIVATE 
    "${CMAKE_SOURCE_DIR}/src"
    "${Vulkan_INCLUDE_DIRS}"
    "${ImGui_Dir}"
    "${ImGui_Dir}/backends"
    "${SDL3_DIR}/include"
    "${JSON_Dir}"
)

# === Platform Specific Libraries ===
if(WIN32)
    target_link_libraries(Nostalgia PRIVATE 
        "${SDL3_DIR}/SDL3.lib"
        "${Vulkan_LIBRARIES}"
    )
    add_custom_command(TARGET ${APP_NAME} POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
            "${SDL3_DIR}/SDL3.dll"
            "$<TARGET_FILE_DIR:${APP_NAME}>/SDL3.dll"
    )
elseif(APPLE)
    target_link_libraries(Nostalgia PRIVATE 
    "${Vulkan_LIBRARIES}"
    "${SDL3_DIR}/libSDL3.dylib"
    )
elseif(UNIX)
    target_link_libraries(Nostalgia PRIVATE 
        # Add Unix specific libraries here
    )
endif()

# === Copy Assets ===
set(ASSETS_SOURCE_DIR "${CMAKE_SOURCE_DIR}/assets")
set(ASSETS_TARGET_DIR "$<TARGET_FILE_DIR:${APP_NAME}>/assets")
add_custom_command(TARGET ${APP_NAME} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E make_directory "$<TARGET_FILE_DIR:${APP_NAME}>/assets"
    COMMAND ${CMAKE_COMMAND} -E copy_directory "${ASSETS_SOURCE_DIR}" "${ASSETS_TARGET_DIR}"
)