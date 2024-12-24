project(RenderSystem)


# 是否启用验证层
option(ENABLE_VALIDATION_LAYERS "Enable Vulkan Validation Layers" ON)
if(ENABLE_VALIDATION_LAYERS)
    add_definitions(-DVK_ENABLE_VALIDATION_LAYERS)
	add_definitions(-DVK_EXT_debug_utils)
    message(STATUS "Vulkan Validation Layers enabled")
endif()

#target_compile_definitions(glfw PUBLIC GLFW_INCLUDE_VULKAN)
target_include_directories(glfw PUBLIC ${GLFW_INCLUDE_DIR} ${Vulkan_INCLUDE_DIR})

#message(${Vulkan_INCLUDE_DIR})
#message(${Vulkan_LIBRARY})
#message(${GLFW_INCLUDE_DIR})

find_package(Vulkan REQUIRED)
#find_package(glfw3 REQUIRED)


file(GLOB_RECURSE HEADER_FILES "*.h")
file(GLOB_RECURSE SOURCE_FILES "*.cpp")

source_group(TREE "${CMAKE_CURRENT_SOURCE_DIR}" FILES ${HEADER_FILES} ${SOURCE_FILES})

#add_executable(${PROJECT_NAME} main.cpp)
#add_executable(${PROJECT_NAME} ${HEADER_FILES} ${SOURCE_FILES})

#message("project is "${PROJECT_NAME})

add_library(${PROJECT_NAME} STATIC  ${HEADER_FILES} ${SOURCE_FILES})

target_include_directories(${PROJECT_NAME} PUBLIC ${Component_PATH} ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(${PROJECT_NAME} PUBLIC ECS Component)
target_link_libraries(${PROJECT_NAME} PRIVATE Vulkan::Vulkan glfw)

set_target_properties(${PROJECT_NAME} PROPERTIES FOLDER ${system_folder})

# shader路径
set(SHADER_PATH "${CMAKE_CURRENT_SOURCE_DIR}/shader/spv/")
target_compile_definitions(${PROJECT_NAME} PUBLIC SHADER_PATH="${SHADER_PATH}")

set(SYSTEMS_NAME "${SYSTEMS_NAME} ${PROJECT_NAME}")

install(TARGETS ${PROJECT_NAME} 
	RUNTIME DESTINATION bin
	ARCHIVE DESTINATION lib
)
#message("systems:" ${SYSTEMS_NAME})

