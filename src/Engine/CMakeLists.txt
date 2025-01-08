project(Engine)

set(COMMON_FUNCTION_PATH ${CMAKE_CURRENT_SOURCE_DIR}/Common)
set(CMAKE_INSTALL_PREFIX "${CMAKE_CURRENT_SOURCE_DIR}/install")

file(GLOB_RECURSE COMMON_HEADERS_FILES "${COMMON_FUNCTION_PATH}/*.h" "${COMMON_FUNCTION_PATH}/*.hpp" "${COMMON_FUNCTION_PATH}/*.cpp")
#message(${COMMON_FUNCTION_PATH})
#message(STATUS "common:" ${COMMON_HEADERS_FILES})

set(ECS_PATH ${CMAKE_CURRENT_SOURCE_DIR}/ECS)
set(Component_PATH ${CMAKE_CURRENT_SOURCE_DIR}/Component)
set(SYSTEMS_PATH ${CMAKE_CURRENT_SOURCE_DIR}/System)
set(SYSTEMS_NAME "")

set(core_folder "Engine")

add_subdirectory(ECS)

add_subdirectory(System)
add_subdirectory(Component)

add_subdirectory(Core)