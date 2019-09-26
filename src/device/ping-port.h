#pragma once

// port interface used by PingDevice class
class PingPort {
public:
  virtual int read(char* buffer, int nBytes)      = 0;
  virtual int write(const char* data, int nBytes) = 0;
};
