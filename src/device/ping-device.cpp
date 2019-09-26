#include <ping-device.h>

#include <ping-message-common.h>
#include <ping-message.h>
#include <ping-time.h>

bool PingDevice::initialize() {
  if (!request(CommonId::PROTOCOL_VERSION)) {
    return false;
  }

  if (!request(CommonId::DEVICE_INFORMATION)) {
    return false;
  }

  return true;
}

ping_message* PingDevice::read() {
  char b;
  int result = _port.read(&b, 1);
  if (result > 0) {
    if (_parser.parseByte((char)b) == PingParser::NEW_MESSAGE) {
      return &_parser.rxMessage;
    }
  }
  return nullptr;
}

ping_message* PingDevice::request(uint16_t id, int timeoutMs) {
  common_general_request msg;
  msg.set_requested_id(id);
  writeMessage(msg);
  return waitMessage(id, timeoutMs);
}

ping_message* PingDevice::waitMessage(uint16_t id, int timeoutMs) {
  int tstart = PingTime::timeMs();
  while (PingTime::timeMs() < tstart + timeoutMs) {
    ping_message* msg = read();

    if (!msg) {
      continue;
    }

    _handleMessage(msg);

    if (msg->message_id() == CommonId::NACK) {
      return nullptr;
    }

    if (msg->message_id() == id) {
      return msg;
    }
  }

  return nullptr;
}

int PingDevice::write(const char* data, int nBytes) { return _port.write(data, nBytes); }

void PingDevice::writeMessage(ping_message& message) {
  message.updateChecksum();
  write((char*)message.msgData, message.msgDataLength());
}

void PingDevice::_handleMessage(ping_message* message) {
  device_id = message->source_device_id();

  switch (message->message_id()) {
  case CommonId::NACK: {
    break;
  }
  case CommonId::PROTOCOL_VERSION: {
    common_protocol_version* msg = static_cast<common_protocol_version*>(message);
    version_major                = msg->version_major();
    version_minor                = msg->version_minor();
    version_patch                = msg->version_patch();
    break;
  }
  case CommonId::DEVICE_INFORMATION: {
    common_device_information* msg = static_cast<common_device_information*>(message);
    device_type                    = msg->device_type();
    device_revision                = msg->device_revision();
    firmware_version_major         = msg->firmware_version_major();
    firmware_version_minor         = msg->firmware_version_minor();
    firmware_version_patch         = msg->firmware_version_patch();
    break;
  }
  default:
    break;
  }
}
