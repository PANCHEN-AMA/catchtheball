#include <opencv2/opencv.hpp> /// include most of core headers
#include <opencv2/core.hpp> /// include most of core headers

const int field_length = 0.25;

/**
 * convert model coordinate system to game coordinate system
 * All matrices are ready to openGL
 * @param  gameMatrix result converted matrix
 * @param modelMatrix input
 * @param fieldMatrix input
 */
void Model2GameMatrix(float* gameMatrix, float* modelMatrix, float* fieldMatrix);

/**
 * convert model coordinate system to game coordinate system
 * All matrices are ready to openGL
 * @param  gameMatrix result converted matrix
 * @param modelMatrix input
 * @param fieldMatrix input
 */
void Game2ModelMatrix(float* modelMatrix, float* gameMatrix, float* fieldMatrix);
