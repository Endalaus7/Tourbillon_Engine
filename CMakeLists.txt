cmake_minimum_required(VERSION 3.10)
project(TourBillon)


set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>DLL")
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

set(CMAKE_INSTALL_PREFIX ${CMAKE_CURRENT_SOURCE_DIR}/install)
set(BIN_DIR ${CMAKE_CURRENT_SOURCE_DIR}/bin)
set(DATA_DIR ${CMAKE_CURRENT_SOURCE_DIR}/Data)

set(THIRD_PARTY_DIR ${CMAKE_CURRENT_SOURCE_DIR}/3rdParty)

set(EXECUTABLE_OUTPUT_PATH ${BIN_DIR}/${PROJECT_NAME})

#libs
set(GLFW_INCLUDE_DIR "${THIRD_PARTY_DIR}/glfw/include")
#set(Vulkan_LIBRARY "${THIRD_PARTY_DIR}/VulkanLibs/Lib/Win32/vulkan-1.lib") 
#set(Vulkan_INCLUDE_DIR "${THIRD_PARTY_DIR}/VulkanLibs/include")

set(Vulkan_LIBRARY "${THIRD_PARTY_DIR}/VulkanSDK/Lib/vulkan-1.lib")
set(Vulkan_INCLUDE_DIR "${THIRD_PARTY_DIR}/VulkanSDK/Include")

add_subdirectory(3rdParty)
add_subdirectory(src)

message("build finished.")