project (log)

# Boost
set (BOOST_ROOT "$ENV{BOOSTROOT}")

set (Boost_NO_SYSTEMS_PATHS ON)
set (Boost_USE_MULTITHREADED ON)
set (Boost_USE_STATIC_LIBS ON)
set (Boost_USE_STATIC_RUNTIME OFF)
set (Boost_ALL_DYN_LINK OFF)

#add_definitions(-DBOOST_LOG_DYN_LINK)

find_package(Boost 1.61.0 REQUIRED COMPONENTS system thread log)

if (Boost_FOUND)

    include_directories(${Boost_INCLUDE_DIR})

    find_package(Threads REQUIRED)

    if (WIN32 AND __COMPILER_GNU)
        add_definitions(-DBOOST_THREAD_USE_LIB)
    endif (WIN32 AND __COMPILER_GNU)

    set (LOG_DEPENDENCIES ${LOG_DEPENDENCIES} ${CMAKE_THREAD_LIBS_INIT})
    set (LOG_DEPENDENCIES ${LOG_DEPENDENCIES} ${Boost_LIBRARIES})
endif (Boost_FOUND)

set (LOG_SOURCES ${LOG_SOURCES} log.cpp)

add_library(log STATIC ${LOG_SOURCES})
target_link_libraries(log ${LOG_DEPENDENCIES})

install (TARGETS log DESTINATION bin)
install (FILES log.h DESTINATION include)
