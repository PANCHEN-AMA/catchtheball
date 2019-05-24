
#include <opencv2/opencv.hpp> /// include most of core headers
#include <opencv2/core.hpp> /// include most of core headers
#include <opencv2/imgproc.hpp> /// include GUI-related headers
#include <opencv2/highgui.hpp> /// include GUI-related headers

const float markerSize = 0.048182f;     // real marker size (m)

const int markerAreaMin = 10000;
const int markerAreaMax = 100000000;


/**
 * computes the pose of AR markers with specific code number
 * @param matrices result of AR marker poses at a openGL coordinate system.
 * @param img_bgr sorce image from camera.
 * @param codes the codes of AR marker.
 * @param codes_count the length of the "codes".
 */
void GetPoses(float matrices[][16], cv::Mat img_bgr, const int codes[], const int codes_count);
