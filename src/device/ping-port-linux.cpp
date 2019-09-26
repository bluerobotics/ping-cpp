// Adopted from original example here:
// https://blog.mbedded.ninja/programming/operating-systems/linux/linux-serial-ports-using-c-cpp/#overview
#include <ping-port-linux.h>

#include <cerrno> // Error integer and strerror() function
#include <cstdio>
#include <cstring>
#include <fcntl.h>   // Contains file controls like O_RDWR
#include <termios.h> // Contains POSIX terminal control definitions
#include <unistd.h>  // write(), read(), close()

PingPortLinux::PingPortLinux(const char* filename) {
  strcpy(_filename, filename);
  _handle = std::fopen(_filename, "a+");
  setBaudrate(B115200);
}

PingPortLinux::~PingPortLinux() {
  if (_handle) {
    std::fclose(_handle);
  }
}

int PingPortLinux::read(char* data, int nBytes) {
  int result = std::fread(data, 1, nBytes, _handle);

  if (result == 0) {
    std::fclose(_handle);
    _handle = std::fopen(_filename, "a+");
  }
  return result;
}

void PingPortLinux::sendBreak(int milliseconds) { tcsendbreak(fileno(_handle), 0); }

bool PingPortLinux::setBaudrate(int baudrate) {
  // Create new termios struc, we call it 'tty' for convention
  struct termios tty;
  memset(&tty, 0, sizeof tty);

  // Read in existing settings, and handle any error
  if (tcgetattr(fileno(_handle), &tty) != 0) {
    printf("Error %i from tcgetattr: %s\n", errno, strerror(errno));
  }

  tty.c_cflag &= ~PARENB;        // Clear parity bit, disabling parity (most common)
  tty.c_cflag &= ~CSTOPB;        // Clear stop field, only one stop bit used in
                                 // communication (most common)
  tty.c_cflag |= CS8;            // 8 bits per byte (most common)
  tty.c_cflag &= ~CRTSCTS;       // Disable RTS/CTS hardware flow control (most common)
  tty.c_cflag |= CREAD | CLOCAL; // Turn on READ & ignore ctrl lines (CLOCAL = 1)

  tty.c_oflag &= ~OPOST; // Prevent special interpretation of output bytes (e.g.
                         // newline chars)
  tty.c_oflag &= ~ONLCR; // Prevent conversion of newline to carriage return/line feed

  tty.c_lflag = 0;

  tty.c_iflag = 0;
  tty.c_iflag &= (IGNBRK | IGNPAR | INPCK);

  tty.c_cc[VTIME] = 1; // Wait for up to 0.1s, returning as soon as any data is received.
  tty.c_cc[VMIN]  = 0;

  // Set in/out baud rate
  cfsetispeed(&tty, baudrate);
  cfsetospeed(&tty, baudrate);

  // Save tty settings, also checking for error
  if (tcsetattr(fileno(_handle), TCSANOW, &tty) != 0) {
    printf("Error %i from tcsetattr: %s\n", errno, strerror(errno));
    return false;
  }

  return true;
}

int PingPortLinux::write(const char* data, int nBytes) {
  int bytes = std::fwrite(data, 1, nBytes, _handle);
  std::fflush(_handle);
  return bytes;
}
