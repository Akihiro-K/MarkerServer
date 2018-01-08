#include "../../libraries/fifo/fifo.h"
#include <iostream>

#define FIFO_DATA_BUFFER_LENGTH (256)

struct FromMarker {
  uint32_t timestamp; // microseconds
  float position[3]; // meter
  float quaternion[3]; // x y z
  float r_var[3]; // meter^2
  uint8_t status; // 1 : detected, 0 : not detected
} __attribute__((packed));

struct FromMarker from_marker_send = {1, {2, 3, 4}, {5, 6, 7}, {8, 9, 10}, 11};
struct FromMarker from_marker_recv;

void FromMarkerHandler(uint8_t component_id, uint8_t message_id, const uint8_t * data_buffer, size_t len);

int main(int argc, char const *argv[])
{
  //
  // Note: Run this program in sudo! Otherwise opening FIFO will fail.
  //

  UTFIFOWriter fw = UTFIFOWriter("/dev/marker_fifo");

  struct FromMarker * ptr = &from_marker_send;
  fw.send_data((const char *)ptr,sizeof(from_marker_send));

  UTFIFOReader fr = UTFIFOReader("/dev/marker_fifo");
  fr.recv_data(FromMarkerHandler);

  std::cout << "timestamp: " << unsigned(from_marker_recv.timestamp) << std::endl;
  std::cout << "position x: " << from_marker_recv.position[0] << std::endl;
  std::cout << "position y: " << from_marker_recv.position[1] << std::endl;
  std::cout << "position z: " << from_marker_recv.position[2] << std::endl;
  std::cout << "quaternion x: " << from_marker_recv.quaternion[0] << std::endl;
  std::cout << "quaternion y: " << from_marker_recv.quaternion[1] << std::endl;
  std::cout << "quaternion z: " << from_marker_recv.quaternion[2] << std::endl;
  std::cout << "r_var x: " << from_marker_recv.r_var[0] << std::endl;
  std::cout << "r_var y: " << from_marker_recv.r_var[1] << std::endl;
  std::cout << "r_var z: " << from_marker_recv.r_var[2] << std::endl;
  std::cout << "status: " << unsigned(from_marker_recv.status) << std::endl;
  return 0;
}


void FromMarkerHandler(uint8_t component_id, uint8_t message_id, const uint8_t * data_buffer, size_t len)
{
  uint8_t temp[FIFO_DATA_BUFFER_LENGTH];
  std::memcpy(temp, data_buffer, len);
  FromMarker temp_s;

  struct FromMarker * struct_ptr = (struct FromMarker *)temp;
  temp_s.timestamp = struct_ptr->timestamp;
  for (int i = 0; i < 3; i++) {
    temp_s.position[i] = struct_ptr->position[i];
    temp_s.quaternion[i] = struct_ptr->quaternion[i];
    temp_s.r_var[i] = struct_ptr->r_var[i];
  }
  temp_s.status = struct_ptr->status;

  from_marker_recv = temp_s;
}
