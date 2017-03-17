cmake_minimum_required(VERSION 2.8 FATAL_ERROR)

find_package(gazebo REQUIRED)
include_directories(${GAZEBO_INCLUDE_DIRS})
link_directories(${GAZEBO_LIBRARY_DIRS})
list(APPEND CMAKE_CXX_FLAGS "${GAZEBO_CXX_FLAGS}")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${GAZEBO_CXX_FLAGS}")

# Model factory.
add_library(factory_plugin SHARED factory_plugin.cc)
target_link_libraries(factory_plugin ${GAZEBO_LIBRARIES})

# HeisenPerson
add_library(heisen_person_plugin SHARED heisen_person_plugin.cc)
target_link_libraries(heisen_person_plugin ${GAZEBO_LIBRARIES})

# HeisenCamera
add_library(heisen_camera_plugin SHARED heisen_camera_plugin.cc)
target_link_libraries(heisen_camera_plugin ${GAZEBO_LIBRARIES})

# CameraPlugin
add_library(take_photo_plugin SHARED take_photo_plugin.cc)
target_link_libraries(take_photo_plugin ${GAZEBO_LIBRARIES})
