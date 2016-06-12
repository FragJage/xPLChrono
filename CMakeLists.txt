cmake_minimum_required (VERSION 2.8)

#Activer le support c++11 
if(CMAKE_VERSION VERSION_LESS "3.1")
  set(CMAKE_CXX_FLAGS "-std=c++11 ${CMAKE_CXX_FLAGS}")
else ()
  set (CMAKE_CXX_STANDARD 11)
endif ()

message(STATUS "CMAKE_CXX_FLAGS : ${CMAKE_CXX_FLAGS}")
  
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

#Déclaration de l'exécutable
add_executable(
  xPLChrono
  ${source_files}
  ${dependency_files}
)