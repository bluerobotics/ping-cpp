// Test ping-message c++ api
// Test creating ping_message object from byte array (receive)
// Test creating byte array from ping_message object (transmit)

#include <stdio.h>
#include "../src/message/ping-parser.h"
#include "../src/message/ping-message.h"
#include "../src/message/ping-message-common.h"
#include "../src/message/ping-message-ping1d.h"
#include "../src/message/ping-message-ping360.h"

template <class T>
bool compare(const char* s, T value, T expected);

static const uint8_t header_length = 8;
static const uint8_t checksum_length = 2;

///////////////
///// common
///////////////
static const uint16_t protocol_version_payload_length = 4;
static const uint32_t protocol_version_msg_length = header_length + protocol_version_payload_length + checksum_length;
struct test_protocol_version_s;
bool verifyProtocolMessage(common_protocol_version m, test_protocol_version_s t);
bool testProtocolVersion();

///////////////
///// ping1d
///////////////
static const uint16_t profile_static_payload_length = 4+2+2+4+4+4+4+2;
static const uint16_t profile_points = 10;
static const uint16_t profile_payload_length = profile_static_payload_length + profile_points;
static const uint32_t profile_msg_length = header_length + profile_payload_length + checksum_length;
struct test_profile_s;
bool verifyProfileMessage(ping1d_profile m, test_profile_s t);
bool testProfile();

// TODO maybe rework to read in test cases from file.
int main(void)
{
    bool ret = false;
    ret |= testProtocolVersion();
    ret |= testProfile();
    return ret;
}

template <class T>
bool compare(const char* s, T value, T expected)
{
    printf("%s: %d expected: %d\n", s, value, expected);
    if (value != expected) {
        printf("fail\n");
        return true;
    }
    return false;
}

struct test_protocol_version_s
{
    uint8_t start1;
    uint8_t start2;
    uint16_t payload_length;
    uint16_t message_id;

    uint8_t source_device_id;
    uint8_t destination_device_id;

    uint8_t version_major;
    uint8_t version_minor;
    uint8_t version_patch;
    uint8_t reserved;

    uint16_t checksum;
}__attribute__((packed));

bool verifyProtocolMessage(common_protocol_version m, test_protocol_version_s t)
{
    bool result = false;
    result |= compare("payload_length", m.payload_length(), t.payload_length);
    result |= compare("message_id", m.message_id(), t.message_id);
    result |= compare("source_id", m.source_device_id(), t.source_device_id);
    result |= compare("destination_id", m.destination_device_id(), t.destination_device_id);
    result |= compare("version_major", m.version_major(), t.version_major);
    result |= compare("version_minor", m.version_minor(), t.version_minor);
    result |= compare("version_patch", m.version_patch(), t.version_patch);
    result |= compare("reserved", m.reserved(), t.reserved);
    return result;
}

bool testProtocolVersion()
{
    bool result = false;

    // Hand-made test case
    union {
        test_protocol_version_s fields {
            'B',
            'R',
            4,
            5,
            77,
            211,
            1,
            2,
            3,
            99,
            550
        };
        uint8_t data[protocol_version_msg_length];
    } test;


    printf("Testing byte array to object conversion...\n\n");

    //TODO test all constructors
    //TODO test dynamic arrays, implicit and explicit length
    common_protocol_version test_in(test.data, protocol_version_msg_length);

    result |= verifyProtocolMessage(test_in, test.fields);

    printf("\nTesting object to byte array conversion...\n\n");

    common_protocol_version test_out;
    test_out.set_source_device_id(test.fields.source_device_id);
    test_out.set_destination_device_id(test.fields.destination_device_id);
    test_out.set_version_major(test.fields.version_major);
    test_out.set_version_minor(test.fields.version_minor);
    test_out.set_version_patch(test.fields.version_patch);
    test_out.set_reserved(test.fields.reserved);
    test_out.updateChecksum();

    for (uint8_t i = 0; i < protocol_version_msg_length; i++) {
        result |= compare("checking output byte", test_out.msgData[i], test.data[i]);
    }

    printf("\ntesting parser...\n\n");

    PingParser p;
    for(uint8_t i = 0; i < protocol_version_msg_length; i++) {
        printf("parse state: %d\n", p.parseByte(test.data[i]));
    }

    result |= verifyProtocolMessage(p.rxMessage, test.fields);

    return result;
}

