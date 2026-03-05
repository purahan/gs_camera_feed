# GStreamer Camera Feed with ROS 2 (Humble)

This repository provides a high-performance ROS 2 package for streaming USB camera feeds using GStreamer and OpenCV. It features hardware-aware pipelines for both x86 (amd64) and NVIDIA Jetson platforms, along with dynamic configuration services.

## Key Features
* **Hardware-Optimized Pipelines**: Specific GStreamer strings for Jetson (`v4l2src` with `jpegdec`) and amd64 (`decodebin`) architectures.
* **Dynamic Control**: Change resolution and framerate at runtime via ROS 2 services without restarting nodes.
* **Persistant Mapping**: Includes utilities to map USB serial IDs to device paths, ensuring the correct camera is used even if `/dev/videoX` indices change.
* **Built-in Filtering**: Includes a "daylight" filter to enhance image warmth.
* **Transport Support**: Fully compatible with `image_transport` for compressed image streams.

## Dependencies
Ensure you have the following installed on your system:
* **ROS 2 Compressed Image Transport**:
  ```bash
  sudo apt install ros-humble-compressed-image-transport
  ```
* **System Libraries**:
  ```bash
  sudo apt install libsystemd-dev
  ```
* **OpenCV 4.x** (typically included with ROS 2 Humble desktop installations).
## Installation
1. Clone this repository into your ROS 2 workspace:
   ```bash
   cd ~/ros2_ws/src
   git clone <repository_url>
   ```
2. Build the workspace (ensure `custom_interfaces` is built first):
   ```bash
   cd ~/ros2_ws
   colcon build --packages-select custom_interfaces camera_streamer
   source install/setup.bash
   ```
## Usage
**1. Identify Your Camera**
Run the utility script to find the serial ID for your USB camera:
```bash
bash utils/find_devpath.bash
```

**2. Configure the Encoder**
Edit `camera_streamer/launch/encoder.launch.py`:
* Set `HOST_MACHINE` to "amd64" or "jetson".
* Update the `CameraEncoder` object with your camera's name and the **Serial ID** found in the previous step.
**3. Launch the Nodes
On the Host (Camera Side):**
```bash
ros2 launch camera_streamer encoder.launch.py
```
**On the Target (Viewer Side):**
```bash
ros2 launch camera_streamer decoder.launch.py
```
## ROS 2 Interfaces
**Services**
* **Toggle Camera** (`/<camera_name>/toggle_camera`): Starts or stops the video capture.
* **Set Encoder Config** (`/<camera_name>/set_encoder_config`): Dynamically updates `image_width`, `image_height`, and `frame_rate`.
**Parameters**
* `camera_cap_width` / `camera_cap_height`: The raw resolution captured from the hardware.
* `image_send_width` / `image_send_height`: The resolution after resizing, sent over the network.
* `jpeg_quality`: Compression level (1-100) for the `compressed` transport.

## Package Structure
* `camera_streamer`: Contains the core encoder, decoder, and viewer nodes.
* `custom_interfaces`: Defines the `SetEncoderConfig` service.
* `utils`: Helper scripts for hardware identification.
