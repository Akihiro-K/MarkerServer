#include "../../libraries/arucowrapper/arucowrapper.h"
#include "../../libraries/utserial/utserial.h"
#include "json.hpp"

#include <raspicam/raspicam_cv.h>
#include <sstream>
#include <iomanip>

using namespace raspicam;
using json = nlohmann::json;

#define SERIAL_BAUDRATE_MKR (57600)
#define UT_SERIAL_COMPONENT_ID_RASPI (2)

const char SERIAL_PORT_MKR[] = "/dev/ttyAMA0";

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
    std::ifstream ifs("../input_data/mount.json");
    json j_;
    ifs >> j_;
    std::ostringstream oss1;
    oss1 << "mount_" << atoi(argv[1]);
    std::string mount_name = oss1.str();
    float offset_x = j_[mount_name.c_str()]["x"];
    float offset_y = j_[mount_name.c_str()]["y"];
    float offset_z = j_[mount_name.c_str()]["z"];
    System.SetOffset(offset_x, offset_y, offset_z);
  }

  ut_serial FC_comm(SERIAL_PORT_MKR, SERIAL_BAUDRATE_MKR);

  if (Camera.open()) {
    std::cout << "Capture is opened" << std::endl;
    for(;;){
      Camera.grab();
      Camera.retrieve(img);
      if (System.MarkerUpdate(img)) {
        // display state when marker is detected
        System.Disp();
      }
      System.Logging();
      FC_comm.send_data(UT_SERIAL_COMPONENT_ID_RASPI, 1, (uint8_t *)System.Packet(), sizeof(*System.Packet()));
    }
  }
    return 0;
}
