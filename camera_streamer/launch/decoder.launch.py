from launch import LaunchDescription
from launch_ros.actions import Node
from dataclasses import dataclass

ENABLE_WINDOW_VIEW = True
IMAGE_TRANSPORT = "compressed"

@dataclass
class CameraDecoder():

    camera_name: str = ""
    image_transport: str = IMAGE_TRANSPORT

    def get_node(self) -> Node:
        
        camera_decoder_node = Node(
            package = 'camera_streamer',
            executable = 'camera_decoder',
            name = self.camera_name + "_dec",
            output='screen',
            parameters=[
                {"camera_name": self.camera_name},
                {"image_transport": self.image_transport}
            ],
        )  

        return camera_decoder_node 

@dataclass
class CameraViewer():

    camera_name: str = ""
    window_width: int = 640
    window_heigth: int = 480

    def get_node(self) -> Node:
        
        camera_viewer_node = Node(
            package='camera_streamer',
            executable='camera_viewer',
            name = self.camera_name + "_view",
            output='screen',
            parameters=[
                {'camera_name': self.camera_name},
                {'window_width': self.window_width},
                {'window_height': self.window_heigth}
            ],
        )

        return camera_viewer_node  
 

def generate_launch_description():

    camera_list = [
        # CameraDecoder("logi_webcam"),
        CameraDecoder("laptop_webcam"),
    ]

    node_list = []

    for camera in camera_list:
        node_list.append(camera.get_node())

    if ENABLE_WINDOW_VIEW == True:
        for camera in camera_list:
            node_list.append(CameraViewer(camera.camera_name).get_node())  

    return LaunchDescription(node_list)


