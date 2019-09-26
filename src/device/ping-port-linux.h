#pragma once

#include <ping-port.h>

#include <cstdio>

class PingPortLinux : public PingPort {
public:
  PingPortLinux(const char* filename);
  ~PingPortLinux();

  virtual int read(char* buffer, int nBytes) override final;
  void sendBreak(int milliseconds);
  bool setBaudrate(int baudrate);
  virtual int write(const char* data, int nBytes) override final;

private:
  char _filename[256];
  std::FILE* _handle;
};
