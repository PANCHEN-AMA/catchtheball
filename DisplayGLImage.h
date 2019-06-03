
#include <opencv2/opencv.hpp> /// include most of core headers
#include <opencv2/core.hpp> /// include most of core headers

#define GLFW_INCLUDE_GLU // add support for GLU with GLFW
//#include <GL/glew.h> // if necessary, include before the glfw header
#include <GLFW/glfw3.h> /// this also includes other openGL headers


const int virtual_camera_angle = 80;    // near to the real camera angle
/**
 * Initialize open GL window
 */
void initGL();


/**
 * render window image with openGL
 * @param window displaying window
 * @param img_bgr background image from camera.
 */
void display (GLFWwindow* window, const cv::Mat &img_bgr, float poseMatrices[][16], int codes_count, int width_gl, int height_gl, int& starnum);

void InitBall();