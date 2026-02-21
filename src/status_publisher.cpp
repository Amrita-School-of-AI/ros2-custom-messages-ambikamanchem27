#include "rclcpp/rclcpp.hpp"
#include "ros2_custom_msgs/msg/robot_status.hpp"

using namespace std::chrono_literals;

class StatusPublisher : public rclcpp::Node
{
public:
  StatusPublisher() : Node("status_publisher")
  {
    publisher_ = this->create_publisher<
      ros2_custom_msgs::msg::RobotStatus>("/robot_status", 10);

    timer_ = this->create_wall_timer(
      1s, std::bind(&StatusPublisher::publish_msg, this));
  }

private:
  void publish_msg()
  {
    ros2_custom_msgs::msg::RobotStatus msg;
    msg.robot_name = "Explorer1";
    msg.battery_level = battery_;
    msg.is_active = true;
    msg.mission_count = mission_count_;

    publisher_->publish(msg);

    RCLCPP_INFO(this->get_logger(),
      "Robot=%s Battery=%.2f Missions=%d",
      msg.robot_name.c_str(),
      msg.battery_level,
      msg.mission_count);

    battery_ -= 1.0;
    mission_count_++;
  }

  rclcpp::Publisher<ros2_custom_msgs::msg::RobotStatus>::SharedPtr publisher_;
  rclcpp::TimerBase::SharedPtr timer_;
  double battery_ = 100.0;
  int mission_count_ = 0;
};

int main(int argc, char * argv[])
{
  rclcpp::init(argc, argv);
  rclcpp::spin(std::make_shared<StatusPublisher>());
  rclcpp::shutdown();
  return 0;
}
