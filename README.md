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
is reduced, and only features on preceding car are taken into account.

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

### MP.9 Timings / total scores

Table with regards of detection/descriptor extraction times

| Det / Desc | Min Matches | Max Matches | Average Matches | Avg. Detection Time | Avg. Desc. Extraction Time | Total Time | Total score |
| ---------- | ----------- | ----------- | --------------- | ------------------- | -------------------------- | ---------- | ----------- |
| SHITOMASI / BRISK | 79 | 95 | 85.2222 | 18.4819 ms | 2.16979 ms | 20.6517 ms | 4.12665 |
| SHITOMASI / BRIEF | 100 | 115 | 104.889 | 19.5499 ms | 1.35555 ms | 20.9054 ms | 5.0173 |
| SHITOMASI / ORB | 97 | 106 | 100.889 | 18.6833 ms | 1.08865 ms | 19.7719 ms | 5.10263 |
| SHITOMASI / FREAK | 80 | 90 | 85.3333 | 10.6279 ms | 36.0039 ms | 46.6318 ms | 1.82994 |
| SHITOMASI / SIFT | 96 | 112 | 103 | 10.4349 ms | 11.7574 ms | 22.1924 ms | 4.64124 |
| HARRIS / BRISK | 10 | 23 | 15.7778 | 11.5331 ms | 0.533341 ms | 12.0664 ms | 1.30758 |
| HARRIS / BRIEF | 11 | 26 | 19.2222 | 11.2255 ms | 0.297754 ms | 11.5232 ms | 1.66813 |
| HARRIS / ORB | 12 | 24 | 18 | 11.3132 ms | 0.580918 ms | 11.8941 ms | 1.51336 |
| HARRIS / FREAK | 12 | 21 | 16 | 11.3149 ms | 35.0574 ms | 46.3723 ms | 0.345033 |
| HARRIS / SIFT | 11 | 24 | 18.1111 | 11.3519 ms | 10.4039 ms | 21.7558 ms | 0.832473 |
| FAST / BRISK | 85 | 107 | 99.8889 | 0.916347 ms | 1.5269 ms | 2.44325 ms | 40.8836 |
| FAST / BRIEF | 108 | 131 | 122.111 | 0.934625 ms | 0.63872 ms | 1.57334 ms | 77.6124 |
| FAST / ORB | 106 | 126 | 119 | 0.909289 ms | 0.721501 ms | 1.63079 ms | 72.9708 |
| FAST / FREAK | 85 | 105 | 97.5556 | 0.909774 ms | 36.003 ms | 36.9128 ms | 2.64287 |
| FAST / SIFT | 103 | 123 | 116.222 | 0.915371 ms | 14.3418 ms | 15.2571 ms | 7.61757 |
| BRISK / BRISK | 73 | 92 | 83.4444 | 11.0821 ms | 1.29526 ms | 12.3773 ms | 6.74171 |
| BRISK / BRIEF | 43 | 84 | 60.5556 | 5.75147 ms | 0.462515 ms | 6.21398 ms | 9.74505 |
| BRISK / ORB | 67 | 101 | 84.7778 | 5.80198 ms | 3.52125 ms | 9.32322 ms | 9.09318 |
| BRISK / FREAK | 36 | 56 | 46.6667 | 5.76497 ms | 35.605 ms | 41.37 ms | 1.12803 |
| BRISK / SIFT | 67 | 95 | 84.7778 | 5.83441 ms | 33.5454 ms | 39.3798 ms | 2.15282 |
| ORB / BRISK | 157 | 188 | 174.444 | 33.2073 ms | 2.62672 ms | 35.834 ms | 4.86813 |
| ORB / BRIEF | 178 | 207 | 189.333 | 33.0152 ms | 0.871132 ms | 33.8863 ms | 5.58731 |
| ORB / ORB | 158 | 182 | 168.222 | 33.0004 ms | 3.63592 ms | 36.6363 ms | 4.59168 |
| ORB / FREAK | 155 | 183 | 169.333 | 33.0404 ms | 36.727 ms | 69.7674 ms | 2.42711 |
| ORB / SIFT | 169 | 195 | 182.889 | 32.9948 ms | 28.8524 ms | 61.8472 ms | 2.95711 |
| AKAZE / BRISK | 157 | 188 | 174.444 | 32.9491 ms | 2.60237 ms | 35.5515 ms | 4.90681 |
| AKAZE / BRIEF | 178 | 207 | 189.333 | 32.9899 ms | 0.885498 ms | 33.8754 ms | 5.58911 |
| AKAZE / ORB | 158 | 182 | 168.222 | 32.9446 ms | 3.51744 ms | 36.4621 ms | 4.61362 |
| AKAZE / FREAK | 155 | 183 | 169.333 | 32.9955 ms | 36.5895 ms | 69.585 ms | 2.43347 |
| AKAZE / SIFT | 169 | 195 | 182.889 | 32.9256 ms | 28.8436 ms | 61.7692 ms | 2.96084 |
| SIFT / BRISK | 157 | 188 | 174.444 | 33.0443 ms | 2.62374 ms | 35.6681 ms | 4.89077 |
| SIFT / BRIEF | 178 | 207 | 189.333 | 33.3744 ms | 0.875252 ms | 34.2496 ms | 5.52804 |
| SIFT / FREAK | 155 | 183 | 169.333 | 33.0492 ms | 36.6645 ms | 69.7137 ms | 2.42898 |
| SIFT / SIFT | 169 | 195 | 182.889 | 33.1263 ms | 28.9877 ms | 62.114 ms | 2.94441 |

---
#### Top 3 det / desc pairs:
Score by which different combinations of detectors and descriptors is calculated by following formula:

score = AM / AT

AM - Average Matches on 10 test images
AT - Average total time, detection + descriptor extraction on 10 test images

| Det / Desc | Average Matches | Avg. Det. Time | Avg. Desc. Extraction Time | Avg. Total Time | Total Score (Matches/ms) |
| ---------- | --------------- | -------------- | -------------------------- | --------------- | ------------------------ |
| FAST / BRIEF | 122.111 | 0.934625 ms | 0.63872 ms | 1.57334 ms | 77.6124
| FAST / ORB | 119 | 0.909289 ms | 0.721501 ms | 1.63079 ms | 72.9708
| FAST / BRISK | 99.8889 | 0.916347 ms | 1.5269 ms | 2.44325 ms | 40.8836

After taking look at statistics, it is also important to note that all combinations have
fair amount of false detections. FAST + BRIEF seems to be best candidate, as it seems that it has smallest amount
of false positives (conclusion based on visual observations).