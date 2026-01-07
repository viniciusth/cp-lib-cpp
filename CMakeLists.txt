cmake_minimum_required(VERSION 3.20)
project(komodo LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

add_executable(komodo src/cli/main.cpp)

# We might add more sources later
target_sources(komodo PRIVATE
    src/cli/main.cpp
)

# Enable some warnings
if(MSVC)
    target_compile_options(komodo PRIVATE /W4)
else()
    target_compile_options(komodo PRIVATE -Wall -Wextra -pedantic)
endif()

# Include 'src' directory globally so specific solution files can find headers
include_directories(${CMAKE_SOURCE_DIR}/src)

# Add targets for all solution files automatically
file(GLOB SOLUTION_FILES "src/code/*.cpp")
foreach(SOL_FILE ${SOLUTION_FILES})
    get_filename_component(SOL_NAME ${SOL_FILE} NAME_WE)
    add_executable("solution_${SOL_NAME}" ${SOL_FILE})
endforeach()
