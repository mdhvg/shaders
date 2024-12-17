cmake_minimum_required(VERSION 3.7)
project(gltest VERSION 0.1.0 LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 20)
set(LIB_DIR "${CMAKE_CURRENT_SOURCE_DIR}/deps")
# set output directories
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/bin")

# Add debug flags
if(CMAKE_BUILD_TYPE MATCHES Debug)
    set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -g")
    set(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -g")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wno-long-long -pedantic")
endif()

file(GLOB_RECURSE LIB_SOURCES CONFIGURE_DEPENDS "${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp")
add_executable(${CMAKE_PROJECT_NAME} ${LIB_SOURCES})

if(CMAKE_BUILD_TYPE MATCHES Debug)
    target_compile_definitions(${CMAKE_PROJECT_NAME} PUBLIC DEBUG=1)
endif()

target_compile_definitions(${CMAKE_PROJECT_NAME} PUBLIC ROOT_DIR="${CMAKE_CURRENT_SOURCE_DIR}")
target_include_directories(${CMAKE_PROJECT_NAME} PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/include/")

# Multi-threading
set(THREADS_PREFER_PTHREAD_FLAG ON)
find_package(Threads REQUIRED)
target_link_libraries(${CMAKE_PROJECT_NAME} Threads::Threads)

# GLFW
set(GLFW_DIR "${LIB_DIR}/glfw")

set(GLFW_BUILD_EXAMPLES OFF CACHE INTERNAL "Build the GLFW example programs")
set(GLFW_BUILD_TESTS OFF CACHE INTERNAL "Build the GLFW test programs")
set(GLFW_BUILD_DOCS OFF CACHE INTERNAL "Build the GLFW documentation")
set(GLFW_INSTALL OFF CACHE INTERNAL "Generate installation target")

add_subdirectory("${GLFW_DIR}")

target_link_libraries(${CMAKE_PROJECT_NAME} glfw "${GLFW_LIBRARIES}")
target_include_directories(${CMAKE_PROJECT_NAME} PRIVATE "${GLFW_DIR}/include")
target_compile_definitions(${CMAKE_PROJECT_NAME} PRIVATE GLFW_INCLUDE_NONE)

# Glad
set(GLAD_DIR "${LIB_DIR}/glad")

add_library(glad "${GLAD_DIR}/src/glad.c")

target_include_directories(glad PRIVATE "${GLAD_DIR}/include")
target_include_directories(${CMAKE_PROJECT_NAME} PRIVATE "${GLAD_DIR}/include")
target_link_libraries(${CMAKE_PROJECT_NAME} glad ${CMAKE_DL_LIBS})

# GLM
# set(GLM_DIR "${LIB_DIR}/glm")
# add_subdirectory(${GLM_DIR})
# target_link_libraries(${CMAKE_PROJECT_NAME} glm)

# STB
# set(STB_DIR "${LIB_DIR}/stb")
# add_library(stb "${STB_DIR}/src/stb_image.cpp")
# target_include_directories(stb PRIVATE "${STB_DIR}/include")
# target_include_directories(${CMAKE_PROJECT_NAME} PRIVATE "${STB_DIR}/include")
# target_link_libraries(${CMAKE_PROJECT_NAME} stb ${CMAKE_DL_LIBS})

# Freetype
# set(FREETYPE_DIR "${LIB_DIR}/freetype")
# add_subdirectory("${FREETYPE_DIR}")
# target_link_libraries(${CMAKE_PROJECT_NAME} ${FREETYPE_LIBRARIES})
# target_include_directories(${CMAKE_PROJECT_NAME} PRIVATE "${FREETYPE_DIR}/include")

# ImGUI
set(IMGUI_DIR "${LIB_DIR}/imgui")

add_library("imgui" "${IMGUI_DIR}/imgui.cpp"
                  "${IMGUI_DIR}/imgui_demo.cpp"
                  "${IMGUI_DIR}/imgui_draw.cpp"
                  "${IMGUI_DIR}/imgui_tables.cpp"
                  "${IMGUI_DIR}/imgui_widgets.cpp"
                  "${IMGUI_DIR}/backends/imgui_impl_glfw.cpp"
                  "${IMGUI_DIR}/backends/imgui_impl_opengl3.cpp")

target_include_directories("imgui" PRIVATE "${IMGUI_DIR}")
target_include_directories("imgui" PRIVATE "${IMGUI_DIR}/backends")
target_include_directories(${PROJECT_NAME} PRIVATE "${IMGUI_DIR}")
target_include_directories(${PROJECT_NAME} PRIVATE "${IMGUI_DIR}/backends")
target_link_libraries(${PROJECT_NAME} "imgui" "${CMAKE_DL_LIBS}")

# # NLOHMANN JSON
# set(JSON_DIR "${LIB_DIR}/json")

# add_subdirectory("${JSON_DIR}")

# target_link_libraries(${CMAKE_PROJECT_NAME} nlohmann_json::nlohmann_json)
# target_include_directories(${CMAKE_PROJECT_NAME} PRIVATE "${JSON_DIR}/include")

# FMT
set(FMT_DIR "${LIB_DIR}/fmt")

add_subdirectory("${FMT_DIR}")

target_link_libraries(${CMAKE_PROJECT_NAME} fmt::fmt)
target_include_directories(${CMAKE_PROJECT_NAME} PRIVATE "${FMT_DIR}/include")