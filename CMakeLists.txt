cmake_minimum_required(VERSION 3.10.0)
project(ShootingRange VERSION 0.1.0)

include(CTest)
enable_testing()



file(GLOB SOURCES "src/*.cpp")
add_executable(ShootingRange  ${SOURCES}	external/GLAD/src/glad.c )

add_subdirectory(${CMAKE_SOURCE_DIR}/external/GLM)
add_subdirectory(${CMAKE_SOURCE_DIR}/external/GLFW)


add_subdirectory(${CMAKE_SOURCE_DIR}/external/freetype)


include_directories(${CMAKE_SOURCE_DIR}/external/GLM)
include_directories(${CMAKE_SOURCE_DIR}/external/GLAD/include)
include_directories(${CMAKE_SOURCE_DIR}/external/GLFW/include)
include_directories(${CMAKE_SOURCE_DIR}/external/STB)
include_directories(${CMAKE_SOURCE_DIR}/external/freetype/include)
include_directories(${CMAKE_SOURCE_DIR}/external/irrKlang/include)
 
target_link_libraries(ShootingRange glfw ${GLFW_LIBRARIES} )
target_link_libraries(ShootingRange freetype ${FREETYPE_LIBRARIES})

if(WIN32)
    target_link_libraries(ShootingRange ${CMAKE_SOURCE_DIR}/external/irrKlang/lib/irrKlang.lib)
endif()


set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)
