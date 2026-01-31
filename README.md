# GStream Camera Feed with ROS2(Humble)

## Dependencies
* Install ROS 2 Compressed Image Transport:
```
sudo apt install ros-humble-compressed-image-transport
```
* Make sure Compressed Image Transport is listed:
```
ros2 run image_transport list_transports
```
* OpenCV 4.x
* libsystemd-dev:
```
sudo apt install libsystemd-dev
```

## Usage:
* Find the serial ID for the USB camera devices using the `find_devpath.bash` file within the `utils/` folder.
* Set the proper host machine architecture using `HOST_MACHINE` within `encoder.launch.py`.
* Add `CameraEncoder` objects to the `encoder.launch.py` file, and give each a camera name and serial ID.
* Add `CameraDecoder` objects to the `decoder.launch.py` file, and specify the camera name.
Then `colcon build` it and start `encoder.launch.py` on the `HOST_MACHINE` and `decoder.launch.py` on the `TARGET_MACHINE`


