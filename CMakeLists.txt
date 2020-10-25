cmake_minimum_required(VERSION 3.9)

project(Graph_Project)

# Showing the system type
message(STATUS "We are on a ${CMAKE_SYSTEM_NAME} system")

# setting the version of the project
set(${PROJECT_NAME}_MAJOR_VERSION 0)
set(${PROJECT_NAME}_MINOR_VERSION 1)
set(${PROJECT_NAME}_VERSION "${${PROJECT_NAME}_MAJOR_VERSION}.${${PROJECT_NAME}_MINOR_VERSION}")

# Output the version of the project
message("${PROJECT_NAME} version: ${${PROJECT_NAME}_VERSION}")

set(CMAKE_MODULE_PATH
  ${CMAKE_SOURCE_DIR}/cmake/Modules
)
include_directories(application/includes)
# set output dirictories dedicated for libs and binaries
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

set(${PROJECT_NAME}_SOURCES 
                "application/sources/FileLoader.cxx"
                "application/sources/Graph.cxx"
                "application/sources/WordSimilarity.cxx"
                "application/sources/Common.cxx"
                "application/sources/Solver.cxx"
            )
set(${PROJECT_NAME}_INCLUDES 
                "application/includes/FileLoader.h"
                "application/includes/Graph.h"
                "application/includes/Globals.h"
                "application/includes/Common.hpp"
                "application/includes/WordSimilarity.h"
                "application/includes/Solver.h"
            )
set(${PROJECT_NAME}_MAIN "application/main.cxx")

file(GLOB_RECURSE GRAPH_FILES ${CMAKE_SOURCE_DIR}/graphs/*.grph)

source_group("Graph Files" FILES ${GRAPH_FILES})
source_group("" FILES ${${PROJECT_NAME}_MAIN})

include(safeguard)
include(setflags)

add_executable(${PROJECT_NAME} 
                    ${${PROJECT_NAME}_MAIN} 
                    ${${PROJECT_NAME}_SOURCES}
                    ${${PROJECT_NAME}_INCLUDES}
                    ${GRAPH_FILES}
                )

macro(copy_files srcDir destDir)
    message(STATUS "Configuring directory ${destDir}")
    make_directory(${destDir})

    file(GLOB templateFiles RELATIVE ${srcDir} ${srcDir}/*)
    foreach(templateFile ${templateFiles})
        set(srcTemplatePath ${srcDir}/${templateFile})
        if(NOT IS_DIRECTORY ${srcTemplatePath})
            message(STATUS "Configuring file ${templateFile}")
            configure_file(
                    ${srcTemplatePath}
                    ${destDir}/${templateFile}
                    COPYONLY)
        endif(NOT IS_DIRECTORY ${srcTemplatePath})
    endforeach(templateFile)
endmacro(copy_files srcDir destDir)
copy_files(${CMAKE_SOURCE_DIR}/graphs ${CMAKE_BINARY_DIR}/graphs)

if(MSVC)
    if(${CMAKE_VERSION} VERSION_LESS "3.6.0") 
        message("\n\t[ WARNING ]\n\n\tCMake version lower than 3.6.\n\n\t - Please update CMake and rerun; OR\n\t - Manually set ${PROJECT_NAME} as StartUp Project in Visual Studio.\n" )
    else()
        set_property(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT ${PROJECT_NAME})
    endif()
endif()