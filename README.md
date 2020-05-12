# Home Service Robot Project
In this Udacity Home Service Robot Project I have programmed a home robot that autonomously maps the environment using SLAM algorithms, then it navigate to pickup and drop off an object.

## Summary of Tasks
The following steps where taken in order to achieve the final outcome for this project.
- Design a simulated world using the Building Editor in Gazebo.
- Teleoperated the robot manually test the SLAM algorithms.
- Use the ROS navigation stack and manually commands your robot using the 2D Nav Goal arrow in rviz to move to 2 different desired positions and orientations.
- Write a pick_objects node that commands the robot to move to the desired pickup and drop off targets.
- Write an add_markers node that subscribes to the pick_objects node, to notify the robot when the object is picked up or dropped off to simulate a virtual object.

To get started create ROS workspace or importe the current repo to your src folders
```
$mkdir -p ~/catkin_ws/search
$cd ~/catkin_ws
$catkin_make
$source devel/setup.bash
```
Clone the required repositories to the ~/catkin_ws/src
```
$ cd ~/catkin_ws/src
$ git clone https://github.com/amab94/home_service_robot.git
```
## Required ROS packages
The repo should include all the required packages, just incase it doesnt here they are;
  1.  gmapping: With the gmapping_demo.launch file, you can easily perform SLAM and build a map of the environment with a robot equipped with laser range finder sensors or RGB-D cameras.
  2.  turtlebot_teleop: With the keyboard_teleop.launch file, you can manually control a robot using keyboard commands.
  3.  turtlebot_rviz_launchers: With the view_navigation.launch file, you can load a preconfigured rviz workspace. You’ll save a lot of time by launching this file, because it will automatically load the robot model, trajectories, and map for you.
  4.  turtlebot_gazebo: With the turtlebot_world.launch you can deploy a turtlebot in a gazebo environment by linking the world file to it.

To install the packages run the installation script.
```
$ cd scripts
$ ./installations.sh
$ cd ~/catkin_ws
$ catkin_make
$ source devel/setup.bash
```
To run the home service code, running the following scripts
```
$ cd ~/catkin_ws/src/scripts
$ ./home_service.sh
```
If everything is installed correctly you should see the windows below, with the robot nevigating to the pickup point to pick the virtual object and then dropping it off.

![](home_robot_in_action.gif)
