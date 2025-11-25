#include <gz/msgs/twist.pb.h>
#include <gz/msgs/laserscan.pb.h>
#include <gz/transport/Node.hh>

gz::transport::Node node;

//this a modification of the tutorial's code, customized for this specific project.
//publishers for the robots.
auto pub_blue = node.Advertise<gz::msgs::Twist>("/cmd_vel");
auto pub_green = node.Advertise<gz::msgs::Twist>("/cmd_vel_green");

//yellow robot logic (used to be blue)
void cb_blue(const gz::msgs::LaserScan &_msg)
{
  gz::msgs::Twist data;
  bool allMore = true;
  
  for (int i = 0; i < _msg.ranges_size(); i++) {
    if (_msg.ranges(i) < 1.0) { allMore = false; break; }
  }

  if (allMore) {
    data.mutable_linear()->set_x(0.5);
    data.mutable_angular()->set_z(0.0);
  } else {
    data.mutable_linear()->set_x(0.0);
    data.mutable_angular()->set_z(0.5); //turn left
  }
  pub_blue.Publish(data);
}

//green robot logic (same but turns in the opposite direction).
void cb_green(const gz::msgs::LaserScan &_msg)
{
  gz::msgs::Twist data;
  bool allMore = true;

  for (int i = 0; i < _msg.ranges_size(); i++) {
    if (_msg.ranges(i) < 1.0) { allMore = false; break; }
  }

  if (allMore) {
    data.mutable_linear()->set_x(0.5);
    data.mutable_angular()->set_z(0.0);
  } else {
    data.mutable_linear()->set_x(0.0);
    data.mutable_angular()->set_z(-0.5); //change is here, it turns right.
  }
  pub_green.Publish(data);
}

int main(int argc, char **argv)
{
  //subscribing to both robots sensors
  if (!node.Subscribe("/lidar", cb_blue)) {
    std::cerr << "Error subscribing to Blue Lidar" << std::endl;
    return -1;
  }

  if (!node.Subscribe("/lidar_green", cb_green)) {
    std::cerr << "Error subscribing to Green Lidar" << std::endl;
    return -1;
  }

  std::cout << "Central Brain Running: Controlling Blue and Green robots..." << std::endl;
  gz::transport::waitForShutdown();
  return 0;
}