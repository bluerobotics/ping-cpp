#include <ping-message-common.h>
#include "ping-message-ping360.h"
#include "ping-device-ping360.h"

Ping360::~Ping360()
{
    if (auto_device_data_data.data) {
        delete[] auto_device_data_data.data;
    }
    if (device_data_data.data) {
        delete[] device_data_data.data;
    }
}

bool Ping360::initialize()
{
    if (!PingDevice::initialize()) {
        return false;
    }

    return true;
}

void Ping360::_handleMessage(const ping_message* message)
{
    switch (message->message_id()) {
        case Ping360Id::AUTO_DEVICE_DATA:
        {
            const ping360_auto_device_data* message_auto_device_data = static_cast<const ping360_auto_device_data*>(message);
            auto_device_data_data.mode = message_auto_device_data->mode();
            auto_device_data_data.gain_setting = message_auto_device_data->gain_setting();
            auto_device_data_data.angle = message_auto_device_data->angle();
            auto_device_data_data.transmit_duration = message_auto_device_data->transmit_duration();
            auto_device_data_data.sample_period = message_auto_device_data->sample_period();
            auto_device_data_data.transmit_frequency = message_auto_device_data->transmit_frequency();
            auto_device_data_data.start_angle = message_auto_device_data->start_angle();
            auto_device_data_data.stop_angle = message_auto_device_data->stop_angle();
            auto_device_data_data.num_steps = message_auto_device_data->num_steps();
            auto_device_data_data.delay = message_auto_device_data->delay();
            auto_device_data_data.number_of_samples = message_auto_device_data->number_of_samples();
            if (message_auto_device_data->data_length() > auto_device_data_data.data_length) {
                if (auto_device_data_data.data) {
                    delete[] auto_device_data_data.data;
                }
                auto_device_data_data.data = new uint8_t[message_auto_device_data->data_length()];
            }

            auto_device_data_data.data_length = message_auto_device_data->data_length();
            memcpy(auto_device_data_data.data, message_auto_device_data->data(), message_auto_device_data->data_length());
        }
        break;
        case Ping360Id::DEVICE_DATA:
        {
            const ping360_device_data* message_device_data = static_cast<const ping360_device_data*>(message);
            device_data_data.mode = message_device_data->mode();
            device_data_data.gain_setting = message_device_data->gain_setting();
            device_data_data.angle = message_device_data->angle();
            device_data_data.transmit_duration = message_device_data->transmit_duration();
            device_data_data.sample_period = message_device_data->sample_period();
            device_data_data.transmit_frequency = message_device_data->transmit_frequency();
            device_data_data.number_of_samples = message_device_data->number_of_samples();
            if (message_device_data->data_length() > device_data_data.data_length) {
                if (device_data_data.data) {
                    delete[] device_data_data.data;
                }
                device_data_data.data = new uint8_t[message_device_data->data_length()];
            }

            device_data_data.data_length = message_device_data->data_length();
            memcpy(device_data_data.data, message_device_data->data(), message_device_data->data_length());
        }
        break;

        default:
            break;
    }

    PingDevice::_handleMessage(message);
}


void Ping360::set_auto_transmit(uint8_t _mode, uint8_t _gain_setting, uint16_t _transmit_duration, uint16_t _sample_period, uint16_t _transmit_frequency, uint16_t _number_of_samples, uint16_t _start_angle, uint16_t _stop_angle, uint8_t _num_steps, uint8_t _delay)
{
    ping360_auto_transmit message;
    message.set_mode(_mode);
    message.set_gain_setting(_gain_setting);
    message.set_transmit_duration(_transmit_duration);
    message.set_sample_period(_sample_period);
    message.set_transmit_frequency(_transmit_frequency);
    message.set_number_of_samples(_number_of_samples);
    message.set_start_angle(_start_angle);
    message.set_stop_angle(_stop_angle);
    message.set_num_steps(_num_steps);
    message.set_delay(_delay);
    writeMessage(message);
}

void Ping360::set_motor_off()
{
    ping360_motor_off message;
    writeMessage(message);
}

void Ping360::set_reset(uint8_t _bootloader, uint8_t _reserved)
{
    ping360_reset message;
    message.set_bootloader(_bootloader);
    message.set_reserved(_reserved);
    writeMessage(message);
}

void Ping360::set_transducer(uint8_t _mode, uint8_t _gain_setting, uint16_t _angle, uint16_t _transmit_duration, uint16_t _sample_period, uint16_t _transmit_frequency, uint16_t _number_of_samples, uint8_t _transmit, uint8_t _reserved)
{
    ping360_transducer message;
    message.set_mode(_mode);
    message.set_gain_setting(_gain_setting);
    message.set_angle(_angle);
    message.set_transmit_duration(_transmit_duration);
    message.set_sample_period(_sample_period);
    message.set_transmit_frequency(_transmit_frequency);
    message.set_number_of_samples(_number_of_samples);
    message.set_transmit(_transmit);
    message.set_reserved(_reserved);
    writeMessage(message);
}

