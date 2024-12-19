cmake_minimum_required(VERSION 3.15)
project(CustomBuilds VERSION 0.1.0 LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 20)
set(LIB_DIR "${CMAKE_CURRENT_SOURCE_DIR}/deps")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/bin")

# Add debug flags
if(CMAKE_BUILD_TYPE MATCHES Debug)
    set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -g")
    set(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -g")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wno-long-long -pedantic")
endif()

include_directories(${CMAKE_SOURCE_DIR}/include)
file(GLOB_RECURSE ROOT_SRC CONFIGURE_DEPENDS "${CMAKE_SOURCE_DIR}/src/*.cpp")

if(NOT DEFINED PROGRAMS)
    message(FATAL_ERROR "Please specify PROGRAMS to build using -DPROGRAMS=<name1;name2>, or use -DPROGRAMS=all.")
endif()

file(GLOB PROGRAM_DIRS RELATIVE ${CMAKE_SOURCE_DIR}/programs ${CMAKE_SOURCE_DIR}/programs/*)

# Convert list of valid programs to a semicolon-separated string
list(REMOVE_ITEM PROGRAM_DIRS CMakeLists.txt) # Remove CMakeLists.txt if present
string(REPLACE ";" ", " VALID_PROGRAMS "${PROGRAM_DIRS}")

# Handle "all" case
if("all" IN_LIST PROGRAMS)
    message(STATUS "Building all programs...")
    set(SELECTED_PROGRAMS ${PROGRAM_DIRS})
else()
    # Validate the user-specified program names
    foreach(PROGRAM ${PROGRAMS})
        if(NOT PROGRAM IN_LIST PROGRAM_DIRS)
            message(FATAL_ERROR "Invalid program name '${PROGRAM}'. Valid options are: ${VALID_PROGRAMS}, or 'all'.")
        endif()
        list(APPEND SELECTED_PROGRAMS ${PROGRAM})
    endforeach()
endif()

# External Libraries Setup
set(GLFW_DIR "${LIB_DIR}/glfw")
set(GLAD_DIR "${LIB_DIR}/glad")
set(IMGUI_DIR "${LIB_DIR}/imgui")
set(FMT_DIR "${LIB_DIR}/fmt")

# Threads
set(THREADS_PREFER_PTHREAD_FLAG ON)
find_package(Threads REQUIRED)

# GLFW
add_subdirectory("${GLFW_DIR}")

# Glad
add_library(glad "${GLAD_DIR}/src/glad.c")
target_include_directories(glad PRIVATE "${GLAD_DIR}/include")

# ImGUI
add_library("imgui" "${IMGUI_DIR}/imgui.cpp"
                    "${IMGUI_DIR}/imgui_demo.cpp"
                    "${IMGUI_DIR}/imgui_draw.cpp"
                    "${IMGUI_DIR}/imgui_tables.cpp"
                    "${IMGUI_DIR}/imgui_widgets.cpp"
                    "${IMGUI_DIR}/backends/imgui_impl_glfw.cpp"
                    "${IMGUI_DIR}/backends/imgui_impl_opengl3.cpp")
target_include_directories("imgui" PRIVATE "${GLFW_DIR}/include")
target_include_directories("imgui" PRIVATE "${IMGUI_DIR}")
target_include_directories("imgui" PRIVATE "${IMGUI_DIR}/backends")

# FMT
add_subdirectory("${FMT_DIR}")

# Build selected programs
foreach(PROG ${SELECTED_PROGRAMS})
    message(STATUS "Building program '${PROG}'...")

    # Add sources specific to the program and root sources
    file(GLOB PROGRAM_SOURCES "${CMAKE_SOURCE_DIR}/programs/${PROG}/*.cpp")
    add_executable(${PROG} ${PROGRAM_SOURCES} ${ROOT_SRC})

    # Root directory constant
    target_compile_definitions(${PROG} PUBLIC ROOT_DIR="${CMAKE_CURRENT_SOURCE_DIR}/programs/${PROG}")

    # Include directories for the program
    target_include_directories(${PROG} PRIVATE "${CMAKE_SOURCE_DIR}/include/")

    # Link external libraries

    # Threads
    target_link_libraries(${PROG} Threads::Threads)

    # GLFW
    target_link_libraries(${PROG} glfw "${GLFW_LIBRARIES}")
    target_include_directories(${PROG} PRIVATE "${GLFW_DIR}/include")
    target_compile_definitions(${PROG} PRIVATE GLFW_INCLUDE_NONE)

    # Glad
    target_include_directories(${PROG} PRIVATE "${GLAD_DIR}/include")
    target_link_libraries(${PROG} glad ${CMAKE_DL_LIBS})

    # ImGUI
    target_include_directories(${PROG} PRIVATE "${IMGUI_DIR}")
    target_include_directories(${PROG} PRIVATE "${IMGUI_DIR}/backends")
    target_link_libraries(${PROG} "imgui" "${CMAKE_DL_LIBS}")

    # FMT
    target_link_libraries(${PROG} fmt::fmt)
    target_include_directories(${PROG} PRIVATE "${FMT_DIR}/include")
endforeach()

# Libraries that I might use later
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

# # NLOHMANN JSON
# set(JSON_DIR "${LIB_DIR}/json")
# add_subdirectory("${JSON_DIR}")
# target_link_libraries(${CMAKE_PROJECT_NAME} nlohmann_json::nlohmann_json)
# target_include_directories(${CMAKE_PROJECT_NAME} PRIVATE "${JSON_DIR}/include")