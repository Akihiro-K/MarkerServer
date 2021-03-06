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

//  aruco_wrapper System("../input_data/intrinsics2.yml",
//                       "../input_data/map_2.yml");
  aruco_wrapper System("../input_data/intrinsics3.yml",
                       "../input_data/map_3.yml");
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
      s.send_data((const char *)System.Packet(), sizeof(*System.Packet()));
    }
  }
    return 0;
}
