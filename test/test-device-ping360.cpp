#include <link/desktop/abstract-link.h>
#include <ping-device-ping360.h>
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
    Ping360 device = Ping360(*port.get());

    // Basic information
    PingDeviceTest::test("Device initialization", device.initialize());
    PingDeviceTest::test("Device type", static_cast<PingDeviceType>(device.device_information.device_type) == PingDeviceType::PING360);
    PingDeviceTest::test("Device ID", device.device_id == 2);

    PingDeviceTest::test("Ping protocol version",
        device.protocol_version.version_major == 1);

    printf("Firmware version: %d.%d.%d\n", device.device_information.firmware_version_major, device.device_information.firmware_version_minor, device.device_information.firmware_version_patch);

    for(int number = 0; number < 200; number += 1) {
        device.set_transducer(
            1           //mode
            ,1          //gain_setting
            ,number     //angle
            ,100        //transmit_duration
            ,100        //sample_period
            ,750        //transmit_frequency
            ,200        //number_of_samples
            ,1          //transmit
            ,0          //reserved
        );

        [[maybe_unused]] const auto message_device_data = static_cast<const ping360_device_data*>(device.waitMessage(Ping360Id::DEVICE_DATA, 8000));
        assert(message_device_data);
        assert(message_device_data->angle() == number);
    }
    PingDeviceTest::test("Check 400 transducer messages", true);

    device.set_auto_transmit(
        1           //mode
        ,1          //gain_setting
        ,100        //transmit_duration
        ,100        //sample_period
        ,750        //transmit_frequency
        ,200        //number_of_samples
        ,0          //start_angle
        ,399        //stop_angle
        ,1          //number_of_steps
        ,0          //delay
    );
    for(int number = 0; number < 200; number += 1) {
        [[maybe_unused]] const auto message_device_data = static_cast<const ping360_auto_device_data*>(device.waitMessage(Ping360Id::AUTO_DEVICE_DATA, 8000));
        assert(message_device_data);
        assert(message_device_data->angle() == number);
    }
    PingDeviceTest::test("Check 400 auto transducer messages", true);

    device.set_motor_off();
    PingDeviceTest::test("Turn off motor", device.waitMessage(CommonId::ACK, 1000));

    return 0;
}
