#include <ping-device-ping1d.h>
#include <ping-message-common.h>
#include <ping-message-ping1d.h>

Ping1d::~Ping1d()
{
  if (profile_data) {
   free(profile_data);
  }
}

bool Ping1d::initialize(uint16_t pingIntervalMs)
{
  if (!PingDevice::initialize()) {
    return false;
  }

  // Configure ping interval
  if (!set_ping_interval(pingIntervalMs)) {
    return false;
  }

  return true;
}

void Ping1d::_handleMessage(const ping_message* message)
{
  switch (message->message_id()) {
    case Ping1dId::DISTANCE:
    {
      const ping1d_distance* msg = static_cast<const ping1d_distance*>(message);
      distance = msg->distance();
      confidence = msg->confidence();
      transmit_duration = msg->transmit_duration();
      ping_number = msg->ping_number();
      scan_start = msg->scan_start();
      scan_length = msg->scan_length();
      gain_setting = msg->gain_setting();
    }
      break;
    case Ping1dId::DISTANCE_SIMPLE:
    {
      const ping1d_distance_simple* msg = static_cast<const ping1d_distance_simple*>(message);
      distance = msg->distance();
      confidence = msg->confidence();
    }
      break;
    case Ping1dId::FIRMWARE_VERSION:
    {
      const ping1d_firmware_version* msg = static_cast<const ping1d_firmware_version*>(message);
      device_type = msg->device_type();
      device_model = msg->device_model();
      firmware_version_major = msg->firmware_version_major();
      firmware_version_minor = msg->firmware_version_minor();
    }
      break;
    case Ping1dId::GAIN_SETTING:
    {
      const ping1d_gain_setting* msg = static_cast<const ping1d_gain_setting*>(message);
      gain_setting = msg->gain_setting();
    }
      break;
    case Ping1dId::GENERAL_INFO:
    {
      const ping1d_general_info* msg = static_cast<const ping1d_general_info*>(message);
      firmware_version_major = msg->firmware_version_major();
      firmware_version_minor = msg->firmware_version_minor();
      voltage_5 = msg->voltage_5();
      ping_interval = msg->ping_interval();
      gain_setting = msg->gain_setting();
      mode_auto = msg->mode_auto();
    }
      break;
    case Ping1dId::MODE_AUTO:
    {
      const ping1d_mode_auto* msg = static_cast<const ping1d_mode_auto*>(message);
      mode_auto = msg->mode_auto();
    }
      break;
    case Ping1dId::PCB_TEMPERATURE:
    {
      const ping1d_pcb_temperature* msg = static_cast<const ping1d_pcb_temperature*>(message);
      pcb_temperature = msg->pcb_temperature();
    }
      break;
    case Ping1dId::PING_ENABLE:
    {
      const ping1d_ping_enable* msg = static_cast<const ping1d_ping_enable*>(message);
      ping_enabled = msg->ping_enabled();
    }
      break;
    case Ping1dId::PING_INTERVAL:
    {
      const ping1d_ping_interval* msg = static_cast<const ping1d_ping_interval*>(message);
      ping_interval = msg->ping_interval();
    }
      break;
    case Ping1dId::PROCESSOR_TEMPERATURE:
    {
      const ping1d_processor_temperature* msg = static_cast<const ping1d_processor_temperature*>(message);
      processor_temperature = msg->processor_temperature();
    }
      break;
    case Ping1dId::PROFILE:
    {
      const ping1d_profile* msg = static_cast<const ping1d_profile*>(message);
      distance = msg->distance();
      confidence = msg->confidence();
      transmit_duration = msg->transmit_duration();
      ping_number = msg->ping_number();
      scan_start = msg->scan_start();
      scan_length = msg->scan_length();
      gain_setting = msg->gain_setting();
      if (msg->profile_data_length() > profile_data_length) {
        if (profile_data) {
           free(profile_data);
        }
        profile_data = (uint8_t*)malloc(msg->profile_data_length() * sizeof(uint8_t));
      }

      profile_data_length = msg->profile_data_length();

      for (uint16_t i = 0; i < profile_data_length; i++) {
        profile_data[i] = msg->profile_data()[i];
      }
    }
      break;
    case Ping1dId::RANGE:
    {
      const ping1d_range* msg = static_cast<const ping1d_range*>(message);
      scan_start = msg->scan_start();
      scan_length = msg->scan_length();
    }
      break;
    case Ping1dId::SPEED_OF_SOUND:
    {
      const ping1d_speed_of_sound* msg = static_cast<const ping1d_speed_of_sound*>(message);
      speed_of_sound = msg->speed_of_sound();
    }
      break;
    case Ping1dId::TRANSMIT_DURATION:
    {
      const ping1d_transmit_duration* msg = static_cast<const ping1d_transmit_duration*>(message);
      transmit_duration = msg->transmit_duration();
    }
      break;
    case Ping1dId::VOLTAGE_5:
    {
      const ping1d_voltage_5* msg = static_cast<const ping1d_voltage_5*>(message);
      voltage_5 = msg->voltage_5();
    }
      break;

    default:
      break;
  }

  PingDevice::_handleMessage(message);
}

