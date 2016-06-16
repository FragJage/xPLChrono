cmake_minimum_required (VERSION 2.8)

#Activer le support c++11 
if(CMAKE_VERSION VERSION_LESS "3.1")
  set(CMAKE_CXX_FLAGS "-std=c++11 ${CMAKE_CXX_FLAGS}")
else ()
  set (CMAKE_CXX_STANDARD 11)
endif ()

message(STATUS "CMAKE_CXX_FLAGS : ${CMAKE_CXX_FLAGS}")

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -W -Wshadow -Wunused-variable -Wunused-parameter -Wunused-function -Wunused -Wno-system-headers -Wno-deprecated -Woverloaded-virtual") # various warning flags
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g -O0") # debug, no optimisation
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} --coverage") # enabling coverage
  
#Déclaration du projet
project(xPLChrono)
set(EXECUTABLE_OUTPUT_PATH bin/${CMAKE_BUILD_TYPE})

include_directories(dependency)
 
#Génération de la liste des fichiers sources
file(
  GLOB_RECURSE
  source_files
  src/*
)
file(
  GLOB_RECURSE
  dependency_files
  dependency/*
)
file(
  GLOB_RECURSE
  test_files
  tests/*
)

#Déclaration des exécutables
add_executable(
  xPLChrono
  ${source_files}
  ${dependency_files}
)
add_executable(
  unittest
  ${test_files}
  ${dependency_files}
)

if(WIN32)
  target_link_libraries(xPLChrono ws2_32)
  target_link_libraries(unittest ws2_32)
endif()