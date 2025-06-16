import os
import subprocess
from pathlib import Path

# This Python script is used to initialize the CMake build system for a C++ Project
# It sets up the build directory, generates the CMake files, and builds the project.

project_dir = Path.cwd()
project_name = project_dir.name
cmake_file = project_dir / "CMakeLists.txt"
src_dir = project_dir / "src"
main_cpp = src_dir / "main.cpp"

# Create src directory if it doesn't exist
src_dir.mkdir(parents=True, exist_ok=True)

import textwrap

# Create a dummy main.cpp file if it doesn't exist
if not main_cpp.exists():
    print("Creating dummy main.cpp")
    main_cpp.write_text(textwrap.dedent("""\
        #include <iostream>

        int main() {
            std::cout << "Hello, world!\\n";
            return 0;
        }
    """))

if not cmake_file.exists():
    print("Creating CMakeLists.txt file")
    cmake_file.write_text(textwrap.dedent(f"""\
        cmake_minimum_required(VERSION 3.15)
        project({project_name})

        set(CMAKE_CXX_STANDARD 23)
        set(CMAKE_CXX_STANDARD_REQUIRED ON)
        set(CMAKE_CXX_EXTENSIONS OFF)

        message(STATUS "Detected platform: ${{CMAKE_SYSTEM_NAME}}")

        set(APP_NAME {project_name})
        
        # Set the source files
        # Include all /src/*.cpp files
        file(GLOB_RECURSE SRC_FILES CONFIGURE_DEPENDS ${{CMAKE_SOURCE_DIR}}/src/*.cpp)

        if(WIN32)
        elseif(APPLE)
        endif()
        
        # Add Executable
        add_executable({project_name} 
            ${{SRC_FILES}}
        )

        # Target Include Directories
        target_include_directories({project_name} PRIVATE 
            src
        )

        # Platform Specific Libraries
        if(WIN32)
            target_link_libraries({project_name} PRIVATE 
                # Add Windows specific libraries here
            )
        elseif(APPLE)
            target_link_libraries({project_name} PRIVATE 
                # Add macOS specific libraries here
            )
        elseif(UNIX)
            target_link_libraries({project_name} PRIVATE 
                # Add Unix specific libraries here
            )
        endif()
    """))


else:
	print("CMakeLists.txt already exists. Skipping creation.")

#Run cmake generation setup
print("Running CMake configuation") 
result = subprocess.run([
	"cmake",
	"-G", "Visual Studio 17 2022", 
	"-A", "x64",
	"-S", str(project_dir),
	"-B", str(project_dir / "build")
], shell = True)

if result.returncode != 0:
	print("[ERROR] Build failed.")
	exit(1)

print("[SUCCESS] Setup complete.")

