#include "../../libraries/mymarker/mymarker.h"
#include "../../libraries/mytcp/tcpserver.h"

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
  Camera.set(CV_CAP_PROP_FPS, 60);

  aruco_wrapper System("../input_data/intrinsics2.yml", 
                       "../input_data/map_2");

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
