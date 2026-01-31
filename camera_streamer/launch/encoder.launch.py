from launch import LaunchDescription
from launch_ros.actions import Node
from dataclasses import dataclass

HOST_MACHINE = "amd64" # jetson or amd64
AUTO_ENABLE_CAMERAS = True
JPEG_COMPRESSION = 10 # 1 - 100, where lower is more compressed.


@dataclass
class CameraEncoder():

    camera_name: str = ""
    serial_ID: str = ""

    cap_width: int = 1280
    cap_height: int = 720
    cap_framerate: int = 30

    send_width: int = 1280
    send_height: int = 720
    send_framerate: int = 30

    jpeg_quality: int = JPEG_COMPRESSION # 1 - 100   

    auto_enable: bool = AUTO_ENABLE_CAMERAS
    host_machine: str = HOST_MACHINE

    def get_node(self) -> Node:
        
        camera_encoder_node = Node(
            package = 'camera_streamer',
            executable = 'camera_encoder',
            name = self.camera_name + "_enc",
            output='screen',
            parameters=[
                {'camera_name': self.camera_name},
                {'serial_ID': self.serial_ID},
                {'camera_cap_width': self.cap_width},
                {'camera_cap_height': self.cap_height},
                {'camera_cap_fps': self.cap_framerate},
                {'image_send_width': self.send_width},
                {'image_send_height': self.send_height},
                {'image_send_fps': self.send_framerate},
                {'auto_enable_camera': self.auto_enable},
                {'host_machine': self.host_machine},

                # Specific to compressed image transport
                {self.camera_name + '.transport.format': 'jpeg'},
                {self.camera_name + '.transport.jpeg_quality': self.jpeg_quality}
            ],
        )  

        return camera_encoder_node  


def generate_launch_description():

    camera_list = (
        # CameraEncoder("logi_webcam", "046d_C922_Pro_Stream_Webcam_F5B8DF8F"),
        CameraEncoder("laptop_webcam", "_Integrated_Camera_0001"),
    )
    
    node_list = []

    for camera in camera_list:
        node_list.append(camera.get_node())

    return LaunchDescription(node_list)

