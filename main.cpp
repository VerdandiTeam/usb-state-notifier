#include <libudev.h>
#include <iostream>
#include <cstring>
#include <fcntl.h>
#include <unistd.h>

enum {
    NONE,
    PERIPHERAL,
    HOST,
} USB_MODE;

bool setUSBMode(int value)
{
    int fd = open("/sys/devices/platform/soc/4e00000.ssusb/mode", O_WRONLY);

    if (fd < 0)
        return false;

    std::string out;
    switch (value) {
        case HOST:
            out = "host";
            break;
        case PERIPHERAL:
            out = "peripheral";
            break;
        case NONE:
        default:
            out = "none";
    }

    ssize_t written = write(fd, out.data(), out.length());

    close(fd);

    return written > 0;
}

int main() {
    setUSBMode(NONE);

    udev *udevCtx = udev_new();
    udev_monitor *monitor = udev_monitor_new_from_netlink(udevCtx, "udev");

    udev_monitor_filter_add_match_subsystem_devtype(monitor, "power_supply", nullptr);

    udev_monitor_enable_receiving(monitor);

    int fd = udev_monitor_get_fd(monitor);

    while (true) {
        fd_set fds;
        FD_ZERO(&fds);
        FD_SET(fd, &fds);

        select(fd + 1, &fds, nullptr, nullptr, nullptr);

        if (FD_ISSET(fd, &fds)) {
            udev_device *dev = udev_monitor_receive_device(monitor);

            if (dev && strcmp(udev_device_get_sysname(dev), "usb") == 0) {
                const char *onlineStr = udev_device_get_property_value(dev, "POWER_SUPPLY_ONLINE");
                int online = std::stoi(onlineStr);

                if (online) {
                    setUSBMode(PERIPHERAL);
                } else {
                    setUSBMode(NONE);
                }

                udev_device_unref(dev);
            }
        }
    }
}