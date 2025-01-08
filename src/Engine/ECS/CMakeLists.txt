cmake_minimum_required(VERSION 3.10)
#libs
project(ECS)

file(GLOB HEADER_FILES "*.h")
file(GLOB SOURCE_FILES "*.cpp")


#file(GLOB SYSTEM_LIBRARIES "*.lib")
#message("global files:"${COMMON_FUNCTION_PATH})

add_library(${PROJECT_NAME} STATIC ${HEADER_FILES} ${SOURCE_FILES})

target_include_directories(${PROJECT_NAME} PUBLIC ${SYSTEMS_PATH} ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(${PROJECT_NAME} PUBLIC Common)

set_target_properties(${PROJECT_NAME} PROPERTIES FOLDER ${core_folder})

install(TARGETS ${PROJECT_NAME} DESTINATION lib)
