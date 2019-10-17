#include <ping-device.h>

#include <ping-message-common.h>
#include <ping-message.h>
#include <ping-time.h>

bool PingDevice::initialize() {
    return request(CommonId::PROTOCOL_VERSION)
        && request(CommonId::DEVICE_INFORMATION);
}

ping_message* PingDevice::read() {
    char b;
    int result = _port.read(&b, 1);
    if (result > 0) {
        if (_parser.parseByte(static_cast<char>(b)) == PingParser::NEW_MESSAGE) {
        return &_parser.rxMessage;
        }
    }
    return nullptr;
}

ping_message* PingDevice::request(uint16_t id, int timeoutMs) {
    common_general_request msg;
    msg.set_requested_id(id);
    writeMessage(msg);
    ping_message* reply = waitMessage(id, timeoutMs);
    if (reply && reply->message_id() == CommonId::NACK) {
        common_nack* m = static_cast<common_nack*>(reply);
        if (m->nacked_id() != CommonId::GENERAL_REQUEST) {
        return nullptr;
        }
    }

    return reply;
}

ping_message* PingDevice::waitMessage(uint16_t id, int timeoutMs) {
    int tstart = PingTime::timeMs();
    while (PingTime::timeMs() < tstart + timeoutMs) {
        // TODO make this totally nonblocking
        // this will block up to 0.1s for data to be available
        ping_message* msg = read();

        if (!msg) {
        continue;
        }

        _handleMessage(msg);

        if (msg->message_id() == id || msg->message_id() == CommonId::NACK) {
        return msg;
        }
        // sleep for 1ms to prevent cpu spinlock
        PingTime::microsecondDelay(1000);
    }

    return nullptr;
}

int PingDevice::write(const char* data, int nBytes) { return _port.write(data, nBytes); }

void PingDevice::writeMessage(ping_message& message) {
    message.updateChecksum();
    write(reinterpret_cast<char*>(message.msgData), message.msgDataLength());
}

void PingDevice::_handleMessage(const ping_message* message) {
    device_id = message->source_device_id();

    switch (message->message_id()) {
    case CommonId::NACK: {
        break;
    }
    case CommonId::PROTOCOL_VERSION: {
        const common_protocol_version* msg = static_cast<const common_protocol_version*>(message);
        version_major = msg->version_major();
        version_minor = msg->version_minor();
        version_patch = msg->version_patch();
        break;
    }
    case CommonId::DEVICE_INFORMATION: {
        const common_device_information* msg = static_cast<const common_device_information*>(message);
        device_type = msg->device_type();
        device_revision = msg->device_revision();
        firmware_version_major = msg->firmware_version_major();
        firmware_version_minor = msg->firmware_version_minor();
        firmware_version_patch = msg->firmware_version_patch();
        break;
    }
    default:
        break;
    }
}
