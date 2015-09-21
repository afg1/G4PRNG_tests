
cmake_minimum_required(VERSION 2.8)
find_package(Geant4 REQUIRED vis_all)

set(CXX11 "-std=c++11")
include(${Geant4_USE_FILE})
include_directories("include" ${Geant4_INCLUDE_DIRS})
add_definitions(${Geant4_DEFINITIONS} ${CXX11})
#set(CMAKE_CXX_FLAGS ${Geant4_CXX_FLAGS})

FILE(GLOB sources src/*.cc)
FILE(GLOB headers include/*.hh)
add_executable(G4PRNG_tests G4PRNG_tests.cc ${sources} ${headers})
target_link_libraries(G4PRNG_tests ${Geant4_LIBRARIES})
