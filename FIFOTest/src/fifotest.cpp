#include "../../libraries/fifo/fifo.h"
#include <iostream>

int main(int argc, char const *argv[])
{
  //
  // Note: Run this program in sudo! Otherwise opening FIFO will fail.
  //

  std::string message = "Hello";
  std::string message2 = "World";

  FIFOWriter fw = FIFOWriter("/dev/fifotest");

  // This message will disappear by calling flush()
  fw.write_bytes(message.c_str(),message.size());

  FIFOReader fr = FIFOReader("/dev/fifotest");
  fr.flush();

  // This message will stay
  fw.write_bytes(message2.c_str(),message2.size());

  while(fr.available()){
    std::cout << "new byte: " << fr.read_byte() << std::endl;
  }

  return 0;
}
