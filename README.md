# catchtheball

## main.cpp
- codes : Tracking AR marker codes. You can change the length using "codes_count".
-poseMatrices : poseMatrices are saved in this array. "poseMatrices[n]" is corresponding to the marker of "codes[n]".

You can track new AR marker changing these data. However, you have to change "DisplayGLImage.cpp" (below line 70) to render something  on the marker.

## DisplayGLImage.cpp
If you use Mac OSX, you have to comment out the line 58 ,and remove comment out of the line 57.
