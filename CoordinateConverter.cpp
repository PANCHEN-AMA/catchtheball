#include "CoordinateConverter.h"


void Model2GameMatrix(float* gameMatrix, float* modelMatrix, float* fieldMatrix){
    cv::Mat matGameMatrix = cv::Mat(4, 4, CV_32F, gameMatrix);
    cv::Mat matModelMatrix = cv::Mat(4, 4, CV_32F, modelMatrix);
    cv::Mat matFieldMatrix = cv::Mat(4, 4, CV_32F, fieldMatrix);
    
    float filterGameMatrix[16] = {
        1, 1, 1, 1,
        1, 1, 1, 1,
        1, 1, 1, 1,
        field_length, field_length, field_length, 1
    };
    cv::Mat matFilterGameMatrix = cv::Mat(4, 4, CV_32F, filterGameMatrix);
    
    matGameMatrix = (matModelMatrix * matFieldMatrix.inv()) / matFilterGameMatrix;
}


void Game2ModelMatrix(float* modelMatrix, float* gameMatrix, float* fieldMatrix){
    cv::Mat matModelMatrix = cv::Mat(4, 4, CV_32F, modelMatrix);
    cv::Mat matGameMatrix = cv::Mat(4, 4, CV_32F, gameMatrix);
    cv::Mat matFieldMatrix = cv::Mat(4, 4, CV_32F, fieldMatrix);
    float filterGameMatrix[16] = {
        1, 1, 1, 1,
        1, 1, 1, 1,
        1, 1, 1, 1,
        field_length, field_length, field_length, 1
    };
    cv::Mat matFilterGameMatrix = cv::Mat(4, 4, CV_32F, filterGameMatrix);

    
    matModelMatrix = matGameMatrix.mul(matFilterGameMatrix) * matFieldMatrix;
}
