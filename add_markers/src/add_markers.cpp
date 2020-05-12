#include <ros/ros.h>
#include <visualization_msgs/Marker.h>
#include <nav_msgs/Odometry.h>
#include <std_msgs/UInt8.h>

using namespace std;


//initialise global variable to keep track of the current search phase from the pick_objects node
uint8_t trigger = 0;

// Define destination subscriber callback to set the search phase to the received value
void pickupDropoffCheck(const std_msgs::UInt8::ConstPtr& msg)
{
   ROS_INFO("Received message!");
   ROS_INFO("Message is: %d ", msg->data);
   trigger = msg->data;
   return;
}

//Pickup and Dropoff locations
float pickup[2] = {-0.5, -7};
float dropoff[2] = {-2.1,-2.1};


int main( int argc, char** argv )
{
  ros::init(argc, argv, "add_markers");
  ros::NodeHandle n;
  ros::Rate r(1);
  ros::Publisher marker_pub = n.advertise<visualization_msgs::Marker>("visualization_marker", 1);
  ros::Subscriber location_sub = n.subscribe("/target_reached", 1, pickupDropoffCheck);

  // Set our initial shape type to be a cube
  uint32_t shape = visualization_msgs::Marker::CUBE;

  while (ros::ok())
  {
    //Do this every cycle to ensure the subscriber receives the message
    ros::spinOnce();

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


//wait for the robot to reach the pickup location
    while (trigger==0) {
      marker_pub.publish(marker);
      ROS_WARN_ONCE("Pickup mark displayed");
      ros::spinOnce();
      }
//check if the robot has arrived to the pick location if so delete marker
    if(trigger==1)
    {
      marker.action = visualization_msgs::Marker::DELETE;
      marker_pub.publish(marker);
      ROS_WARN_ONCE("Pickup marker deleted");
//      ros::Duration(5.0).sleep();
    }
    //else {ros::spinOnce();}

//check if the robot has arrived to the dropoff location and if so show marker
    if(trigger==2)
      {
        marker.action = visualization_msgs::Marker::ADD;

        marker.pose.position.x = dropoff[0];
        marker.pose.position.y = dropoff[1];
        marker.pose.position.z = 0;

        marker_pub.publish(marker);
        ROS_WARN_ONCE("Drop off mark displayed");
        ros::Duration(5.0).sleep();
      }

    }
    return 0;
  }
