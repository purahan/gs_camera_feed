#include <string.h>
#include "image_transport/image_transport.hpp"
#include "rclcpp/rclcpp.hpp"

void topic_callback(const sensor_msgs::msg::Image::ConstSharedPtr & msg, rclcpp::Publisher<sensor_msgs::msg::Image>::SharedPtr publisher)
{
    publisher->publish(*msg);
}

void exit_signal_callback(int signum)
{
    signum = signum; // Prevent ununsed param warning.
    rclcpp::shutdown();
    exit(0);
}

int main(int argc, char ** argv)
{
    // Register signal handlers
    signal(SIGINT, exit_signal_callback);

    const int QUEUE_SIZE = 1;

    rclcpp::init(argc, argv);

    rclcpp::NodeOptions options;
    rclcpp::Node::SharedPtr node = rclcpp::Node::make_shared("camera_subscriber", options);

    node->declare_parameter("camera_name", "camera1");
    node->declare_parameter("image_transport", "compressed");
    
    /* ^ Set this parameter to the desired image transport.
     The topic name for this node will automatically be resolved to the appropriate 
     base topic + image transport name.
     The external publisher automatically advertises all available image transports.
     Use 'ros2 run image_transport list_transports' to list available transports.
    */

    std::string camera_name = node->get_parameter("camera_name").as_string();

    std::string base_topic = camera_name + "/transport";
    std::string output_topic = camera_name + "/out";

    rclcpp::SensorDataQoS qos;
    rclcpp::Publisher<sensor_msgs::msg::Image>::SharedPtr publisher = node->create_publisher<sensor_msgs::msg::Image>(output_topic, qos);

    image_transport::ImageTransport transport(node);
    image_transport::Subscriber subscriber = transport.subscribe(base_topic, QUEUE_SIZE, std::bind(topic_callback, std::placeholders::_1, publisher));

    rclcpp::spin(node);

    return 0;
}
