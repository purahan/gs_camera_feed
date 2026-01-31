cmake_minimum_required(VERSION 3.8)
project(camera_streamer)

#Default to C++14
#if(NOT CMAKE_CXX_STANDARD)
#  set(CMAKE_CXX_STANDARD 14)
#endif()

if(CMAKE_COMPILER_IS_GNUCXX OR CMAKE_CXX_COMPILER_ID MATCHES "Clang")
  add_compile_options(-Wall -Wextra -Wpedantic)
endif()

# find dependencies
find_package(ament_cmake REQUIRED)
find_package(rclcpp REQUIRED)
find_package(std_msgs REQUIRED)
find_package(sensor_msgs REQUIRED)
find_package(std_srvs REQUIRED)
find_package(image_transport REQUIRED)
find_package(cv_bridge REQUIRED)
find_package(OpenCV 4 REQUIRED)
find_package(custom_interfaces REQUIRED)
find_package(PkgConfig REQUIRED)
pkg_check_modules(SYSTEMD REQUIRED IMPORTED_TARGET libsystemd)



# Camera encoder
add_executable(camera_encoder src/camera_encoder.cpp src/usb_device.cpp)
ament_target_dependencies(camera_encoder rclcpp sensor_msgs std_srvs image_transport cv_bridge OpenCV custom_interfaces)
target_link_libraries(camera_encoder PkgConfig::SYSTEMD)

# Camera decoder
add_executable(camera_decoder src/camera_decoder.cpp)
ament_target_dependencies(camera_decoder rclcpp sensor_msgs image_transport cv_bridge OpenCV)

# Camera viewer
add_executable(camera_viewer src/camera_viewer.cpp)
ament_target_dependencies(camera_viewer rclcpp sensor_msgs image_transport cv_bridge OpenCV)

# Camera encoder config tester
add_executable(encoder_config_test src/encoder_config_test.cpp)
ament_target_dependencies(encoder_config_test rclcpp std_srvs custom_interfaces)


install(TARGETS
  camera_encoder
  camera_decoder
  camera_viewer
  encoder_config_test
  DESTINATION lib/${PROJECT_NAME})

install(
    DIRECTORY launch
    DESTINATION share/${PROJECT_NAME})

if(BUILD_TESTING)
  find_package(ament_lint_auto REQUIRED)
  # the following line skips the linter which checks for copyrights
  # comment the line when a copyright and license is added to all source files
  set(ament_cmake_copyright_FOUND TRUE)
  # the following line skips cpplint (only works in a git repo)
  # comment the line when this package is in a git repo and when
  # a copyright and license is added to all source files
  set(ament_cmake_cpplint_FOUND TRUE)
  ament_lint_auto_find_test_dependencies()
endif()

ament_package()
