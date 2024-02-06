# Sets the minimum CMake version required for this project.
cmake_minimum_required(VERSION 3.22.1)
#Project name and configuration
project("nso_project" LANGUAGES CXX)
#Defininitions
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/src/main/jniLibs/${ANDROID_ABI})
set(LIEF_ROOT_DIR ${CMAKE_SOURCE_DIR}/src/main/jniLibs/${ANDROID_ABI}/LIEF-0.9.0-Android-x86_64/LIEF-0.9.0-Android/)
set(LIEF_LIB_PATH ${LIEF_ROOT_DIR}/lib/libLIEF.a)
#Import LIEF's library
add_library(LIEF_LIB STATIC IMPORTED)
set_target_properties(LIEF_LIB PROPERTIES IMPORTED_LOCATION ${LIEF_LIB_PATH})
#Create dynamicloader shared library
add_library(dynamicloader SHARED
        dynamicloader.cpp)
set_property(TARGET dynamicloader
        PROPERTY CXX_STANDARD           17
        PROPERTY CXX_STANDARD_REQUIRED  ON)
#Include LIEF headers
include_directories(${LIEF_ROOT_DIR}/include/)
#Link libraries to dynamicloader file
target_link_libraries(dynamicloader PUBLIC
        LIEF_LIB
        android
        log)


#for the libhello.ss compilition
#add_library(hello SHARED
#        hello.cpp)
#target_link_libraries(hello
#        android
#        log)


