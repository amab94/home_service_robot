#!/bin/sh

# Build the catkin_ws
cd $(pwd)/../..; catkin_make

xterm -e "source devel/setup.bash; roslaunch turtlebot_gazebo turtlebot_world.launch world_file:=/home/amabz/catkin_uws/src/map/my_world2.world " &
sleep 5
xterm -e "source devel/setup.bash; roslaunch turtlebot_gazebo amcl_demo.launch map_file:=/home/amabz/catkin_uws/src/map/mymap.yaml " &
sleep 5
xterm -e "source devel/setup.bash; roslaunch turtlebot_rviz_launchers view_navigation.launch " &
sleep 5
xterm -e " source devel/setup.bash; rosrun add_markers add_markers "
