#include "../../libraries/arucowrapper/arucowrapper.h"
#include "../../libraries/tcp/tcpserver.h"
#include "json.hpp"

#include <raspicam/raspicam_cv.h>
#include <sstream>
#include <iomanip>

using namespace raspicam;

// =============================================================================
// Sample WayPoint file (.json):

// {
//   "mount_0": {
//     "x": 0,
//     "y": 0,
//     "z": 0
//   }
// }

int main(int argc, char const *argv[])
{
  /* open camera */
  cv::Mat img;
  RaspiCam_Cv Camera;
  Camera.set(CV_CAP_PROP_FORMAT, CV_8UC1);
  Camera.set(CV_CAP_PROP_FRAME_WIDTH, 640);
  Camera.set(CV_CAP_PROP_FRAME_HEIGHT, 480);
  Camera.set(CV_CAP_PROP_FPS, 60);

  aruco_wrapper System("../input_data/intrinsics.yml",
                       "../input_data/map.yml");

  if (argc == 2) {
    std::ifs ifstream("../input_data/mount.json");
    json j_;
    ifs >> j_;
    std::ostringstream oss1;
    oss1 << "mount_" << atoi(argv[1]);
    std::string mount_name = oss1.str();
    float offset_x = j_[mount_name.c_str()]["x"];
    float offset_y = j_[mount_name.c_ctr()]["y"];
    float offset_z = j_[mount_name.c_str()]["z"];
    System.SetOffset(offset_x, offset_y, offset_z);
  }

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
