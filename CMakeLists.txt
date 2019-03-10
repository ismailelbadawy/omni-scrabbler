cmake_minimum_required(VERSION 3.0.0)
project(omni-scrabbler VERSION 0.1.0)

include(CTest)
enable_testing()


file(GLOB omniscrabbler 
${PROJECT_SOURCE_DIR}/GADDAG/*.h
${PROJECT_SOURCE_DIR}/GADDAG/*.cpp
${PROJECT_SOURCE_DIR}/Models/*.h
${PROJECT_SOURCE_DIR}/Models/*.cpp
${PROJECT_SOURCE_DIR}/MoveGenerator/*.h
${PROJECT_SOURCE_DIR}/MoveGenerator/*.cpp

${PROJECT_SOURCE_DIR}/source.cpp)
add_executable(
    omniscrabbler
    ${omniscrabbler}
)
set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)
