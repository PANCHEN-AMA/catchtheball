#include "MarkerTracker.h"
#include "PoseEstimation.h"

#define SHOW_MARKER_DETECTION

uchar subpixSampleSafe(cv::Mat src, cv::Point2f point){
    int int_x = (int)point.x;
    int int_y = (int)point.y;
    float dx = point.x - int_x;
    float dy = point.y - int_y;
    
    if (0 <= int_x && int_x < src.cols - 1 && 0 <= int_y && int_y < src.rows - 1){
        uchar ret = (1 - dy) * ((1 - dx) * src.at<uchar>(int_y, int_x) + dx * src.at<uchar>(int_y, int_x + 1))
        + dy * ((1 - dx) * src.at<uchar>(int_y + 1, int_x) + dx * src.at<uchar>(int_y + 1, int_x + 1));
        return  ret;
    }else{
        return 127; // If outside of the image, return gray.
    }
}

cv::Mat getStripe(cv::Mat src, cv::Point2f center, cv::Point2f v_x, cv::Point2f v_y, int stripe_width){
    int stripe_height = 3;
    cv::Mat iplStripe(cv::Size(stripe_width, stripe_height), CV_8UC1);   // stripe image
    
    cv::Point2f zero_point = center - v_x * (stripe_width - 1) / 2.0  - v_y * (stripe_height - 1) / 2.0;
    for(int i = 0; i < stripe_width; i++){
        for(int j = 0; j < stripe_height; j++){
            cv::Point2f subPixel = zero_point + v_x * i + v_y * j;
            uchar pixel = subpixSampleSafe (src, subPixel);
            iplStripe.at<uchar>(j, i) = pixel;
        }
    }
    return iplStripe;
}


float* filterStripe(cv::Mat src, int stripe_width){
    int filtered_width = stripe_width - 2;
    float* filteredStripe = new float[filtered_width];
    
    /* filter
     * |-1.0, 0.0, 1.0|
     * |-2.0, 0.0, 2.0|
     * |-1.0, 0.0, 1.0|*/
    
    for(int x = 0; x < filtered_width; x++){
        filteredStripe[x] =
        (float)src.at<uchar>(0,x) * -1.0 + (float)src.at<uchar>(0,x+2) * 1.0 +
        (float)src.at<uchar>(1,x) * -2.0 + (float)src.at<uchar>(1,x+2) * 2.0 +
        (float)src.at<uchar>(2,x) * -1.0 + (float)src.at<uchar>(2,x+2) * 1.0;
    }
    
    return filteredStripe;
}

/* Convert projection matrix
 * as to change coordinate system from openCV to openGL */
void ConvertMatrixCV2GL(float* matrix, float* matrix_gl, int width, int height){
    /* Transpose */
    matrix_gl[0] = matrix[0];
    matrix_gl[1] = matrix[4];
    matrix_gl[2] = matrix[8];
    matrix_gl[3] = matrix[12];
    matrix_gl[4] = matrix[1];
    matrix_gl[5] = matrix[5];
    matrix_gl[6] = matrix[9];
    matrix_gl[7] = matrix[13];
    matrix_gl[8] = matrix[2];
    matrix_gl[9] = matrix[6];
    matrix_gl[10] = matrix[10];
    matrix_gl[11] = matrix[14];
    matrix_gl[12] = matrix[3];
    matrix_gl[13] = matrix[7];
    matrix_gl[14] = matrix[11];
    matrix_gl[15] = matrix[15];
    // std::cout << matrix_gl[0] << "," <<  matrix_gl[1] << "," << matrix_gl[2] << "," << matrix_gl[3] << std::endl;
    // std::cout << matrix_gl[4] << "," <<  matrix_gl[5] << "," << matrix_gl[6] << "," << matrix_gl[7] << std::endl;
    // std::cout << matrix_gl[8] << "," <<  matrix_gl[9] << "," << matrix_gl[10] << "," << matrix_gl[11] << std::endl;
    // std::cout << matrix_gl[12] << "," <<  matrix_gl[13] << "," << matrix_gl[14] << "," << matrix_gl[15] << std::endl << std::endl;
}

/**
 * computes the pose of AR markers with specific code number
 * @param matrices result of AR marker poses at a openGL coordinate system.
 * @param img_bgr sorce image from camera.
 * @param codes_forDetect the codes of AR marker.
 * @param codes_count the length of the "codes".
 */
