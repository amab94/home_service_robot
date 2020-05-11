#!/bin/sh

xterm -e " roslaunch turtlebot_gazebo turtlebot_world.launch world_file:=/home/amabz/catkin_uws/src/map/my_world2.world " &
sleep 5
xterm -e " roslaunch turtlebot_gazebo amcl_demo.launch map_file:=/home/amabz/catkin_uws/src/map/mymap.yaml " &
sleep 5
xterm -e " roslaunch turtlebot_rviz_launchers view_navigation.launch "
sleep 5
xterm -e "rosrun pick_objects pick_objects_node"
