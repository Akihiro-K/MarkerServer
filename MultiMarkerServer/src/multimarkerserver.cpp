#include "../../libraries/arucowrapper/arucowrapper.h"
#include "../../libraries/fifo/fifo.h"
#include <raspicam/raspicam_cv.h>

using namespace raspicam;

int main(int argc, char const *argv[])
{
  /* open camera */
  Mat img;
  RaspiCam_Cv Camera;
  Camera.set(CV_CAP_PROP_FORMAT, CV_8UC1);
  Camera.set(CV_CAP_PROP_FRAME_WIDTH, 640);
  Camera.set(CV_CAP_PROP_FRAME_HEIGHT, 480);
  Camera.set(CV_CAP_PROP_FPS, 32);

  string camera_parameter_path =
    "../input_data/camera_module_v2_640x480.yml";
  std::vector<string> marker_map_paths = {
    "../input_data/droneport.yml",
    "../input_data/payload.yml"};
  multi_aruco_wrapper System(camera_parameter_path,marker_map_paths);
  UTFIFOWriter fw_marker = UTFIFOWriter("/dev/marker_fifo");
  UTFIFOWriter fw_payload = UTFIFOWriter("/dev/payload_fifo");

  if (Camera.open()) {
    cout << "Capture is opened" << endl;
    for(;;){
      Camera.grab();
      Camera.retrieve(img);
      if (System.MarkerUpdate(img)) {
        // display state when marker is detected
        System.Disp();
      }
      System.Logging();
      if(System.Packet(0)->status == 1){
        fw_marker.send_data((const char *)System.Packet(0), sizeof(*System.Packet(0)));
      }
      if(System.Packet(1)->status == 1){
        fw_payload.send_data((const char *)System.Packet(1), sizeof(*System.Packet(1)));
      }
    }
  }
    return 0;
}
