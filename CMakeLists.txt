cmake_minimum_required (VERSION 3.6)

project (log_main)

set (log_VERSION_MAJOR 0)
set (log_VERSION_MINOR 1)

# Force C++11
set (CMAKE_CXX_STANDARD 11)
set (CMAKE_CXX_STANDARD_REQUIRED ON)
set (CMAKE_CXX_EXTENSIONS OFF)

set (PROJECT_SOURCE_DIR ${PROJECT_SOURCE_DIR}/src-cmake)

configure_file (
	"${PROJECT_SOURCE_DIR}/logConfig.h.in"
	"${PROJECT_BINARY_DIR}/logConfig.h"
	)

include_directories("${PROJECT_BINARY_DIR}")

include_directories ("${PROJECT_BINARY_DIR}/log")
add_subdirectory ("${PROJECT_SOURCE_DIR}/log")
set (EXTRA_LIBS ${EXTRA_LIBS} log)

add_executable (log_main ${PROJECT_SOURCE_DIR}/main.cpp)
target_link_libraries (log_main ${EXTRA_LIBS})

set_target_properties(log_main PROPERTIES
                CXX_STANDARD 11
                CXX_STANDARD_REQUIRED ON
                CXX_EXTENSIONS OFF)

install (TARGETS log_main DESTINATION build)
install (FILES "${PROJECT_BINARY_DIR}/logConfig.h"
		DESTINATION include)
