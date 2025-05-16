set(third_party_folder "ThirdParty")

set(BUILD_SHARED_LIBS OFF CACHE BOOL "" FORCE)

if(NOT TARGET imgui)
    include(imgui.cmake)
    set_target_properties(imgui PROPERTIES FOLDER ${third_party_folder}/imgui)
endif()

if(NOT TARGET glm)
    option(BUILD_STATIC_LIBS "" ON)
    option(BUILD_TESTING "" OFF)
    option(GLM_TEST_ENABLE "" OFF)
    add_subdirectory(glm)
    set_target_properties(glm PROPERTIES FOLDER ${third_party_folder}/glm)
endif()

if(NOT TARGET glfw)
    option(GLFW_BUILD_EXAMPLES "" OFF)
    option(GLFW_BUILD_TESTS "" OFF)
    option(GLFW_BUILD_DOCS "" OFF)
    option(GLFW_INSTALL "" OFF)
    add_subdirectory(glfw)
    set_target_properties(glfw PROPERTIES FOLDER ${third_party_folder}/glfw)
    set_target_properties(update_mappings PROPERTIES FOLDER ${third_party_folder}/glfw)
endif()

#if(NOT TARGET yaml-cpp)
#	set(YAML_CPP_BUILD_TESTS OFF CACHE BOOL "Disable yaml-cpp tests" FORCE)
#	set(YAML_CPP_BUILD_TOOLS OFF CACHE BOOL "Disable yaml-cpp tools" FORCE)
#	set(YAML_CPP_BUILD_SHARED_LIBS OFF CACHE BOOL "Build only static yaml-cpp library" FORCE)
#
#	add_subdirectory(yaml-cpp)
#	set_target_properties(yaml-cpp PROPERTIES FOLDER ${third_party_folder}/yaml-cpp)
#endif()

if(NOT TARGET jsoncpp_static)
	set(JSONCPP_WITH_TESTS OFF CACHE BOOL "Disable JsonCpp tests" FORCE)
	set(JSONCPP_WITH_POST_BUILD_UNITTEST OFF CACHE BOOL "Disable JsonCpp post-build unit tests" FORCE)

	add_subdirectory(jsoncpp)
	set_target_properties(jsoncpp_static PROPERTIES FOLDER ${third_party_folder}/jsoncpp)
endif()

#if(NOT TARGET stb)
#    include(stb.cmake)
#    set_target_properties(stb PROPERTIES FOLDER ${third_party_folder}/stb)
#endif()
#
#if(NOT TARGET assimp)
#    add_subdirectory(assimp)
#    set_target_properties(assimp PROPERTIES FOLDER ${third_party_folder}/assimp)
#endif()
