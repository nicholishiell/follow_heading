cmake_minimum_required(VERSION 2.8.3)
project(follow_heading)

find_package(catkin REQUIRED COMPONENTS geometry_msgs std_msgs roscpp)

include_directories(${catkin_INCLUDE_DIRS})
set(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/bin)
set(LIBRARY_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/lib)

add_executable(
  follow_heading 
  src/follow_heading.cpp 
)	 

target_link_libraries(follow_heading ${catkin_LIBRARIES})

