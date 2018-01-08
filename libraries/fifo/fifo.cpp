#include "fifo.h"

//=============================================================================
// Public functions:

FIFOWriter::FIFOWriter(std::string name)
{
  // remove fifo if it already exists
  if(file_exist(name)){
    std::remove(name.c_str());
  }

  if(mkfifo(name.c_str(), 0666)==-1){
    perror("mkfifo");
  }

  // reader must open before writer, so create a dummy
  int dummy = open(name.c_str(), O_RDONLY | O_NONBLOCK);
  if((fd=open("/dev/fifotest",O_WRONLY | O_NONBLOCK))==-1){
    perror("open (FIFOWriter)");
    exit(-1);
  }
}

void FIFOWriter::write_byte(char c)
{
  write(fd, &c, 1);
}

void FIFOWriter::write_bytes(const char * src, size_t len)
{
  const char * ptr = src;
  for(int i = 0; i < len; i++){
    write_byte(*ptr++);
  }
}

FIFOReader::FIFOReader(std::string name)
{
  if((fd = open(name.c_str(), O_RDONLY | O_NONBLOCK))==-1){
    perror("open (FIFOReader)");
    exit(-1);
  }
  read_byte_called = true;
}

bool FIFOReader::available(){
  // return true only if new byte is available
  if(read_byte_called){
    get_new_byte();
  }
  return new_byte_available;
}

char FIFOReader::read_byte(){
  read_byte_called = true;
  return c;
}

void FIFOReader::flush(){
  while(available()){
    read_byte();
  }
}

//=============================================================================
// Private functions:
bool FIFOWriter::file_exist(const std::string& name)
{
    struct stat buffer;
    return (stat (name.c_str(), &buffer) == 0);
}

void FIFOReader::get_new_byte()
{
  // reads fifo and ...
  // 1. stores a byte into "c"
  // 2. updates "new_byte_available" flag

  int r = (int) read(fd, &c, 1);

  if(r > 0){
    new_byte_available = true;
  }else{
    new_byte_available = false;
  }
}
