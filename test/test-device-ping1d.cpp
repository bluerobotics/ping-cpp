#include <link/desktop/abstract-link.h>
#include <ping-device-ping1d.h>
#include <ping-message-all.h>

#include "command-line/command-line.h"

#include "helper.h"

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
    if (!port) {
        std::cerr << "Failed to open communication link with device" << std::endl;
        return -1;
    }
    Ping1d device = Ping1d(*port.get());

    // Basic information
    PingDeviceTest::test("Device initialization", device.initialize(100));
    PingDeviceTest::test("Device type", static_cast<PingDeviceType>(device.device_information.device_type) == PingDeviceType::PING1D);
    PingDeviceTest::test("Device ID", device.device_id == 1);

    PingDeviceTest::test("Ping protocol version",
        device.protocol_version.version_major == 1);
    PingDeviceTest::test("Device Firmware",
        device.device_information.firmware_version_major >= 3 && device.device_information.firmware_version_minor >= 27);

    // Test commands
    PingDeviceTest::test("Set mode", device.set_mode_auto(true));
    PingDeviceTest::test("Set range", !device.set_range(100, 2000)); // We can't set range in auto mode
    PingDeviceTest::test("Set mode", device.set_mode_auto(false));
    PingDeviceTest::test("Set range", device.set_range(100, 2000)); // We are not in auto mode anymore

    PingDeviceTest::test("Set speed of sound", device.set_speed_of_sound(1550000));
    PingDeviceTest::test("Set mode", device.set_mode_auto(false));
    PingDeviceTest::test("Set interval", device.set_ping_interval(200));
    PingDeviceTest::test("Set gain", device.set_gain_setting(1));
    PingDeviceTest::test("Set ping enable", device.set_ping_enable(1));

    // Test request
    auto requestIDs = {
        Ping1dId::FIRMWARE_VERSION,
        Ping1dId::DEVICE_ID,
        Ping1dId::VOLTAGE_5,
        Ping1dId::SPEED_OF_SOUND,
        Ping1dId::RANGE,
        Ping1dId::MODE_AUTO,
        Ping1dId::PING_INTERVAL,
        Ping1dId::GAIN_SETTING,
        Ping1dId::TRANSMIT_DURATION,
        Ping1dId::GENERAL_INFO,
        Ping1dId::DISTANCE_SIMPLE,
        Ping1dId::DISTANCE,
        Ping1dId::PROCESSOR_TEMPERATURE,
        Ping1dId::PCB_TEMPERATURE,
        Ping1dId::PING_ENABLE,
        Ping1dId::PROFILE,
    };

    for (auto id : requestIDs) {
        std::string title = "Requesting message [";
        title.append(PingHelper::nameFromMessageId(static_cast<PingEnumNamespace::PingMessageId>(id)));
        title.append("]");

        PingDeviceTest::test(title, device.request(id, 1000));
    }

    return 0;
}
