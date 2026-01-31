from launch import LaunchDescription
from launch_ros.actions import Node

def generate_launch_description():

    encoder_config_test = Node(
        package='camera_streamer',
        executable='encoder_config_test',
        name='encoder_config_test',
        output='screen',
        parameters=[
            {"camera_name": "laptop_webcam_test"},
            {"toggle_camera": True},
            {"image_width": 500},
            {"image_height": 500},
            {"frame_rate": 30}
        ],
    )

    return LaunchDescription([
        encoder_config_test,
    ])

