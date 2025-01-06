# Minimum CMake version required
cmake_minimum_required (VERSION 3.8)

# Enable Hot Reload for MSVC compilers if supported.
if (POLICY CMP0141)
  cmake_policy(SET CMP0141 NEW)
  set(CMAKE_MSVC_DEBUG_INFORMATION_FORMAT "$<IF:$<AND:$<C_COMPILER_ID:MSVC>,$<CXX_COMPILER_ID:MSVC>>,$<$<CONFIG:Debug,RelWithDebInfo>:EditAndContinue>,$<$<CONFIG:Debug,RelWithDebInfo>:ProgramDatabase>>")
endif()

# Project name
project ("Console Chess")

# Specify C++ standard
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Collect all source files and headers dynamically
file(GLOB_RECURSE SOURCES "${CMAKE_SOURCE_DIR}/Board/*.cpp")
file(GLOB_RECURSE HEADERS "${CMAKE_SOURCE_DIR}/Board/*.h")

# Add the main source file and the collected files to the target
add_executable(CMakeTarget "main.cpp" ${SOURCES} ${HEADERS})

# Include the Board directory and its subdirectories
target_include_directories(CMakeTarget PRIVATE ${CMAKE_SOURCE_DIR}/Board)

# Optional: Add debug-specific or release-specific flags here
