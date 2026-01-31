#pragma once

#include <iostream>
#include <string>
#include <systemd/sd-device.h>
#include <vector>
#include <utility>

std::vector<std::pair<std::string, std::string>> get_device_list();

std::string get_device_path(std::string serial_ID, std::string path_hint);
