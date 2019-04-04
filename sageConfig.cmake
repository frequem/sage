include("${CMAKE_CURRENT_LIST_DIR}/sageTargets.cmake")

include(CMakeFindDependencyMacro)

find_package(SDL2 CONFIG REQUIRED)
find_package(SDL_ttf CONFIG REQUIRED)
if(NOT ANDROID)
	find_package(glew CONFIG REQUIRED)
endif()
find_package(glm CONFIG REQUIRED)
find_package(soil CONFIG REQUIRED)
