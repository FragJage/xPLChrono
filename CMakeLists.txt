cmake_minimum_required (VERSION 2.8)

#Activer le support c++11 
if(CMAKE_VERSION VERSION_LESS "3.1")
  set(CMAKE_CXX_FLAGS "-std=c++11 ${CMAKE_CXX_FLAGS}")
else ()
  set(CMAKE_CXX_STANDARD 11)
endif ()

#Déclaration du projet
project (xPLChrono)
 
#add_subdirectory(src)
add_subdirectory(test)