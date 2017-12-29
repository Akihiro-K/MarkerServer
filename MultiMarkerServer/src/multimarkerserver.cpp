#include "../../libraries/arucowrapper/arucowrapper.h"
#include "../../libraries/tcp/tcpserver.h"
#include <raspicam/raspicam_cv.h>

#define TCP_PORT_MARKER (8080)
#define TCP_PORT_PAYLOAD (8081)

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
  tcp_server s1; // server for sending position relative to marker
  tcp_server s2; // server for sending position relative to payload

  // accept connection
  s1.start_listen(TCP_PORT_MARKER);
  s1.start_accept();
  s2.start_listen(TCP_PORT_PAYLOAD);
  s2.start_accept();

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
      s1.send_data((const char *)System.Packet(0), sizeof(*System.Packet(0)));
      s2.send_data((const char *)System.Packet(1), sizeof(*System.Packet(1)));
    }
  }
    return 0;
}