void GetPoses(float matrices[][16], cv::Mat img_bgr, const int codes_forDetect[], const int codes_count){
    bool isClosed = true;
    int thickness = 4;
    
    cv::Mat img_gray;
    cv::Mat img_filtered;
    
    cv::cvtColor( img_bgr, img_gray, cv::COLOR_BGR2GRAY);
    
    // cv::threshold( img_gray, img_filtered, 100, 255, cv::THRESH_BINARY);
    cv::adaptiveThreshold(img_gray, img_filtered, 255, cv::ADAPTIVE_THRESH_MEAN_C, cv::THRESH_BINARY, 33, 5);
    
    std::vector< std::vector<cv::Point> > contours;
    std::vector<cv::Vec4i> hierarchy;
    cv::findContours(img_filtered, contours, hierarchy, cv::RETR_LIST, cv::CHAIN_APPROX_SIMPLE);    // changed to cv::RETR_LIST
    
    for(std::vector<cv::Point> contour : contours) {
        auto epsilon = 0.1*cv::arcLength(contour, true);
        std::vector<cv::Point> approx;
        cv::approxPolyDP(contour, approx, epsilon, true);
        
        // Check size
        cv::Rect rect = cv::boundingRect(approx);
        int markerArea = rect.area();
        bool is_valid = (markerArea > markerAreaMin)
        && (markerArea < markerAreaMax)
        && approx.size() == 4
        && cv::isContourConvex(approx);
        
        if (is_valid == false) continue;
        
#ifdef SHOW_MARKER_DETECTION
        cv::polylines(img_bgr, approx, isClosed, CV_RGB(0,255,0), thickness);
#endif
        
        cv::Vec4f lines[4];     // for edges
        
        for (int i=0; i<4; ++i){        // i: index of edge
            cv::circle (img_bgr, approx[i], 3, CV_RGB(0,255,0), -1);
            
            cv::Point2f vertex_0, vertex_1, v_y, b;
            
            vertex_0 = approx[i % 4];
            vertex_1 = approx[(i + 1) % 4];
            v_y = vertex_1 - vertex_0;
            
            // Stripe size
            float v_y_norm = cv::norm(v_y);
            int stripeLength = (int)(0.1*v_y_norm);
            if (stripeLength < 5){
                stripeLength = 5;
            }
            
            v_y = v_y / v_y_norm;  // normalize
            cv::Point2f v_x{v_y.y, -v_y.x};  //v_x is perpendicular to v_y
            
            cv::Point2f edgePoints[6];
            
            for (int j=1; j<7; ++j){        // j: index of point
                float rate = j / 7.0;
                cv::Point2f stripe_p = (1 - rate) * vertex_0 + rate * vertex_1;
                
#ifdef SHOW_MARKER_DETECTION
                cv::circle (img_bgr, stripe_p, 2, CV_RGB(0,0,255), -1);
#endif
                
                // get stripes
                cv::Mat stripe = getStripe(img_gray, stripe_p, v_x, v_y, stripeLength);
                
                float* filtered_stripe = filterStripe(stripe, stripeLength);
                float peak_num = 0;     // large number
                int peak_index = 1;
                for(int i = 1; i < stripeLength - 2; i++){
                    float num = filtered_stripe[i];
                    if(peak_num < num){
                        peak_num = num;
                        peak_index = i;
                    }
                }
                // detect the lowest point
                float a = (filtered_stripe[peak_index-1] + filtered_stripe[peak_index+1]) / 2 - filtered_stripe[peak_index];
                
                float b = filtered_stripe[peak_index + 1] - filtered_stripe[peak_index] - a * (2 * peak_index + 1);
                float peak_x = -b/(2 * a);
                cv::Point2f peak_point = stripe_p + (peak_x + 1.0 - stripeLength/2) * v_x;
                
#ifdef SHOW_MARKER_DETECTION
                cv::circle (img_bgr, peak_point, 2, CV_RGB(0,255,255), -1);
#endif
                edgePoints[j-1] = peak_point;
                
            }
            
            // seven points are caluculated, then fitting line
            cv::Mat edgePoints_mat(cv::Size(1, 6), CV_32FC2, edgePoints);
            cv::fitLine(edgePoints_mat, lines[i], cv::DIST_L2, 0, 0.01, 0.01);
        }
        
        // detect actual corner point
        cv::Point2f corners_src[4];
        
        for(int i = 0; i < 4; i++){
            cv::Vec4f line_0 = lines[i];
            cv::Vec4f line_1 = lines[(i + 1) % 4];
            float c_0 = line_0[1] * line_0[2] - line_0[0] * line_0[3];
            float c_1 = line_1[1] * line_1[2] - line_1[0] * line_1[3];
            float detA = line_0[0] * line_1[1] - line_0[1] * line_1[0];
            corners_src[i].x = (c_1 * line_0[0] - c_0 * line_1[0]) / detA;
            corners_src[i].y = (c_1 * line_0[1] - c_0 * line_1[1]) / detA;
            
#ifdef SHOW_MARKER_DETECTION
            cv::circle (img_bgr, corners_src[i], 3, CV_RGB(255,255,0), -1);
#endif
        }
        
        cv::Point2f targetCorners[4];
        targetCorners[0].x = -0.5; targetCorners[0].y = -0.5;
        targetCorners[1].x =  5.5; targetCorners[1].y = -0.5;
        targetCorners[2].x =  5.5; targetCorners[2].y =  5.5;
        targetCorners[3].x = -0.5; targetCorners[3].y =  5.5;
        
        cv::Mat perseChangeMat = cv::getPerspectiveTransform(corners_src, targetCorners);
        cv::Mat marker(6, 6, CV_8U);     // marker
        cv::warpPerspective(img_gray, marker, perseChangeMat, marker.size());
        
        /* detect marker ID */
        cv::Mat marker_filtered;
        // const int bw_thresh = 100;
        // cv::threshold(marker, marker_filtered, bw_thresh, 255, cv::THRESH_BINARY);
        cv::adaptiveThreshold(marker, marker_filtered, 255, cv::ADAPTIVE_THRESH_MEAN_C, cv::THRESH_BINARY, 3, 0);

        // check if border is black
        int code = 0;
        for (int i = 1; i < 5; ++i){
            int pixel1 = marker_filtered.at<uchar>(0,i);
            int pixel2 = marker_filtered.at<uchar>(5,i);
            int pixel3 = marker_filtered.at<uchar>(i,0);
            int pixel4 = marker_filtered.at<uchar>(i,5);
            if ( ( pixel1 > 0 ) || ( pixel2 > 0 ) || ( pixel3 > 0 ) || ( pixel4 > 0 ) ){
                code = -1;
                break;
            }
        }
        
        if ( code < 0 ){
            continue;
        }
        
        //copy the BW values into cP
        int cP[4][4];
        for ( int i=0; i < 4; ++i){
            for ( int j=0; j < 4; ++j){
                cP[i][j] = marker_filtered.at<uchar>(i+1,j+1);
                cP[i][j] = (cP[i][j]==0) ? 1 : 0; //if black then 1 else 0
            }
        }
        
        //save the ID of the marker
        int codes[4];
        codes[0] = codes[1] = codes[2] = codes[3] = 0;
        for (int i=0; i < 16; i++)
        {
            int row = i>>2;
            int col = i%4;
            
            codes[0] <<= 1;
            codes[0] |= cP[row][col]; // 0°
            
            codes[1] <<= 1;
            codes[1] |= cP[3-col][row]; // 90°
            
            codes[2] <<= 1;
            codes[2] |= cP[3-row][3-col]; // 180°
            
            codes[3] <<= 1;
            codes[3] |= cP[col][3-row]; // 270°
        }
        
        if ( (codes[0]==0) || (codes[0]==0xffff) ){
            continue;
        }
        
        //account for symmetry
        code = codes[0];
        int direction = 0;
        for ( int i=1; i<4; ++i ){
            if ( codes[i] < code ){
                code = codes[i];
                direction = i;
            }
        }
        
        cv::Point2f inputPoints[4];
        cv::Point2f offset(img_bgr.cols / 2.0, img_bgr.rows / 2.0);
        
        inputPoints[0] = corners_src[(7 - direction) % 4] - offset;
        inputPoints[0].y = -inputPoints[0].y;
        inputPoints[1] = corners_src[(6 - direction) % 4] - offset;
        inputPoints[1].y = -inputPoints[1].y;
        inputPoints[2] = corners_src[(5 - direction) % 4] - offset;
        inputPoints[2].y = -inputPoints[2].y;
        inputPoints[3] = corners_src[(4 - direction) % 4] - offset;
        inputPoints[3].y = -inputPoints[3].y;
        
        for(int code_index = 0; code_index < codes_count; code_index++){
            if(code == codes_forDetect[code_index]){
                float newMat_cv[16];
                estimateSquarePose(newMat_cv, inputPoints, markerSize);
                ConvertMatrixCV2GL(newMat_cv, matrices[code_index], img_bgr.size().width, img_bgr.size().height);
            }
        }
    }
}