bool Ping1d::set_device_id(uint8_t _device_id, bool verify)
{
  ping1d_set_device_id msg;
  msg.set_device_id(_device_id);
  writeMessage(msg);
  if (!request(Ping1dId::DEVICE_ID)) {
    return false; // no reply from device
  }
  // Read back the data and check that changes have been applied
  if (verify
    && (device_id != _device_id)) {
    return false; // device reply does not match
  }
  return true; // success
}

bool Ping1d::set_gain_setting(uint8_t _gain_setting, bool verify)
{
  ping1d_set_gain_setting msg;
  msg.set_gain_setting(_gain_setting);
  writeMessage(msg);
  if (!request(Ping1dId::GAIN_SETTING)) {
    return false; // no reply from device
  }
  // Read back the data and check that changes have been applied
  if (verify
    && (gain_setting != _gain_setting)) {
    return false; // device reply does not match
  }
  return true; // success
}

bool Ping1d::set_mode_auto(uint8_t _mode_auto, bool verify)
{
  ping1d_set_mode_auto msg;
  msg.set_mode_auto(_mode_auto);
  writeMessage(msg);
  if (!request(Ping1dId::MODE_AUTO)) {
    return false; // no reply from device
  }
  // Read back the data and check that changes have been applied
  if (verify
    && (mode_auto != _mode_auto)) {
    return false; // device reply does not match
  }
  return true; // success
}

bool Ping1d::set_ping_enable(uint8_t _ping_enabled, bool verify)
{
  ping1d_set_ping_enable msg;
  msg.set_ping_enabled(_ping_enabled);
  writeMessage(msg);
  if (!request(Ping1dId::PING_ENABLE)) {
    return false; // no reply from device
  }
  // Read back the data and check that changes have been applied
  if (verify
    && (ping_enabled != _ping_enabled)) {
    return false; // device reply does not match
  }
  return true; // success
}

bool Ping1d::set_ping_interval(uint16_t _ping_interval, bool verify)
{
  ping1d_set_ping_interval msg;
  msg.set_ping_interval(_ping_interval);
  writeMessage(msg);
  if (!request(Ping1dId::PING_INTERVAL)) {
    return false; // no reply from device
  }
  // Read back the data and check that changes have been applied
  if (verify
    && (ping_interval != _ping_interval)) {
    return false; // device reply does not match
  }
  return true; // success
}

bool Ping1d::set_range(uint32_t _scan_start, uint32_t _scan_length, bool verify)
{
  ping1d_set_range msg;
  msg.set_scan_start(_scan_start);
  msg.set_scan_length(_scan_length);
  writeMessage(msg);
  if (!request(Ping1dId::RANGE)) {
    return false; // no reply from device
  }
  // Read back the data and check that changes have been applied
  if (verify
    && (scan_start != _scan_start
        || scan_length != _scan_length)) {
    return false; // device reply does not match
  }
  return true; // success
}

bool Ping1d::set_speed_of_sound(uint32_t _speed_of_sound, bool verify)
{
  ping1d_set_speed_of_sound msg;
  msg.set_speed_of_sound(_speed_of_sound);
  writeMessage(msg);
  if (!request(Ping1dId::SPEED_OF_SOUND)) {
    return false; // no reply from device
  }
  // Read back the data and check that changes have been applied
  if (verify
    && (speed_of_sound != _speed_of_sound)) {
    return false; // device reply does not match
  }
  return true; // success
}

