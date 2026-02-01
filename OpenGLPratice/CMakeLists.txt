cmake_minimum_required(VERSION 3.13)

set(PROJECT_NAME cmake_project_example)
set(CMAKE_CXX_STANDARD 17)

project(${PROJECT_NAME})

if(MSVC)
    add_compile_options(/utf-8)
endif()

add_executable(${PROJECT_NAME} src/main.cpp)

include(Dependency.cmake)

target_include_directories(${PROJECT_NAME} PUBLIC ${DEP_INCLUDE_DIR})
target_link_directories(${PROJECT_NAME} PUBLIC ${DEP_LIB_DIR})
target_link_libraries(${PROJECT_NAME} PUBLIC ${DEP_LIBS})

add_dependencies(${PROJECT_NAME} ${DEP_LIST})