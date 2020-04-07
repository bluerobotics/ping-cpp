#include <link/desktop/abstract-link.h>
#include <ping-device.h>
#include <ping-message-all.h>

#include "command-line/command-line.h"

#include <cstdio>

/**
 * @brief Test generic ping devices
 */
int main(int argc, char* argv[])
{
    if (!CommandLine::self()->run(argc, argv)) {
        return -1;
    }

    auto port = AbstractLink::openUrl(CommandLine::self()->connectionString);
    PingDevice device = PingDevice(*port.get());

    printf("initializing\n");

    if (!device.initialize()) {
        printf("device initialization failed\n");
    } else {
        printf("pass\n");
        printf("Device Type %d id %d hardware revision %d\n", device.device_type, device.device_id,
            device.device_revision);
        printf("Ping Protocol v%d.%d.%d\n", device.version_major, device.version_minor, device.version_patch);
        printf("Device Firmware v%d.%d.%d\n", device.firmware_version_major, device.firmware_version_minor,
            device.firmware_version_patch);
    }

    auto testRequest = [&device](uint16_t msgId, const char* message = nullptr) {
        printf("requesting message: %s\n",
            message ? message : PingHelper::nameFromMessageId(static_cast<PingEnumNamespace::PingMessageId>(msgId)));
        if (!device.request(msgId, 1000)) {
            printf("fail\n");
            return false;
        }

        printf("pass\n");
        return true;
    };

    testRequest(CommonId::PROTOCOL_VERSION);
    testRequest(CommonId::DEVICE_INFORMATION);

    return 0;
}
