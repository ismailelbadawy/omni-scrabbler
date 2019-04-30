cmake_minimum_required(VERSION 3.0.0)
project(omni-scrabbler VERSION 0.1.0)
ADD_DEFINITIONS(-D_AFXDLL)
SET(CMAKE_MFC_FLAG 2)
include(CTest)
enable_testing()
file(GLOB omniscrabbler 
${PROJECT_SOURCE_DIR}/omni-scrabbler-Communication-Testing/Test-Scrabble/*.h
${PROJECT_SOURCE_DIR}/omni-scrabbler-Communication-Testing/Test-Scrabble/*.cpp)
add_executable(
    omniscrabbler
    ${omniscrabbler}
)

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)