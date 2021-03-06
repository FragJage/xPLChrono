#Activer le support c++11 
if(CMAKE_VERSION VERSION_LESS "3.1")
  set(CMAKE_CXX_FLAGS "-std=c++11 ${CMAKE_CXX_FLAGS}")
else ()
  set(CMAKE_CXX_STANDARD 11)
endif ()

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -W -Wshadow -Wunused-variable -Wunused-parameter -Wunused-function -Wunused -Wno-system-headers -Wno-deprecated -Woverloaded-virtual") # various warning flags
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -s -O2") # no debug, active optimisation

include_directories(../dependency)
 
#Génération de la liste des fichiers sources
file(
  GLOB_RECURSE
  Plateforms_files
  ../dependency/Plateforms/*
)

file(
  GLOB_RECURSE
  SafeFunctions_files
  ../dependency/SafeFunctions/*
)

file(
  GLOB_RECURSE
  Service_files
  ../dependency/Service/*
)

file(
  GLOB_RECURSE
  SimpleFolders_files
  ../dependency/SimpleFolders/*
)

file(
  GLOB_RECURSE
  SimpleIni_files
  ../dependency/SimpleIni/*
)

file(
  GLOB_RECURSE
  SimpleLog_files
  ../dependency/SimpleLog/*
)

file(
  GLOB_RECURSE
  SimpleSock_files
  ../dependency/SimpleSock/*
)

file(
  GLOB_RECURSE
  xPLLib_files
  ../dependency/xPLLib/*
)

#Déclaration de l'exécutable
set(EXECUTABLE_OUTPUT_PATH bin/${CMAKE_BUILD_TYPE})
add_executable(
  xPLChrono
  ${Plateforms_files}
  ${SafeFunctions_files}
  ${Service_files}
  ${SimpleFolders_files}
  ${SimpleIni_files}
  ${SimpleLog_files}
  ${SimpleSock_files}
  ${xPLLib_files}
  xPLChrono.cpp
  Counter.cpp
  main.cpp
)

#Spécification WIN32
if(WIN32)
  target_link_libraries(xPLChrono ws2_32)
endif()