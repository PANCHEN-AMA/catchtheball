#include "DisplayGLImage.h"
#include "DrawPrimitives.h"

/**
 * Initialize open GL window
 */
void initGL(){
    glPixelStorei(GL_PACK_ALIGNMENT, 1);
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    glPixelZoom(1.0, -1.0);
    
    // enable and set colors
    glEnable( GL_COLOR_MATERIAL );
    glClearColor( 0.0, 0.0, 0.0, 1.0 );
    
    // enable and set depth parameters
    glEnable( GL_DEPTH_TEST );
    glClearDepth( 1.0 );
    
    // light parameters
    GLfloat light_pos[] = {1.0, 1.0, 1.0, 0.0};
    GLfloat light_amb[] = {0.2, 0.2, 0.2, 1.0};
    GLfloat light_dif[] = {0.7, 0.7, 0.7, 1.0};
    
    // enable lighting
    glLightfv(GL_LIGHT0, GL_POSITION, light_pos);
    glLightfv(GL_LIGHT0, GL_AMBIENT, light_amb);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, light_dif);
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
}


/**
 * render window image with openGL
 * @param window displaying window
 * @param img_bgr background image from camera.
 */
void display (GLFWwindow* window, const cv::Mat &img_bgr, float poseMatrices[][16], int codes_count, int width_gl, int height_gl){
    glfwGetFramebufferSize(window, &width_gl, &height_gl);
    //unsigned char background[width_gl * height_gl * 3];
    unsigned char *background;
	background = new unsigned char[width_gl * height_gl * 3];

    memcpy(background, img_bgr.data, sizeof(background));
    
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    
    glDisable(GL_DEPTH_TEST);
    glMatrixMode(GL_PROJECTION);
    glPushMatrix();
    glLoadIdentity();
    gluOrtho2D(0.0, width_gl, 0.0, height_gl);
    
    glRasterPos2i(0, height_gl - 1);
    // glDrawPixels(width_gl, height_gl, GL_BGR, GL_UNSIGNED_BYTE, background);  // for Mac OSX
    glDrawPixels(width_gl, height_gl, GL_BGR_EXT, GL_UNSIGNED_BYTE, img_bgr);  // for Windows
    
    glPopMatrix();
    glLoadIdentity();
    gluPerspective( virtual_camera_angle, ((double)width_gl/(double)height_gl), 0.01, 100 );
    
    glEnable(GL_DEPTH_TEST);
    
    /* rendering openGL ----------------------------------------- */
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    if (poseMatrices[1] != NULL) {
		glPushMatrix();
		glLoadMatrixf(poseMatrices[1]);
		glScalef(0.05f, 0.05f, 0.05f);

		// Draw field
		drawField();
		glPopMatrix();
	}
    
    if(poseMatrices[0] != NULL){
        glPushMatrix();
        glLoadMatrixf(poseMatrices[0]);
        glScalef(0.01f, 0.01f, 0.01f);
        
        // Draw ball
        glColor4f(1.0, 0.0, 0.0, 1.0);
        drawSphere(1, 10, 10);
        glPopMatrix();
    }
    delete[] background;
    
    
}
