#include <string.h>

#include "cv_bridge/cv_bridge.h"
#include "opencv2/core/mat.hpp"
#include "opencv2/highgui.hpp"
#include "opencv2/videoio.hpp"
#include "rclcpp/rclcpp.hpp"
#include "sensor_msgs/msg/image.hpp"


class CameraViewer : public rclcpp::Node 
{

public:

    CameraViewer() : Node("camera_viewer")
    {
        this->declare_parameter("camera_name", "camera1");
        this->declare_parameter("window_width", 960);
        this->declare_parameter("window_height", 540);

        camera_name = this->get_parameter("camera_name").as_string();
        window_width = this->get_parameter("window_width").as_int();
        window_height = this->get_parameter("window_height").as_int();

        // TEMP
        // Info found from 'xprop | grep FRAME'
        int title_bar_height = 37;
        window_height -= title_bar_height;

        input_topic = camera_name + "/out";

        rclcpp::SensorDataQoS qos;
        camera_sub = this->create_subscription<sensor_msgs::msg::Image>(
                input_topic,
                qos,
                std::bind(&CameraViewer::topic_callback, this, std::placeholders::_1)
            );
        
        cv::namedWindow(camera_name, cv::WINDOW_NORMAL);
        cv::startWindowThread();
    }

    ~CameraViewer()
    {
        cv::destroyWindow(camera_name);
    }

private:

    std::string camera_name;
    std::string input_topic;

    int window_width; 
    int window_height;

    rclcpp::Subscription<sensor_msgs::msg::Image>::SharedPtr camera_sub;


    void topic_callback(const sensor_msgs::msg::Image::ConstSharedPtr &msg)
    {
        cv_bridge::CvImageConstPtr frame_ptr;

        try
        {
            frame_ptr = cv_bridge::toCvShare(msg, msg->encoding);
        }
        catch (cv_bridge::Exception& e)
        {
            RCLCPP_ERROR(this->get_logger(), "cv_bridge exception: %s", e.what());
            return;
        }

        cv::putText(frame_ptr->image, std::to_string(int(this->now().seconds())), cv::Point(5, 25), cv::FONT_HERSHEY_SIMPLEX, 1, cv::Scalar(21, 255, 0), 2);

        cv::resizeWindow(camera_name, cv::Size(window_width, window_height));

        cv::imshow(camera_name, frame_ptr->image);
        cv::waitKey(1);
    }
};

int main(int argc, char** argv)
{
    rclcpp::init(argc, argv);
    rclcpp::spin(std::make_shared<CameraViewer>());
    rclcpp::shutdown();
    return 0;
}