struct test_profile_s
{
    uint8_t start1;
    uint8_t start2;
    uint16_t payload_length;
    uint16_t message_id;

    uint8_t source_device_id;
    uint8_t destination_device_id;

    uint32_t distance;
    uint16_t confidence;
    uint16_t transmit_duration;
    uint32_t ping_number;
    uint32_t scan_start;
    uint32_t scan_length;
    uint32_t gain_setting;
    uint16_t profile_data_length;
    uint8_t  profile_data[profile_points];
    uint16_t checksum;
}__attribute__((packed));

bool verifyProfileMessage(ping1d_profile m, test_profile_s t)
{
    bool result = false;
    result |= compare("payload_length", m.payload_length(), t.payload_length);
    result |= compare("message_id", m.message_id(), t.message_id);
    result |= compare("source_id", m.source_device_id(), t.source_device_id);
    result |= compare("destination_id", m.destination_device_id(), t.destination_device_id);
    result |= compare("distance", m.distance(), t.distance);
    result |= compare("confidence", m.confidence(), t.confidence);
    result |= compare("transmit_duration", m.transmit_duration(), t.transmit_duration);
    result |= compare("ping_number", m.ping_number(), t.ping_number);
    result |= compare("scan_start", m.scan_start(), t.scan_start);
    result |= compare("scan_length", m.scan_length(), t.scan_length);
    result |= compare("gain_setting", m.gain_setting(), t.gain_setting);
    result |= compare("profile_data_length", m.profile_data_length(), t.profile_data_length);
    for (uint16_t i = 0; i < profile_points; i++) {
        compare("compare data", m.profile_data()[i], t.profile_data[i]);
    }
    return result;
}

bool testProfile()
{
    printf("testing ping1d profile message...\n");

    bool result = false;

    // Hand-made test case
    union {
        test_profile_s fields {
            'B',
            'R',
            profile_payload_length,
            Ping1dId::PROFILE,
            56,
            45,
            1000,
            93,
            2111,
            44444444,
            4000,
            35000,
            4,
            profile_points,
            { 0,1,2,3,4,5,6,7,8,9 }, // profile data
            1502 // checksum
        };
        uint8_t data[profile_msg_length];
    } test;

    printf("Testing byte array to object conversion...\n\n");

    //TODO test all constructors
    //TODO test dynamic arrays, implicit and explicit length
    ping1d_profile test_in(test.data, profile_msg_length);

    result |= verifyProfileMessage(test_in, test.fields);

    printf("\nTesting object to byte array conversion...\n\n");

    ping1d_profile test_out(profile_points);
    test_out.set_source_device_id(test.fields.source_device_id);
    test_out.set_destination_device_id(test.fields.destination_device_id);
    test_out.set_distance(test.fields.distance);
    test_out.set_confidence(test.fields.confidence);
    test_out.set_transmit_duration(test.fields.transmit_duration);
    test_out.set_ping_number(test.fields.ping_number);
    test_out.set_scan_start(test.fields.scan_start);
    test_out.set_scan_length(test.fields.scan_length);
    test_out.set_gain_setting(test.fields.gain_setting);
    test_out.set_profile_data_length(test.fields.profile_data_length);
    for (uint16_t i = 0; i < profile_points; i++) {
        test_out.set_profile_data_at(i, test.fields.profile_data[i]);
    }

    test_out.updateChecksum();

    for (uint32_t i = 0; i < profile_msg_length; i++) {
        result |= compare("checking output byte", test_out.msgData[i], test.data[i]);
    }

    printf("\ntesting parser...\n\n");

    PingParser p(512); // parser buffer length must be large enough
    for(uint32_t i = 0; i < profile_msg_length; i++) {
        printf("parse state: %d\n", p.parseByte(test.data[i]));
    }

    result |= verifyProfileMessage(p.rxMessage, test.fields);

    return result;
}
