#include <link/desktop/abstract-link.h>
#include <ping-device.h>
#include <ping-message-all.h>

#include "command-line/command-line.h"
#include "helper.h"

#include <cstdio>

typedef PingDeviceTest::Status Status;

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

    // Basic information
    PingDeviceTest::test("Device initialization", device.initialize());
    PingDeviceTest::test("Device type", static_cast<int>(PingDeviceType::UNKNOWN) < device.device_information.device_type, Status::OK);
    std::cout << "Device detected as: " << PingHelper::nameFromDeviceType(static_cast<PingDeviceType>(device.device_information.device_type)) << std::endl;
    PingDeviceTest::test("Device ID", device.device_id != 0, Status::OK);

    // Common messages validation
    PingDeviceTest::test("Device Hardware revision", device.device_information.device_revision, Status::OK);
    PingDeviceTest::test(
        "Protocol version",
        device.protocol_version.version_major == 1 && device.protocol_version.version_minor == 0 && device.protocol_version.version_patch == 0,
        Status::OK
    );
    PingDeviceTest::test(
        "Firmware version",
        device.device_information.firmware_version_major != 0 || device.device_information.firmware_version_minor != 0 || device.device_information.firmware_version_patch != 0,
        Status::OK
    );

    return 0;
}
