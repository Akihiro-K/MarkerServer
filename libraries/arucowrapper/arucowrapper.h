#ifndef MYMARKER_H_
#define MYMARKER_H_

#include <aruco/aruco.h>
#include <aruco/cvdrawingutils.h>

#include <opencv2/core.hpp>
#include <opencv2/opencv.hpp>

#include <iostream>
#include <fstream>
#include <cstring>
#include <sys/time.h>

using namespace std;
using namespace cv;
using namespace aruco;

/* marker packet */
struct Packet {
  uint32_t timestamp; // microseconds
  float position[3]; // meter
  float quaternion[3]; // x y z
  float r_var[3]; // meter^2
  uint8_t status; // 1 : detected, 0 : not detected
} __attribute__((packed));

class aruco_wrapper
{
private:
  aruco::MarkerMap MM;
  aruco:CameraParameters CP;
  aruco::MarkerMapPoseTracker MMPT;
  struct timeval tv;
  struct Packet packet;
  float offset_x_; // [m]
  float offset_y_; // [m]
  float offset_z_; // [m]
  void getpos(cv::Mat Rvec, cv::Mat Tvec, float position[3]);
  void geterr_(aruco::Marker marker, cv::Mat Rvec, cv::Mat Tvec, float err_[3]);
  void geterr(vector<aruco::Marker> v_m, cv::Mat Rvec, cv::Mat Tvec, float err[3]);
  void getquaternion(cv::Mat Rvec, float quaternion[3]);
  bool checkmarker(vector<aruco::Marker> v_m);
  bool checkpos(float position[3], float dt);
public:
  aruco_wrapper(string pathforCP, string pathforMM);
  ~aruco_wrapper() {};
  void SetCameraParameters(string filepath);
  void SetMarkerMap(string filepath);
  void SetMarkerMapPoseTracker();
  void SetOffset(float offset_x, float offset_y, float offset_z);
  bool MarkerUpdate(cv::Mat img);
  void Disp();
  void Logging();
  struct Packet* Packet();
};

#endif // MYMARKER_H_