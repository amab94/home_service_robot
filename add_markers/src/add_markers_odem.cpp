#include <ros/ros.h>
#include <visualization_msgs/Marker.h>
#include <nav_msgs/Odometry.h>
#include <complex>


using namespace std;

//
float pickup[2] = {-0.5, -7};
float dropoff[2] = {-2.1,-2.1};

//Boolaon Flags
bool pickup_loc = false;
bool dropoff_loc  = false;
bool pickup_complete = false;
bool dropoff_complete = false;

void chatterCallback(const nav_msgs::Odometry::ConstPtr& msg)
{
   //ROS_INFO("Seq: [%d]", msg->header.seq);
   //ROS_INFO("Position-> x: [%f], y: [%f], z: [%f]", msg->pose.pose.position.x,msg->pose.pose.position.y, msg->pose.pose.position.z);
//  ROS_INFO(pickup[0] -msg->pose.pose.position.x);

//Pickup location check with Odometry
if (abs(pickup[0] -msg->pose.pose.position.x) <= 2.44 && abs(pickup[1] -msg->pose.pose.position.y) <= 2.24)
{
  if(!pickup_loc)
  {
    pickup_loc = true;
  }
}
else
{
  pickup_loc = false;
}

//Dropoff location check with Odometry
if (abs(dropoff[0] - msg->pose.pose.position.x) <= 2.35 && abs(dropoff[1] - msg->pose.pose.position.y) <= 2)
{
  if(!dropoff_loc)
  {
    dropoff_loc=true;
  }
}
else
{
  dropoff_loc = false;
}

}

int main( int argc, char** argv )
{
  ros::init(argc, argv, "add_markers");
  ros::NodeHandle n;
  ros::Rate r(1);
  ros::Publisher marker_pub = n.advertise<visualization_msgs::Marker>("visualization_marker", 1);
  ros::Subscriber odom_sub = n.subscribe("odom", 1000, chatterCallback);

  // Set our initial shape type to be a cube
  uint32_t shape = visualization_msgs::Marker::CUBE;

  while (ros::ok())
  {
    visualization_msgs::Marker marker;
    // Set the frame ID and timestamp.  See the TF tutorials for information on these.
    marker.header.frame_id = "map";
    marker.header.stamp = ros::Time::now();

    // Set the namespace and id for this marker.  This serves to create a unique ID
    // Any marker sent with the same namespace and id will overwrite the old one
    marker.ns = "basic_shapes";
    marker.id = 0;

    // Set the marker type.  Initially this is CUBE, and cycles between that and SPHERE, ARROW, and CYLINDER
    marker.type = shape;

    // Set the marker action.  Options are ADD, DELETE, and new in ROS Indigo: 3 (DELETEALL)
    marker.action = visualization_msgs::Marker::ADD;


    // Set the pose of the marker.  This is a full 6DOF pose relative to the frame/time specified in the header
    marker.pose.position.x = pickup[0];
    marker.pose.position.y = pickup[1];
    marker.pose.position.z = 0;
    // marker.pose.orientation.x = 0.0;
    // marker.pose.orientation.y = 0.0;
    // marker.pose.orientation.z = 0.0;
    // marker.pose.orientation.w = 1.0;

    // Set the scale of the marker -- 1x1x1 here means 1m on a side
    marker.scale.x = 0.5;
    marker.scale.y = 0.5;
    marker.scale.z = 0.1;

    // Set the color -- be sure to set alpha to something non-zero!
    marker.color.r = 0.0f;
    marker.color.g = 1.0f;
    marker.color.b = 0.0f;
    marker.color.a = 1.0;

    marker.lifetime = ros::Duration();

    // Publish the marker
    while (marker_pub.getNumSubscribers() < 1)
    {
      if (!ros::ok())
      {
        return 0;
      }
      ROS_WARN_ONCE("Please create a subscriber to the marker");
      sleep(1);
    }
    marker_pub.publish(marker);
    ROS_INFO("Pickup mark displayed");
//wait for the robot to reach the pickup location
    while (!pickup_loc) {
      ros::spinOnce();
    }
//check if the robot has arrived to the pick location and pick up hasn't been done yet
    if(pickup_loc && !pickup_complete)
    {
      marker.action = visualization_msgs::Marker::DELETE;
      marker_pub.publish(marker);
      ROS_INFO("Pickup marker deleted");
      pickup_complete = true;
      ros::Duration(5.0).sleep();
    }

//wait for robot to reach drop off location
  while(!dropoff_loc)
  {
    ros::spinOnce();
  }
//check if the robot has arrived to the pick location and pick up hasn't been done yet
    if(dropoff_loc && !dropoff_complete)
      {
        marker.action = visualization_msgs::Marker::ADD;

        marker.pose.position.x = dropoff[0];
        marker.pose.position.y = dropoff[1];
        marker.pose.position.z = 0;

        marker_pub.publish(marker);
        ROS_INFO("Drop off mark displayed");
        dropoff_complete - true;
        ros::Duration(5.0).sleep();
      }
    return 0;
  }
}
