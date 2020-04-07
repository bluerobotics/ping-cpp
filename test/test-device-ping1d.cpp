#include <link/desktop/abstract-link.h>
#include <ping-device-ping1d.h>
#include <ping-message-all.h>

#include "command-line/command-line.h"

#include <cstdio>

/**
 * @brief Test ping 1D devices
 */
int main(int argc, char* argv[])
{
    if (!CommandLine::self()->run(argc, argv)) {
        return -1;
    }

    auto port = AbstractLink::openUrl(CommandLine::self()->connectionString);
    Ping1d device = Ping1d(*port.get());

    printf("initializing\n");

    if (!device.initialize(100)) {
        printf("device initialization failed\n");
    } else {
        printf("pass\n");
        printf("Device Type %d id %d hardware revision %d\n", device.device_type, device.device_id,
            device.device_revision);
        printf("Ping Protocol v%d.%d.%d\n", device.version_major, device.version_minor, device.version_patch);
        printf("Device Firmware v%d.%d.%d\n", device.firmware_version_major, device.firmware_version_minor,
            device.firmware_version_patch);
    }

    printf("set device id\n");
    if (!device.set_device_id(1)) {
        printf("failed to set device id\n");
    } else {
        printf("pass\n");
    }

    printf("set range\n");
    if (!device.set_range(100, 30000)) {
        printf("failed to set scan range\n");
    } else {
        printf("pass\n");
    }

    printf("set range\n");
    if (!device.set_speed_of_sound(1550000)) {
        printf("failed to set scan range\n");
    } else {
        printf("pass\n");
    }

    printf("set mode auto\n");
    if (!device.set_mode_auto(false)) {
        printf("failed to set device mode auto\n");
    } else {
        printf("pass\n");
    }

    printf("set ping interval\n");
    if (!device.set_ping_interval(200)) {
        printf("failed to set ping interval\n");
    } else {
        printf("pass\n");
    }

    printf("set gain setting\n");
    if (!device.set_gain_setting(1)) {
        printf("failed to set gain setting\n");
    } else {
        printf("pass\n");
    }

    printf("set ping enable\n");
    if (!device.set_ping_enable(1)) {
        printf("failed to set ping enable\n");
    } else {
        printf("pass\n");
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

    testRequest(Ping1dId::FIRMWARE_VERSION);
    testRequest(Ping1dId::DEVICE_ID);
    testRequest(Ping1dId::VOLTAGE_5);
    testRequest(Ping1dId::SPEED_OF_SOUND);
    testRequest(Ping1dId::RANGE);
    testRequest(Ping1dId::MODE_AUTO);
    testRequest(Ping1dId::PING_INTERVAL);
    testRequest(Ping1dId::GAIN_SETTING);
    testRequest(Ping1dId::TRANSMIT_DURATION);
    testRequest(Ping1dId::GENERAL_INFO);
    testRequest(Ping1dId::DISTANCE_SIMPLE);
    testRequest(Ping1dId::DISTANCE);
    testRequest(Ping1dId::PROCESSOR_TEMPERATURE);
    testRequest(Ping1dId::PCB_TEMPERATURE);
    testRequest(Ping1dId::PING_ENABLE);
    testRequest(Ping1dId::PROFILE);

    return 0;
}
