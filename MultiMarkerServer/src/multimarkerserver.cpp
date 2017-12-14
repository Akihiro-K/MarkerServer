#include "../../libraries/arucowrapper/arucowrapper.h"
#include "../../libraries/tcp/tcpserver.h"

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
    "../input_data/intrinsics.yml";
  std::vector<string> marker_map_paths = {
    "../input_data/droneport.yml",
    "../input_data/payload.yml"};
  multi_aruco_wrapper System(camera_parameter_path,marker_map_paths);
  tcp_server s;

  // accept connection
  s.start_listen(8080);
  s.start_accept();

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
      s.send_data((const char *)System.Packet(0), sizeof(*System.Packet(0)));
    }
  }
    return 0;
}
