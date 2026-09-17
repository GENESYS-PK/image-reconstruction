cmake_minimum_required(VERSION 3.25)
project(image-reconstruction LANGUAGES CXX C)
cmake_policy(SET CMP0169 OLD)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED YES)
set(CMAKE_CXX_EXTENSIONS NO)

include(FetchContent)
find_package(blend2d QUIET CONFIG)

if(blend2d_FOUND)
	message(STATUS "${PROJECT_NAME}: Library 'blend2d' is installed, no need to download.")
else()
	message(STATUS "${PROJECT_NAME}: Library 'blend2d' is not installed, downloading it with 'asmjit' dependency...")
	FetchContent_Declare(
		asmjit
		GIT_REPOSITORY https://github.com/asmjit/asmjit
		GIT_TAG master
		GIT_SHALLOW ON
	)

	FetchContent_Declare(
		blend2d
		GIT_REPOSITORY https://github.com/blend2d/blend2d
		GIT_TAG master
		GIT_SHALLOW ON
	)

	FetchContent_Populate(asmjit)
	set(ASMJIT_DIR "${asmjit_SOURCE_DIR}")
	FetchContent_MakeAvailable(blend2d)
endif()

find_package(SDL3 QUIET)
if(SDL3_FOUND)
	message(STATUS "${PROJECT_NAME}: Library 'SDL3' is installed, no need to download.")
else()
	message(STATUS "${PROJECT_NAME}: Library 'SDL3' is not installed, downloading...")
	FetchContent_Declare(
		SDL3
		GIT_REPOSITORY https://github.com/libsdl-org/SDL
		GIT_TAG release-3.4.16
		GIT_SHALLOW ON
	)

	FetchContent_MakeAvailable(SDL3)
endif()

FetchContent_Declare(
	imgui
	GIT_REPOSITORY https://github.com/ocornut/imgui.git
	GIT_TAG v1.92.9b
	GIT_SHALLOW ON
)

message(STATUS "${PROJECT_NAME}: Downloading 'imgui' library...")
FetchContent_MakeAvailable(imgui)

add_library(
	imgui
	"${imgui_SOURCE_DIR}/imgui.cpp"
	"${imgui_SOURCE_DIR}/imgui_demo.cpp"
	"${imgui_SOURCE_DIR}/imgui_draw.cpp"
	"${imgui_SOURCE_DIR}/imgui_tables.cpp"
	"${imgui_SOURCE_DIR}/imgui_widgets.cpp"
	"${imgui_SOURCE_DIR}/backends/imgui_impl_sdl3.cpp"
	"${imgui_SOURCE_DIR}/backends/imgui_impl_sdlgpu3.cpp"
	"${imgui_SOURCE_DIR}/backends/imgui_impl_sdlrenderer3.cpp"
)

target_include_directories(
	imgui
	PUBLIC
	"${imgui_SOURCE_DIR}"
	"${imgui_SOURCE_DIR}/backends"
)

target_link_libraries(imgui PUBLIC SDL3::SDL3)

file(GLOB_RECURSE SRC_FILES CONFIGURE_DEPENDS "src/*.*")
add_executable(${PROJECT_NAME} ${SRC_FILES})
target_link_libraries(${PROJECT_NAME} PRIVATE blend2d::blend2d SDL3::SDL3 imgui)
target_precompile_headers(${PROJECT_NAME} PRIVATE "${PROJECT_SOURCE_DIR}/src/pch.hpp")
