#include <ping-device-ping1d.h>
#include <ping-message-common.h>
#include <ping-message-ping1d.h>
#include <ping-port-linux.h>

#include <stdio.h>

#define EXENAME "test-device-ping1d"

int main(int argc, char* argv[]) {
    printf(" ~* " EXENAME " *~\n");

    if (argc < 2 || argc > 2) {
        printf("usage: " EXENAME " <path to port>\n");
        printf("ex: " EXENAME " /dev/ttyUSB0\n");
        return 1;
    }

    const char* portFileName = argv[1];

    PingPortLinux port = PingPortLinux(portFileName);
    Ping1d device      = Ping1d(port);

    printf("initializing\n");

    if (!device.initialize(100)) {
        printf("device initialization failed\n");
    } else {
        printf("pass\n");
        printf("Device Type %d id %d hardware revision %d\n", device.device_type, device.device_id, device.device_revision);
        printf("Ping Protocol v%d.%d.%d\n", device.version_major, device.version_minor, device.version_patch);
        printf("Device Firmware v%d.%d.%d\n", device.firmware_version_major, device.firmware_version_minor, device.firmware_version_patch);
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

    auto testRequest = [&device](const char* message, uint16_t msgId) {
        printf("requesting %s\n", message);
        if (!device.request(msgId, 1000)) {
        printf("fail\n");
        return false;
        } else {
        printf("pass\n");
        return true;
        }
    };

    testRequest("firmware version", Ping1dId::FIRMWARE_VERSION);
    testRequest("device id", Ping1dId::DEVICE_ID);
    testRequest("voltage 5", Ping1dId::VOLTAGE_5);
    testRequest("speed of sound", Ping1dId::SPEED_OF_SOUND);
    testRequest("range", Ping1dId::RANGE);
    testRequest("auto mode", Ping1dId::MODE_AUTO);
    testRequest("ping interval", Ping1dId::PING_INTERVAL);
    testRequest("gain setting", Ping1dId::GAIN_SETTING);
    testRequest("transmit duration", Ping1dId::TRANSMIT_DURATION);
    testRequest("general info", Ping1dId::GENERAL_INFO);
    testRequest("distance simple", Ping1dId::DISTANCE_SIMPLE);
    testRequest("distance", Ping1dId::DISTANCE);
    testRequest("processor temperature", Ping1dId::PROCESSOR_TEMPERATURE);
    testRequest("pcb temperature", Ping1dId::PCB_TEMPERATURE);
    testRequest("ping enable", Ping1dId::PING_ENABLE);
    testRequest("profile", Ping1dId::PROFILE);

    return 0;
}
