#include <iostream>
#include <stdlib.h>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include "MarkerTracker.h"
#include "DisplayGLImage.h"

const int codes_count = 2;
const int codes[codes_count] = {0x0690, 0x1C44};    // Marker codes
float poseMatrices[codes_count][16] = {NULL, NULL};        // Marker pose matrix
const int camera_width = 640, camera_height = 480;   // real camera pixel
extern int starnum = 0;
void displayscore(int starnum, cv::Mat img_bgr);


void initVideoStream( cv::VideoCapture &cap ) {
    if( cap.isOpened() )
        cap.release();
    
    cap.open(0); // open the default camera
    if ( cap.isOpened()==false ) {
        std::cout << "No webcam found, using a video file" << std::endl;
        cap.open("MarkerMovie.mpg");
        if ( cap.isOpened()==false ) {
            std::cout << "No video file found. Exiting."      << std::endl;
            exit(0);
        }
    }
}

void reshape(GLFWwindow* window, int width, int height){
    glViewport(0, 0, (GLsizei)width, (GLsizei)height);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    
    gluPerspective(virtual_camera_angle, ((GLsizei)width/ (GLsizei)height), 0.01, 500);
}

int main(int ac, char** av){
    GLFWwindow* window;
    
    if(!glfwInit()){
        return -1;
    }
    
    /* Create a windowed mode window and its OpenGL context */
    window = glfwCreateWindow(camera_width, camera_height, "Catch the Ball", NULL, NULL);
    
    if(!window){
        glfwTerminate();
        return -1;
    }
    
    // Set callback functions for GLFW
    glfwSetFramebufferSizeCallback(window, reshape);
    
    /* Make the window's context current */
    glfwMakeContextCurrent(window);
    glfwSwapInterval( 1 );
	/* capture esc key*/
	glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_TRUE);
    
    cv::Mat img_bgr;
    cv::VideoCapture cap;
    initVideoStream(cap);
    
    initGL();
    
    /* Loop until the user closes the window */
    while (glfwGetKey(window, GLFW_KEY_ESCAPE) != GLFW_PRESS && glfwWindowShouldClose(window) == 0) {
        cap >> img_bgr; // capture image
        
        if(img_bgr.empty()){
            std::cout << "Could not capture image" << std::endl;
            initVideoStream(cap);
            cv::waitKey(1000);
            continue;
        }
        GetPoses(poseMatrices, img_bgr, codes, codes_count);
		displayscore(starnum, img_bgr);
        display(window, img_bgr, poseMatrices, codes_count, camera_width, camera_height, starnum);
        glfwSwapBuffers(window);
        glfwPollEvents();
    }
    
    glfwTerminate();
    return 0;
}

/*  compute score user get
	show in the upper left corner of image */
void displayscore(int starnum, cv::Mat img_bgr)
{
	std::string starstring("Score:");
	starstring += std::to_string(starnum);
	int font_face = cv::FONT_HERSHEY_COMPLEX;
	double font_scale = 0.8;
	int thickness = 2;
	int baseline;
	cv::Size text_size = cv::getTextSize(starstring, font_face, font_scale, thickness, &baseline);
	cv::Point origin;
	origin.x = text_size.width / 4;
	origin.y = 100;
	//std::cout << starstring << std::endl;
	putText(img_bgr,
		starstring,
		origin,
		font_face,
		font_scale,
		cv::Scalar(1.0, 0.0, 0.0),
		thickness,
		8,
		0);
}
