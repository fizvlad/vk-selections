# #############################################################################
#
# Project: vk-selections
# Author: fizvlad
#
# #############################################################################

cmake_minimum_required(VERSION 2.8.8 FATAL_ERROR)
project(vk-selections CXX)


# #############################################################################
# Settings
# #############################################################################

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -fPIC -fexceptions")
include_directories("include/")
set(SRC_LIST "src/fizvlad/cpp-formula/cpp-formula.cpp"
             "src/fizvlad/vk_api/vk_utility.cpp"
             "src/fizvlad/vk_api/vk_chat.cpp"
             "src/fizvlad/vk_api/vk_longpoll.cpp"
             "src/fizvlad/vk_selection/vk_selection.cpp"
             "src/selection_manager.cpp")

# #############################################################################
# Dependencies
# #############################################################################

set(CURL_SSL_ENABLED ON)
find_package(CURL REQUIRED)
include_directories("$CURL_INCLUDE_DIRS")


# #############################################################################
# Executable
# #############################################################################

add_executable(${PROJECT_NAME} "src/main.cpp;" ${SRC_LIST})
target_link_libraries(${PROJECT_NAME} ${CURL_LIBRARIES})
