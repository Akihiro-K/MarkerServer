//#include "../../libraries/tcp/tcpclient.h"
#include "../../libraries/fifo/fifo.h"

#include <iostream>

using namespace std;

struct FromMarker {
  uint32_t timestamp; // microseconds
  float position[3]; // meter
  float quaternion[3]; // x y z
  float r_var[3]; // meter^2
  uint8_t status; // 1 : detected, 0 : not detected
} __attribute__((packed));

struct FromMarker from_marker = {0, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, 0};

//void MarkerHandler(const char * src, size_t len);
void MarkerHandler(uint8_t component_id, uint8_t message_id, const uint8_t * src, size_t len);

int new_data_flag = 0;  

int main(int argc, char const *argv[])
{
  //tcp_client c;
  //c.start_connect("127.0.0.1" , 8080);
  UTFIFOReader fr = UTFIFOReader("/dev/marker_fifo");
  fr.flush();
  
  for(;;){
    // at 10 ~ 15HZ
    //c.recv_data(MarkerHandler);
    fr.recv_data(MarkerHandler);
    if(new_data_flag){
      cout << from_marker.position[0] << "\t" << from_marker.position[1] << "\t" << from_marker.position[2] << endl;
      new_data_flag = 0;
    }
  }
}

void MarkerHandler(uint8_t component_id, uint8_t message_id, const uint8_t * src, size_t len)
{
  char temp[FIFO_DATA_BUFFER_LENGTH];
  memcpy(temp, src, len);
  struct FromMarker * struct_ptr = (struct FromMarker *)temp;

  from_marker.timestamp = struct_ptr->timestamp;
  from_marker.status = struct_ptr->status;

  for (int i=0;i<3;i++){
    from_marker.position[i] = struct_ptr->position[i];
    from_marker.quaternion[i] = struct_ptr->quaternion[i];
    from_marker.r_var[i] = struct_ptr->r_var[i];
  }
  
  new_data_flag = 1;
}
