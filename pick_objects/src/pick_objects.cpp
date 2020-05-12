#include <ros/ros.h>
#include <move_base_msgs/MoveBaseAction.h>
#include <actionlib/client/simple_action_client.h>
#include <std_msgs/UInt8.h>
#include <tf/tf.h>


// Define a client for to send goal requests to the move_base server through a SimpleActionClient
typedef actionlib::SimpleActionClient<move_base_msgs::MoveBaseAction> MoveBaseClient;

int main(int argc, char** argv){
  // Initialize the simple_navigation_goals node
  ros::init(argc, argv, "pick_objects");
  ros::NodeHandle n;
  //tell the action client that we want to spin a thread by default
  MoveBaseClient ac("move_base", true);

  //set up publisher to broadcast if robot is at pick up location
  ros::Publisher location_pub = n.advertise<std_msgs::UInt8>("/target_reached", 1);

  // Wait 5 sec for move_base action server to come up
  while(!ac.waitForServer(ros::Duration(5.0))){
    ROS_INFO("Waiting for the move_base action server to come up");
  }

  move_base_msgs::MoveBaseGoal goal;

  // set up the frame parameters
  goal.target_pose.header.frame_id = "map";
  goal.target_pose.header.stamp = ros::Time::now();

  // Define a position and orientation for the robot to reach
  float goals[2][3] = { {-0.5, -7, 1.57}, {-2.1, -2.1, 1.57}  };

int num_stops = 2;

for (int i=0; i < num_stops; i++){

    goal.target_pose.pose.position.x = goals[i][0];
    goal.target_pose.pose.position.y = goals[i][1];
    goal.target_pose.pose.orientation = tf::createQuaternionMsgFromYaw(goals[i][2]);

     // Send the target goal position and orientation for the robot to reach
    ROS_INFO("Sending goal");
    ac.sendGoal(goal);

    // Wait for the results
    ac.waitForResult();

    // Check if the robot reached its goal
  if (ac.getState() == actionlib::SimpleClientGoalState::SUCCEEDED)
  {
      ROS_INFO("Goal reached!");
    //Notify the add_markers node that the goal has been reached
      std_msgs::UInt8 msg;
      msg.data = i+1;
      ROS_INFO("The message is %d", msg.data);
      location_pub.publish(msg);
      //wait 5 seconds for pick up and dropoff
      sleep(5);}
      else{
      ROS_INFO("Failed to reach goal");
      ros::Duration(5.0).sleep();
  }
  }


  return 0;
}
