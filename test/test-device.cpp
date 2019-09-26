#include <ping-device.h>
#include <ping-message-common.h>
#include <ping-port-linux.h>

#include <stdio.h>

#define EXENAME "test-device"

int main(int argc, char* argv[]) {
    printf(" ~* " EXENAME " *~\n");

    if (argc < 2 || argc > 2) {
        printf("usage: " EXENAME " <path to port>\n");
        printf("ex: " EXENAME " /dev/ttyUSB0\n");
        return 1;
    }

    const char* portFileName = argv[1];

    PingPortLinux port = PingPortLinux(portFileName);
    PingDevice device      = PingDevice(port);

    printf("initializing\n");

    if (!device.initialize()) {
        printf("device initialization failed\n");
    } else {
        printf("pass\n");
        printf("Device Type %d id %d hardware revision %d\n", device.device_type, device.device_id, device.device_revision);
        printf("Ping Protocol v%d.%d.%d\n", device.version_major, device.version_minor, device.version_patch);
        printf("Device Firmware v%d.%d.%d\n", device.firmware_version_major, device.firmware_version_minor, device.firmware_version_patch);
    }

    auto testRequest = [&device](const char* message, uint16_t msgId) {
        printf("requesting %s\n", message);
        if (!device.request(msgId, 500)) {
        printf("fail\n");
        return false;
        } else {
        printf("pass\n");
        return true;
        }
    };

    testRequest("protocol version", CommonId::PROTOCOL_VERSION);
    testRequest("device information", CommonId::DEVICE_INFORMATION);

    return 0;
}
