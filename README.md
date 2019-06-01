# catchtheball

## main.cpp
- codes : Tracking AR marker codes. You can change the length using "codes_count".
- poseMatrices : poseMatrices are saved in this array. "poseMatrices[n]" is corresponding to the marker of "codes[n]".

You can track new AR marker changing these data. However, you have to change "DisplayGLImage.cpp" (below line 70) to render something  on the marker.

## DisplayGLImage.cpp
If you use Mac OSX, you have to comment out the line 58 ,and remove comment out of the line 57.

## CoordinateConverter.cpp
- Model2GameMatrix : Convert the model matrix (including captured matrix) to game matrix.
- Game2ModelMatrix : Convert inverse process.

Their input and output matrices are 4 * 4 pose matrix, and the format is same to openGL input format (and captured matrix format).
```
float matrix[16] = {
R, R, R, 0,
R, R, R, 0,
R, R, R, 0,
x, y, z, 1
};  // R is a transposed rotation matrix, and x, y and z are 3D position.
```

If you want to put the object to (x, y, z) in game coordinate system and don't consider rotation and scale, the input matrix "gameMatrix" of Game2ModelMatrix function should be as follows.
```
float gameMatrix[16] = {
1, 0, 0, 0,
0, 1, 0, 0,
0, 0, 1, 0,
x, y, z, 1
};
```

