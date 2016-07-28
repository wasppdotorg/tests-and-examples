
cmake_minimum_required (VERSION 3.0)

set (TARGET sqlite3)
project (${TARGET})

aux_source_directory (. Source_DIRECTORIES)
add_executable (${TARGET} ${Source_DIRECTORIES})

include_directories (/usr/include /usr/local/include)

find_library (PThread_LIBRARIES NAMES pthread PATHS /usr/lib usr/local/lib)

if (CMAKE_CXX_COMPILER_ID MATCHES Clang)
	set (CMAKE_COMPILER_IS_CLANGCXX 1)
endif()

if (CMAKE_COMPILER_IS_GNUCXX OR CMAKE_COMPILER_IS_CLANGCXX)
	set (CMAKE_CXX_FLAGS "-std=c++11 -Wall -g -fexceptions -O2 -DNDEBUG")
endif()

target_link_libraries (${TARGET} ${CMAKE_DL_LIBS})
target_link_libraries (${TARGET} ${PThread_LIBRARIES})

