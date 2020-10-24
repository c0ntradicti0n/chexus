cmake_minimum_required(VERSION 3.16)
project(Nexus)

set(CMAKE_CXX_STANDARD 14)
set(THREADS_PREFER_PTHREAD_FLAG ON)
find_package(Threads REQUIRED)

find_package(OpenMP)
if (OPENMP_FOUND)
    set (CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${OpenMP_C_FLAGS}")
    set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${OpenMP_CXX_FLAGS}")
    set (CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} ${OpenMP_EXE_LINKER_FLAGS}")
endif()

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Ofast")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Ofast")

include(ExternalProject)
ExternalProject_Add(gtest
        URL https://github.com/google/googletest/archive/release-1.10.0.zip
        # Comment above line, and uncomment line below to use subversion.
        # SVN_REPOSITORY http://googletest.googlecode.com/svn/trunk/
        # Uncomment line below to freeze a revision (here the one for 1.7.0)
        # SVN_REVISION -r700

        PREFIX ${CMAKE_CURRENT_BINARY_DIR}/gtest
        INSTALL_COMMAND ""
        )
ExternalProject_Get_Property(gtest source_dir binary_dir)



include_directories(.)

ADD_LIBRARY(Basis STATIC intelligence.cpp
        spielfeld.cpp
        )
ADD_LIBRARY(IO STATIC ./common_functions.h
        ./io.cpp
        )

target_link_libraries(Basis IO)

add_executable(Nexus
        main.cpp
        common_functions.h
        )

target_link_libraries(Nexus IO Basis)


################
# Define a test
add_executable(Nexus_test nexus_test.cpp nexus_test.cpp nexus_test.h)

######################################
# Configure the test to use GoogleTest
#
# If used often, could be made a macro.

add_dependencies(Nexus_test gtest)
include_directories(${source_dir}/googletest/include)

get_cmake_property(_variableNames VARIABLES)
list (SORT _variableNames)
foreach (_variableName ${_variableNames})
    message(STATUS "${_variableName}=${${_variableName}}")
endforeach()

target_link_libraries(Nexus_test Threads::Threads ${binary_dir}/lib/libgtest.a  ${binary_dir}/lib/libgtest_main.a)

##################################
# Just make the test runnable with
#   $ make test

enable_testing()
add_test(NAME    Nexus_test
        COMMAND Nexus_test)