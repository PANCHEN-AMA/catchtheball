/**
 * compute whether user's hand touch the ball
 * @param poseMatrices pose of hand
 * @param ball bpose of the ball.
 */
void DetectCollision(float mark[][16], float ball[16], int &starnum, int& flag, float pose[16], float& startime);

/**
 * print shift to clean the screen
 */
void CleanScreen(int& flag, float& startime, int& starnum, float poseMatrices[][16]);
