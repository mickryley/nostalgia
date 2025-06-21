cmake_minimum_required(VERSION 3.15)
project(Nostalgia)

set(CMAKE_CXX_COMPILER "/opt/homebrew/opt/llvm/bin/clang++")
#set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

message(STATUS "Detected platform: ${CMAKE_SYSTEM_NAME}")

set(APP_NAME Nostalgia)

# Set the source files
# Include all /src/*.cpp files
file(GLOB_RECURSE SRC_FILES CONFIGURE_DEPENDS ${CMAKE_SOURCE_DIR}/src/*.cpp)

if(WIN32)
elseif(APPLE)
endif()

# Add Executable
add_executable(Nostalgia 
    ${SRC_FILES}
)

# Target Include Directories
target_include_directories(Nostalgia PRIVATE 
    src
)

# Platform Specific Libraries
if(WIN32)
    target_link_libraries(Nostalgia PRIVATE 
        # Add Windows specific libraries here
    )
elseif(APPLE)
    target_link_libraries(Nostalgia PRIVATE 
        # Add macOS specific libraries here
    )
elseif(UNIX)
    target_link_libraries(Nostalgia PRIVATE 
        # Add Unix specific libraries here
    )
endif()
