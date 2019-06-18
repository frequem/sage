cmake_minimum_required(VERSION 3.2)

include("cmake/Hunter/HunterGate.cmake")

HunterGate(
    URL "https://github.com/ruslo/hunter/archive/v0.23.94.tar.gz"
    SHA1 "3f4c104df81f1c4a627f4b0ea9f3306de1b0b54b"
    LOCAL
)

project(sage C CXX)

hunter_add_package(SDL2)
hunter_add_package(SDL_mixer)
hunter_add_package(SDL_ttf)
if(NOT ANDROID)
	hunter_add_package(glew)
endif()
hunter_add_package(glm)
hunter_add_package(soil)

find_package(SDL2 CONFIG REQUIRED)
find_package(SDL_mixer CONFIG REQUIRED)
find_package(SDL_ttf CONFIG REQUIRED)
if(NOT ANDROID)
	find_package(glew CONFIG REQUIRED)
endif()
find_package(glm CONFIG REQUIRED)
find_package(soil CONFIG REQUIRED)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_CXX_FLAGS_DEBUG "-O3 -g")
set(CMAKE_CXX_FLAGS_RELEASE "-O3")

file(GLOB_RECURSE SOURCES "src/*.cpp")
file(GLOB_RECURSE HEADERS "include/${PROJECT_NAME}/*.*")
add_library(${PROJECT_NAME} ${SOURCES} ${HEADERS})
include_directories(${PROJECT_NAME} PUBLIC $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>)

if(CMAKE_BUILD_TYPE STREQUAL "Debug")
	target_compile_definitions(${PROJECT_NAME} PRIVATE DEBUG=1)
else()
	target_compile_definitions(${PROJECT_NAME} PRIVATE DEBUG=0)
endif()

target_link_libraries(${PROJECT_NAME} PUBLIC SDL2::SDL2main SDL2::SDL2)
target_link_libraries(${PROJECT_NAME} PUBLIC SDL_mixer::SDL_mixer)
target_link_libraries(${PROJECT_NAME} PUBLIC SDL_ttf::SDL_ttf)
if(NOT ANDROID)
	target_link_libraries(${PROJECT_NAME} PUBLIC glew::glew)
endif()
target_link_libraries(${PROJECT_NAME} PUBLIC glm)
target_link_libraries(${PROJECT_NAME} PUBLIC soil::soil)

include(GNUInstallDirs)

install(
	TARGETS ${PROJECT_NAME}
	EXPORT "${PROJECT_NAME}Targets"
    LIBRARY DESTINATION "${CMAKE_INSTALL_LIBDIR}"
    ARCHIVE DESTINATION "${CMAKE_INSTALL_LIBDIR}"
    RUNTIME DESTINATION "${CMAKE_INSTALL_BINDIR}"
    INCLUDES DESTINATION "${CMAKE_INSTALL_INCLUDEDIR}"
)

install(
	DIRECTORY "include/${PROJECT_NAME}"
	DESTINATION "${CMAKE_INSTALL_INCLUDEDIR}"
)

include(CMakePackageConfigHelpers)

write_basic_package_version_file("${CMAKE_BINARY_DIR}/${PROJECT_NAME}ConfigVersion.cmake"
  VERSION 1.0.4
  COMPATIBILITY AnyNewerVersion
)

install(
	FILES
		"${CMAKE_CURRENT_SOURCE_DIR}/${PROJECT_NAME}Config.cmake"
		"${CMAKE_BINARY_DIR}/${PROJECT_NAME}ConfigVersion.cmake"
	DESTINATION "${CMAKE_INSTALL_LIBDIR}/cmake/${PROJECT_NAME}"
)

install(
	EXPORT "${PROJECT_NAME}Targets"
	FILE "${PROJECT_NAME}Targets.cmake"
	NAMESPACE "${PROJECT_NAME}::"
	DESTINATION "${CMAKE_INSTALL_LIBDIR}/cmake/${PROJECT_NAME}"
)
