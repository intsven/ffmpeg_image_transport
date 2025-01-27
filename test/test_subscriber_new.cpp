#include <memory>
#include <string>
#include <chrono>

#include "rclcpp/rclcpp.hpp"
#include "ffmpeg_image_transport_msgs/msg/ffmpeg_packet.hpp"

class MinimalSubscriber : public rclcpp::Node
{
public:
  MinimalSubscriber(const std::string & topic_name)
  : Node("minimal_subscriber"), packet_count_(0)
  {
    auto topic_callback =
      [this](ffmpeg_image_transport_msgs::msg::FFMPEGPacket::UniquePtr msg) -> void {
        //RCLCPP_INFO(this->get_logger(), "I heard a packet with size: '%zu'", msg->data.size());
        packet_count_++;
      };
    subscription_ =
      this->create_subscription<ffmpeg_image_transport_msgs::msg::FFMPEGPacket>(topic_name, 10, topic_callback);

    timer_ = this->create_wall_timer(
      std::chrono::seconds(1),
      [this]() -> void {
        RCLCPP_INFO(this->get_logger(), "Packets received in the last second: %d", packet_count_);
        packet_count_ = 0;
      });
  }

private:
  rclcpp::Subscription<ffmpeg_image_transport_msgs::msg::FFMPEGPacket>::SharedPtr subscription_;
  rclcpp::TimerBase::SharedPtr timer_;
  int packet_count_;
};

int main(int argc, char * argv[])
{
  rclcpp::init(argc, argv);

  if (argc < 2) {
    RCLCPP_ERROR(rclcpp::get_logger("rclcpp"), "Usage: %s <topic_name>", argv[0]);
    return 1;
  }

  std::string topic_name = argv[1];
  rclcpp::spin(std::make_shared<MinimalSubscriber>(topic_name));
  rclcpp::shutdown();
  return 0;
}