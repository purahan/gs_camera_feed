#include "usb_device.h"

/*
Uses libsystemd to get access to sd api (pretty much udev).
View potential device params ex: "udevadm info /dev/video0"
*/

// Gets a list of devices active connected to the computer.
// Returns a list of pairs of (serial_ID, device_path).
std::vector<std::pair<std::string, std::string>> get_device_list() 
{
    std::vector<std::pair<std::string, std::string>> device_list;
    
    sd_device_enumerator *enumerator = nullptr;

    sd_device_enumerator_new(&enumerator);

    if (enumerator == nullptr) {
        std::cout << "ERROR: Unable to allocate new device enumerator!" << std::endl;
        return device_list;
    }

    sd_device *device = sd_device_enumerator_get_device_first(enumerator);

    while (device != nullptr)
    {
        const char* device_path = nullptr;
        const char* serial_ID = nullptr;
        
        sd_device_get_property_value(device, "DEVNAME", &device_path);
        sd_device_get_property_value(device, "ID_SERIAL", &serial_ID);

        if (device_path != nullptr && serial_ID != nullptr)
        {
            device_list.push_back(std::make_pair(serial_ID, device_path));
        }

        device = sd_device_enumerator_get_device_next(enumerator);
    }

    sd_device_enumerator_unref(enumerator);

    return device_list;
}

// Returns a device path given the udev serial ID and a keyword to
// compare to a viable device path.
std::string get_device_path(std::string serial_ID, std::string path_hint)
{
    std::vector<std::pair<std::string, std::string>> device_list = get_device_list();

    for (auto info_pair : device_list)
    {
        std::string id = info_pair.first;

        if (id == serial_ID)
        {
            std::string device_path = info_pair.second;

            // Check if path contains path hint.
            size_t path_hint_index = device_path.find(path_hint);

            // Ignore if does not contain path hint.
            if (path_hint_index == std::string::npos)
                continue;
            
            // TODO - Replace this with check for ID_V4L_CAPABILITIES=:capture:
            // Get rid of odd numbered dev paths for /dev/video#
            // odd numbers are for camera control rather than image output
            if ((int(device_path[device_path.length() - 1]) - 48) % 2 == 1) {
                continue;
            }

            return device_path;
        }
    }

    return "";
}