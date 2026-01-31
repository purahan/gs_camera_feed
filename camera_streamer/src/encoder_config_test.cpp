#include <string.h>
#include "rclcpp/rclcpp.hpp"
#include "std_srvs/srv/set_bool.hpp"
#include "custom_interfaces/srv/set_encoder_config.hpp"


int main(int argc, char ** argv)
{
    rclcpp::init(argc, argv);

    rclcpp::NodeOptions options;
    rclcpp::Node::SharedPtr node = rclcpp::Node::make_shared("encoder_config_test", options);

    node->declare_parameter("camera_name", "camera1");
    node->declare_parameter("toggle_camera", true);

    node->declare_parameter("image_width", 1280);
    node->declare_parameter("image_height", 720);
    node->declare_parameter("frame_rate", 30);

    std::string cameraName = node->get_parameter("camera_name").as_string();

    bool enableCamera = node->get_parameter("toggle_camera").as_bool();

    int imageWidth = node->get_parameter("image_width").as_int();
    int imageHeight = node->get_parameter("image_height").as_int();
    int frameRate = node->get_parameter("frame_rate").as_int();

    std::string toggle_camera_srv_name = cameraName + "/toggle_camera";
    std::string set_enc_cfg_srv_name = cameraName + "/set_encoder_config";

    // Create clients -----------------

    auto toggle_camera_cli =
        node->create_client<std_srvs::srv::SetBool>(toggle_camera_srv_name);

    auto set_encoder_config_cli =
        node->create_client<custom_interfaces::srv::SetEncoderConfig>(set_enc_cfg_srv_name);

    // Create service requests -----------------

    auto toggle_camera_request = std::make_shared<std_srvs::srv::SetBool::Request>();

    auto set_encoder_config_request = std::make_shared<custom_interfaces::srv::SetEncoderConfig::Request>();
    
    // Assign data to request ------------------------------

    toggle_camera_request->data = enableCamera;

    set_encoder_config_request->image_width = imageWidth;
    set_encoder_config_request->image_height = imageHeight;
    set_encoder_config_request->frame_rate = frameRate;

    if (toggle_camera_cli->service_is_ready() == false ||
        set_encoder_config_cli->service_is_ready() == false)
    {
        std::cout << "Error: A service is not available!" << std::endl;
        rclcpp::shutdown();
        exit(1);
    }

    auto toggle_camera_result = toggle_camera_cli->async_send_request(toggle_camera_request);

    // Wait for the result.
    if (rclcpp::spin_until_future_complete(node, toggle_camera_result) ==
        rclcpp::FutureReturnCode::SUCCESS)
    {
        std::cout << "Camera toggle {" << enableCamera << "} ";

        if (toggle_camera_result.get()->success)
            std::cout << "successful!";
        else
            std::cout << "failed!";
        
        std::cout << std::endl;
    }

    auto set_encoder_config_result = set_encoder_config_cli->async_send_request(set_encoder_config_request);

    // Wait for the result.
    if (rclcpp::spin_until_future_complete(node, set_encoder_config_result) ==
        rclcpp::FutureReturnCode::SUCCESS)
    {
        std::cout << "Encoder config set {" 
            << imageWidth << " x " << imageHeight
            << " @ " << frameRate << " Hz" << "} ";

        if (set_encoder_config_result.get()->success)
            std::cout << "successful!";
        else
            std::cout << "failed!";
        
        std::cout << std::endl;
    }

    rclcpp::shutdown();
    return 0;
}
