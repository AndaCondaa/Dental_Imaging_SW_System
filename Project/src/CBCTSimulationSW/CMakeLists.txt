cmake_minimum_required(VERSION 3.17)

project(Qt_VTK_CT)

#======================= INCLUSION OF Qt =======================#
set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_PREFIX_PATH $ENV{QTDIR})

find_package(OpenGL)
find_package(Qt6Core REQUIRED)
find_package(Qt6Gui REQUIRED)
find_package(Qt6OpenGL REQUIRED)
find_package(Qt6Xml REQUIRED)
find_package(Qt6Widgets REQUIRED)

#======================= INCLUSION OF VTK ======================#
set(VTK_DIR $ENV{VTK_DIR})
find_package(VTK REQUIRED)
include(${VTK_USE_FILE})

#======================= INCLUSION OF Our Code ======================#
set(FORMS_DIR "${CMAKE_SOURCE_DIR}/forms")
set(INCLUDE_DIR "${CMAKE_SOURCE_DIR}/include")
set(SOURCE_DIR "${CMAKE_SOURCE_DIR}/src")
set(RESOURCES_DIR "${CMAKE_SOURCE_DIR}/resources")

include_directories(${FORMS_DIR})
include_directories(${INCLUDE_DIR})
include_directories(${SOURCE_DIR})
include_directories(${RESOURCES_DIR})

file(GLOB_RECURSE SOURCES
    "${FORMS_DIR}/*.ui"
    "${INCLUDE_DIR}/*.h"
    "${SOURCE_DIR}/*.cpp"
    "${RESOURCES_DIR}/*.qrc"
)

# Add the forms directory to the AUTOUIC search paths
set(CMAKE_AUTOUIC_SEARCH_PATHS ${CMAKE_AUTOUIC_SEARCH_PATHS} ${FORMS_DIR})

# Add the executable
if (WIN32) 
    add_executable(Qt_VTK_CT WIN32 ${SOURCES})
elseif(UNIX)
    add_executable(Qt_VTK_CT ${SOURCES})
endif()

# Add the target includes
target_include_directories(Qt_VTK_CT PRIVATE ${FORMS_DIR})
target_include_directories(Qt_VTK_CT PRIVATE ${INCLUDE_DIR})
target_include_directories(Qt_VTK_CT PRIVATE ${SOURCE_DIR})
target_include_directories(Qt_VTK_CT PRIVATE ${RESOURCES_DIR})

#===================== LINKING LIBRARIES =======================#
target_link_libraries( Qt_VTK_CT Qt6::OpenGL)
target_link_libraries( Qt_VTK_CT Qt6::Xml)
target_link_libraries( Qt_VTK_CT Qt6::Widgets)
target_link_libraries( Qt_VTK_CT ${QT_LIBRARIES} ${OPENGL_LIBRARIES})
target_link_libraries( Qt_VTK_CT ${VTK_LIBRARIES})
