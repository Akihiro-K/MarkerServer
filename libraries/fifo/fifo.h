#ifndef FIFO_H_
#define FIFO_H_

#include <iostream>
#include <stdio.h>
#include <unistd.h> //Used for UART
#include <fcntl.h> //Used for UART
#include <termios.h> //Used for UART
#include <sys/ioctl.h> //Used for UART
#include <sys/stat.h> //Used for FIFO

class FIFOWriter
{
private:
  int fd;
  bool file_exist(const std::string& name);
public:
  FIFOWriter(std::string name); // "/dev/gps_fifo"
  ~FIFOWriter() {};
  void write_byte(char c);
  void write_bytes(const char * src, size_t len);
};

class FIFOReader
{
private:
  int fd;
  bool new_byte_available;
  bool read_byte_called;
  char c;
  void get_new_byte();
public:
  FIFOReader(std::string name); // "/dev/gps_fifo"
  ~FIFOReader() {};
  bool available(); // check if new byte available
  char read_byte(); // get next byte
  void flush(); // flush all bytes in fifo
};

#endif // FIFO_H_
