cmake_minimum_required(VERSION 3.10)
project(Core)

file(GLOB_RECURSE HEADER_FILES "*.h")
file(GLOB_RECURSE SOURCE_FILES "*.cpp")

source_group("Common Files" FILES ${COMMON_HEADERS_FILES})
#add_library(${PROJECT_NAME} STATIC ${HEADER_FILES} ${SOURCE_FILES})
add_executable(${PROJECT_NAME} ${HEADER_FILES} ${SOURCE_FILES} ${COMMON_HEADERS_FILES})

#message("systems:" ${SYSTEMS_NAME})
target_link_libraries(${PROJECT_NAME} PUBLIC RenderSystem Component)

target_include_directories(${PROJECT_NAME} PUBLIC ${ECS_PATH} ${SYSTEMS_PATH} ${COMMON_FUNCTION_PATH} ${CMAKE_CURRENT_SOURCE_DIR} )

set_target_properties(${PROJECT_NAME} PROPERTIES FOLDER ${core_folder})

install(TARGETS ${PROJECT_NAME} DESTINATION bin)