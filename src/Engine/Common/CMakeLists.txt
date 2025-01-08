project(Common)

file(GLOB_RECURSE HEADER_FILES "*.h" "*.hpp")
file(GLOB_RECURSE SOURCE_FILES "*.cpp")

source_group(TREE "${CMAKE_CURRENT_SOURCE_DIR}" FILES ${HEADER_FILES} ${SOURCE_FILES})

add_library(${PROJECT_NAME} STATIC  ${HEADER_FILES} ${SOURCE_FILES})

target_include_directories(${PROJECT_NAME} PUBLIC ${Component_PATH} ${CMAKE_CURRENT_SOURCE_DIR})

target_link_libraries(${PROJECT_NAME})

target_compile_definitions(${PROJECT_NAME} PUBLIC DATA_DIR="${DATA_DIR}")

set_target_properties(${PROJECT_NAME} PROPERTIES FOLDER ${core_folder})

install(TARGETS ${PROJECT_NAME} 
	RUNTIME DESTINATION bin
	ARCHIVE DESTINATION lib
)