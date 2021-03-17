# SFND 2D Feature Tracking

<img src="images/keypoints.png" width="820" height="248" />

The idea of the camera course is to build a collision detection system - that's the overall goal for the Final Project. As a preparation for this, you will now build the feature tracking part and test various detector / descriptor combinations to see which ones perform best. This mid-term project consists of four parts:

* First, you will focus on loading images, setting up data structures and putting everything into a ring buffer to optimize memory load. 
* Then, you will integrate several keypoint detectors such as HARRIS, FAST, BRISK and SIFT and compare them with regard to number of keypoints and speed. 
* In the next part, you will then focus on descriptor extraction and matching using brute force and also the FLANN approach we discussed in the previous lesson. 
* In the last part, once the code framework is complete, you will test the various algorithms in different combinations and compare them with regard to some performance measures. 

See the classroom instruction and code comments for more details on each of these parts. Once you are finished with this project, the keypoint matching part will be set up and you can proceed to the next lesson, where the focus is on integrating Lidar points and on object detection using deep-learning. 

## Dependencies for Running Locally
* cmake >= 2.8
  * All OSes: [click here for installation instructions](https://cmake.org/install/)
* make >= 4.1 (Linux, Mac), 3.81 (Windows)
  * Linux: make is installed by default on most Linux distros
  * Mac: [install Xcode command line tools to get make](https://developer.apple.com/xcode/features/)
  * Windows: [Click here for installation instructions](http://gnuwin32.sourceforge.net/packages/make.htm)
* OpenCV >= 4.1
  * This must be compiled from source using the `-D OPENCV_ENABLE_NONFREE=ON` cmake flag for testing the SIFT and SURF detectors.
  * The OpenCV 4.1.0 source code can be found [here](https://github.com/opencv/opencv/tree/4.1.0)
* gcc/g++ >= 5.4
  * Linux: gcc / g++ is installed by default on most Linux distros
  * Mac: same deal as make - [install Xcode command line tools](https://developer.apple.com/xcode/features/)
  * Windows: recommend using [MinGW](http://www.mingw.org/)

## Basic Build Instructions

1. Clone this repo.
2. Make a build directory in the top level directory: `mkdir build && cd build`
3. Compile: `cmake .. && make`
4. Run it: `./2D_feature_tracking`.

Neigh size min:
Neigh size max:
Neigh size mean

### MP.7 Detector stats:
Overview on each of the evaluated detector and it's performance on total ammount of keypoint detections in region of 
interest (preceeding vehicle).

| Detector | Min Keypoints | Max Keypoints | Neighborhood size min | Neighborhood size max | Neighborhood size mean |
| -------- | ------------- | ------------- | --------------------- | --------------------- | ---------------------- |
| SHITOMASI | 111 | 125 | 4 | 4 | 4 |
| HARRIS | 14 | 43 | 6 | 6 | 6 |
| FAST | 138 | 156 | 7 | 7 | 7 |
| BRISK | 92 | 130 | 31 | 111.079 | 55.9928 |
| ORB | 254 | 297 | 8.4 | 72 | 21.9444 |
| AKAZE | 254 | 297 | 8.4 | 72 | 21.9444 |
| SIFT | 254 | 297 | 8.4 | 72 | 21.9444 |

### MP.8 Matches stats:
Total matches on tested detector/descriptor pairs, where brute force matching
is used with KNN selector and descriptor distance ratio of 0.8. Region of interest
is reduced, and only features on preceding care are taken into account.

| Det / Desc | Min Matches | Max Matches | Average Matches |
| ---------- | ----------- | ----------- | --------------- |
| SHITOMASI / BRISK | 79 | 95 | 85.2222 |
| SHITOMASI / BRIEF | 100 | 115 | 104.889 |
| SHITOMASI / ORB | 97 | 106 | 100.889 |
| SHITOMASI / FREAK | 80 | 90 | 85.3333 |
| SHITOMASI / AKAZE | N/A | N/A | N/A |
| SHITOMASI / SIFT | 96 | 112 | 103 |
| HARRIS / BRISK | 10 | 23 | 15.7778 |
| HARRIS / BRIEF | 11 | 26 | 19.2222 |
| HARRIS / ORB | 12 | 24 | 18 |
| HARRIS / FREAK | 12 | 21 | 16 |
| HARRIS / AKAZE | N/A | N/A | N/A |
| HARRIS / SIFT | 11 | 24 | 18.1111 |
| FAST / BRISK | 85 | 107 | 99.8889 |
| FAST / BRIEF | 108 | 131 | 122.111 |
| FAST / ORB | 106 | 126 | 119 |
| FAST / FREAK | 85 | 105 | 97.5556 |
| FAST / AKAZE | N/A | N/A | N/A |
| FAST / SIFT | 103 | 123 | 116.222 |
| BRISK / BRISK | 73 | 92 | 83.4444 |
| BRISK / BRIEF | 43 | 84 | 60.5556 |
| BRISK / ORB | 67 | 101 | 84.7778 |
| BRISK / FREAK | 36 | 56 | 46.6667 |
| BRISK / AKAZE | N/A | N/A | N/A |
| BRISK / SIFT | 67 | 95 | 84.7778 |
| ORB / BRISK | 157 | 188 | 174.444 |
| ORB / BRIEF | 178 | 207 | 189.333 |
| ORB / ORB | 158 | 182 | 168.222 |
| ORB / FREAK | 155 | 183 | 169.333 |
| ORB / AKAZE | N/A | N/A | N/A |
| ORB / SIFT | 169 | 195 | 182.889 |
| AKAZE / BRISK | 157 | 188 | 174.444 |
| AKAZE / BRIEF | 178 | 207 | 189.333 |
| AKAZE / ORB | 158 | 182 | 168.222 |
| AKAZE / FREAK | 155 | 183 | 169.333 |
| AKAZE / AKAZE | N/A | N/A | N/A |
| AKAZE / SIFT | 169 | 195 | 182.889 |
| SIFT / BRISK | 157 | 188 | 174.444 |
| SIFT / BRIEF | 178 | 207 | 189.333 |
| SIFT / ORB | N/A | N/A | N/A |
| SIFT / FREAK | 155 | 183 | 169.333 |
| SIFT / AKAZE | N/A | N/A | N/A |
| SIFT / SIFT | 169 | 195 | 182.889 |