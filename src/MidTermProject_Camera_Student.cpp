/* INCLUDES FOR THIS PROJECT */
#include <iostream>
#include <fstream>
#include <sstream>
#include <iomanip>
#include <vector>
#include <cmath>
#include <limits>
#include <opencv2/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/features2d.hpp>
#include <opencv2/xfeatures2d.hpp>
#include <opencv2/xfeatures2d/nonfree.hpp>
#include <numeric>

#include "dataStructures.h"
#include "matching2D.hpp"

using namespace std;

void debugRoutine()
{
    /* INIT VARIABLES AND DATA STRUCTURES */

    // data location
    string dataPath = "../";

    // camera
    string imgBasePath = dataPath + "images/";
    string imgPrefix = "KITTI/2011_09_26/image_00/data/000000"; // left camera, color
    string imgFileType = ".png";
    int imgStartIndex = 0; // first file index to load (assumes Lidar and camera names have identical naming convention)
    int imgEndIndex = 9;   // last file index to load
    int imgFillWidth = 4;  // no. of digits which make up the file index (e.g. img-0001.png)

    // misc
    int dataBufferSize = 2;       // no. of images which are held in memory (ring buffer) at the same time
    vector<DataFrame> dataBuffer; // list of data frames which are held in memory at the same time
    bool bVis = false;            // visualize results

    /* MAIN LOOP OVER ALL IMAGES */

    for (size_t imgIndex = 0; imgIndex <= imgEndIndex - imgStartIndex; imgIndex++)
    {
        /* LOAD IMAGE INTO BUFFER */

        // assemble filenames for current index
        ostringstream imgNumber;
        imgNumber << setfill('0') << setw(imgFillWidth) << imgStartIndex + imgIndex;
        string imgFullFilename = imgBasePath + imgPrefix + imgNumber.str() + imgFileType;

        // load image from file and convert to grayscale
        cv::Mat img, imgGray;
        img = cv::imread(imgFullFilename);
        cv::cvtColor(img, imgGray, cv::COLOR_BGR2GRAY);

        //// STUDENT ASSIGNMENT
        //// TASK MP.1 -> replace the following code with ring buffer of size dataBufferSize

        // push image into data frame buffer
        DataFrame frame;
        frame.cameraImg = imgGray;
        dataBuffer.push_back(frame);
        if(dataBuffer.size() > dataBufferSize)
        {
            // Don't allow vector to grow past defined size
            dataBuffer.erase(dataBuffer.begin());
        }

        //// EOF STUDENT ASSIGNMENT
        cout << "#1 : LOAD IMAGE INTO BUFFER done" << endl;

        /* DETECT IMAGE KEYPOINTS */

        // extract 2D keypoints from current image
        vector<cv::KeyPoint> keypoints; // create empty feature list for current image
        string detectorType = "SHITOMASI"; // -> SHITOMASI, HARRIS, FAST, BRISK, ORB, AKAZE, SIFT

        //// STUDENT ASSIGNMENT
        //// TASK MP.2 -> add the following keypoint detectors in file matching2D.cpp and enable string-based selection based on detectorType
        //// -> HARRIS, FAST, BRISK, ORB, AKAZE, SIFT

        double duration = 0.0;
        if (detectorType.compare("SHITOMASI") == 0)
        {
            detKeypointsShiTomasi(keypoints, imgGray, duration, false);
        }
        else if (detectorType.compare("HARRIS") == 0)
        {
            detKeypointsHarris(keypoints, imgGray, duration, false);
        }
        else
        {
            detKeypointsModern(keypoints, imgGray, detectorType, duration, bVis);
        }

        //// EOF STUDENT ASSIGNMENT

        //// STUDENT ASSIGNMENT
        //// TASK MP.3 -> only keep keypoints on the preceding vehicle

        // only keep keypoints on the preceding vehicle
        bool bFocusOnVehicle = true;
        cv::Rect vehicleRect(535, 180, 180, 150);
        if (bFocusOnVehicle)
        {
            keypoints.erase(
                    std::remove_if(keypoints.begin(), keypoints.end(), [&vehicleRect](const cv::KeyPoint &keypoint)
                    {
                        return !vehicleRect.contains(keypoint.pt);
                    }), keypoints.end());
        }

        //// EOF STUDENT ASSIGNMENT

        // optional : limit number of keypoints (helpful for debugging and learning)
        bool bLimitKpts = true;
        if (bLimitKpts)
        {
            int maxKeypoints = 50;

            if (detectorType.compare("SHITOMASI") == 0)
            { // there is no response info, so keep the first 50 as they are sorted in descending quality order
                keypoints.erase(keypoints.begin() + maxKeypoints, keypoints.end());
            }
            cv::KeyPointsFilter::retainBest(keypoints, maxKeypoints);
            cout << " NOTE: Keypoints have been limited!" << endl;
        }

        // push keypoints and descriptor for current frame to end of data buffer
        (dataBuffer.end() - 1)->keypoints = keypoints;
        cout << "#2 : DETECT KEYPOINTS done" << endl;

        /* EXTRACT KEYPOINT DESCRIPTORS */

        //// STUDENT ASSIGNMENT
        //// TASK MP.4 -> add the following descriptors in file matching2D.cpp and enable string-based selection based on descriptorType
        //// -> BRIEF, ORB, FREAK, AKAZE, SIFT

        cv::Mat descriptors;
        string descriptorType = "FREAK"; // BRISK, BRIEF, ORB, FREAK, AKAZE, SIFT
        double descDuration = 0.0;
        descKeypoints((dataBuffer.end() - 1)->keypoints, (dataBuffer.end() - 1)->cameraImg, descriptors, descriptorType, descDuration);
        //// EOF STUDENT ASSIGNMENT

        // push descriptors for current frame to end of data buffer
        (dataBuffer.end() - 1)->descriptors = descriptors;

        cout << "#3 : EXTRACT DESCRIPTORS done" << endl;

        if (dataBuffer.size() > 1) // wait until at least two images have been processed
        {

            /* MATCH KEYPOINT DESCRIPTORS */

            vector<cv::DMatch> matches;
            string matcherType = "MAT_FLANN";     // MAT_BF, MAT_FLANN
            string descriptorType = "DES_BINARY"; // DES_BINARY, DES_HOG
            string selectorType = "SEL_KNN";       // SEL_NN, SEL_KNN

            //// STUDENT ASSIGNMENT
            //// TASK MP.5 -> add FLANN matching in file matching2D.cpp
            //// TASK MP.6 -> add KNN match selection and perform descriptor distance ratio filtering with t=0.8 in file matching2D.cpp

            matchDescriptors((dataBuffer.end() - 2)->keypoints, (dataBuffer.end() - 1)->keypoints,
                             (dataBuffer.end() - 2)->descriptors, (dataBuffer.end() - 1)->descriptors,
                             matches, descriptorType, matcherType, selectorType);

            //// EOF STUDENT ASSIGNMENT

            // store matches in current data frame
            (dataBuffer.end() - 1)->kptMatches = matches;

            cout << "#4 : MATCH KEYPOINT DESCRIPTORS done" << endl;

            // visualize matches between current and previous image
            bVis = true;
            if (bVis)
            {
                cv::Mat matchImg = ((dataBuffer.end() - 1)->cameraImg).clone();
                cv::drawMatches((dataBuffer.end() - 2)->cameraImg, (dataBuffer.end() - 2)->keypoints,
                                (dataBuffer.end() - 1)->cameraImg, (dataBuffer.end() - 1)->keypoints,
                                matches, matchImg,
                                cv::Scalar::all(-1), cv::Scalar::all(-1),
                                vector<char>(), cv::DrawMatchesFlags::DRAW_RICH_KEYPOINTS);

                string windowName = "Matching keypoints between two camera images";
                cv::namedWindow(windowName, 7);
                cv::imshow(windowName, matchImg);
                cout << "Press key to continue to next image" << endl;
                cv::waitKey(0); // wait for key to be pressed
            }
            bVis = false;
        }

    } // eof loop over all images
}

void printStats(const std::string& detectorType, std::vector<std::vector<cv::KeyPoint>>& detections, std::vector<float> neighSizes)
{
    std::sort(detections.begin(), detections.end(),
              [](const std::vector<cv::KeyPoint> &rhs, const std::vector<cv::KeyPoint> &lhs)
              {
                  return lhs.size() > rhs.size();
              });

    std::sort(neighSizes.begin(), neighSizes.end());

    float meanSize = std::accumulate(neighSizes.begin(), neighSizes.end(), 0.0, [](float sum, float size)
    {
        return sum + size;
    }) / neighSizes.size();

    std::ostringstream os;

    os << "| " << detectorType << " | " << detections[0].size()
        << " | " << detections[detections.size() - 1].size()
        << " | " <<  neighSizes[0]
        << " | " << neighSizes[neighSizes.size() - 1]
        << " | " << meanSize << " |";

    std::cout << os.str() << std::endl;
}

void printM8Stats(const std::string &detector, const std::string &descriptor, std::vector<int> &totalMatches)
{
    std::sort(totalMatches.begin(), totalMatches.end());
    std::cout << "| " << detector << " / " << descriptor
              << " | " << totalMatches[0]
              << " | " << totalMatches[totalMatches.size() - 1]
              << " | " << ((float) std::accumulate(totalMatches.begin(), totalMatches.end(), 0.0)) / totalMatches.size()
              << " |" << std::endl;
}

void printNA(const std::string &detector, const std::string &descriptor)
{
    std::cout << "| " << detector << " / " << descriptor
              << " | " << "N/A"
              << " | " << "N/A"
              << " | " << "N/A"
              << " |" << std::endl;
}

struct StatHolder
{
    std::string detector;
    std::string descriptor;
    double averageDetTime;
    double averageDescTime;
    double averageMatches;
    double totalScore;
};

void prepareM9StatsRanking(const std::string &detector, const std::string &descriptor, std::vector<int> &totalMatches,
                  std::vector<double> &detTimes, std::vector<double> &descTimes, std::vector<StatHolder>& records)
{
    std::sort(totalMatches.begin(), totalMatches.end());
    std::sort(detTimes.begin(), detTimes.end());
    std::sort(descTimes.begin(), descTimes.end());

    double averageDetTime = ((double) std::accumulate(detTimes.begin(), detTimes.end(), 0.0)) / detTimes.size();
    double averageDescTime = ((double) std::accumulate(descTimes.begin(), descTimes.end(), 0.0)) / descTimes.size();
    double averageMatches = ((double) std::accumulate(totalMatches.begin(), totalMatches.end(), 0.0)) / totalMatches.size();

    records.push_back({detector, descriptor, averageDetTime, averageDescTime, averageMatches, averageMatches / (averageDescTime + averageDetTime)});
}

void printM9Stats(const std::string &detector, const std::string &descriptor, std::vector<int> &totalMatches,
                  std::vector<double> &detTimes, std::vector<double> &descTimes)
{
    std::sort(totalMatches.begin(), totalMatches.end());
    std::sort(detTimes.begin(), detTimes.end());
    std::sort(descTimes.begin(), descTimes.end());

    double averageDetTime = ((double) std::accumulate(detTimes.begin(), detTimes.end(), 0.0)) / detTimes.size();
    double averageDescTime = ((double) std::accumulate(descTimes.begin(), descTimes.end(), 0.0)) / descTimes.size();
    double averageMatches = ((double) std::accumulate(totalMatches.begin(), totalMatches.end(), 0.0)) / totalMatches.size();

    std::cout << "| " << detector << " / " << descriptor
              << " | " << totalMatches[0]
              << " | " << totalMatches[totalMatches.size() - 1]
              << " | " << averageMatches
              << " | " << averageDetTime << " ms"
              << " | " << averageDescTime << " ms"
              << " | " << averageDetTime + averageDescTime << " ms"
              << " | " << averageMatches / (averageDescTime + averageDetTime) // Total score avg matches / avg time
              << " |" << std::endl;
}


std::vector<std::string> getCompatibleDescType(const std::string &descriptor)
{
    if (descriptor == "BRISK")
    {
        return {"DES_BINARY", "DES_HOG"};
    }
    else if (descriptor == "BRIEF")
    {
        return {"DES_BINARY", "DES_HOG"};
    }
    else if (descriptor == "ORB")
    {
        return {"DES_BINARY", "DES_HOG"};
    }
    else if (descriptor == "FREAK")
    {
        return {"DES_BINARY", "DES_HOG"};
    }
    else if (descriptor == "AKAZE")
    {
        return {"DES_BINARY", "DES_HOG"};
    }
    else if (descriptor == "SIFT")
    {
        return {"DES_HOG"};
    }
    else
    {
        return {};
    }
}

std::vector<std::string> detectors = {"SHITOMASI", "HARRIS", "FAST", "BRISK", "ORB", "AKAZE", "SIFT"};
std::vector<std::string> descriptors = {"BRISK", "BRIEF", "ORB", "FREAK", "AKAZE", "SIFT"};

void measurementRoutine()
{
    bool printM7StatsFlag = false;
    bool printM8StatsFlag = false;
    bool printM9StatsFlag = true;
    std::vector<StatHolder> records;
    string dataPath = "../";

    // camera
    string imgBasePath = dataPath + "images/";
    string imgPrefix = "KITTI/2011_09_26/image_00/data/000000"; // left camera, color
    string imgFileType = ".png";
    int imgStartIndex = 0; // first file index to load (assumes Lidar and camera names have identical naming convention)
    int imgEndIndex = 9;   // last file index to load
    int imgFillWidth = 4;  // no. of digits which make up the file index (e.g. img-0001.png)

    for (const auto& detector : detectors)
    {
        string detectorType = detector; // -> SHITOMASI, HARRIS, FAST, BRISK, ORB, AKAZE, SIFT

        string matcherType = "MAT_BF";     // MAT_BF, MAT_FLANN
        string selectorType = "SEL_KNN";       // SEL_NN, SEL_KNN

        for(const auto& descriptor : descriptors)
        {
            string descriptorType = descriptor; // BRISK, BRIEF, ORB, FREAK, AKAZE, SIFT

            // Don't allow cases that result in assert to be ran
            if (descriptorType == "AKAZE" || (descriptorType == "ORB" && detectorType == "SIFT"))
            {
                if(printM8StatsFlag)
                {
                    printNA(detectorType, descriptorType);
                }
                continue;
            }

            string descriptorTypeBin = getCompatibleDescType(descriptorType)[0]; // DES_BINARY, DES_HOG

            std::vector<std::vector<cv::KeyPoint>> keypointsDetections;
            std::vector<float> neighborSizes;
            std::vector<int> numOfMatches;
            std::vector<double> detectionTimes;
            std::vector<double> descTimes;

            int dataBufferSize = 2;       // no. of images which are held in memory (ring buffer) at the same time
            vector<DataFrame> dataBuffer; // list of data frames which are held in memory at the same time
            bool bVis = false;            // visualize results

            for (size_t imgIndex = 0; imgIndex <= imgEndIndex - imgStartIndex; imgIndex++)
            {
                // assemble filenames for current index
                ostringstream imgNumber;
                imgNumber << setfill('0') << setw(imgFillWidth) << imgStartIndex + imgIndex;
                string imgFullFilename = imgBasePath + imgPrefix + imgNumber.str() + imgFileType;

                // load image from file and convert to grayscale
                cv::Mat img, imgGray;
                img = cv::imread(imgFullFilename);
                cv::cvtColor(img, imgGray, cv::COLOR_BGR2GRAY);

                // push image into data frame buffer
                DataFrame frame;
                frame.cameraImg = imgGray;
                dataBuffer.push_back(frame);
                if(dataBuffer.size() > dataBufferSize)
                {
                    // Don't allow vector to grow past defined size
                    dataBuffer.erase(dataBuffer.begin());
                }

                // extract 2D keypoints from current image
                vector<cv::KeyPoint> keypoints; // create empty feature list for current image
                double detectionTime = 0.0;

                if (detectorType.compare("SHITOMASI") == 0)
                {
                    detKeypointsShiTomasi(keypoints, imgGray, detectionTime);
                }
                else if (detectorType.compare("HARRIS") == 0)
                {
                    detKeypointsHarris(keypoints, imgGray, detectionTime);
                }
                else
                {
                    detKeypointsModern(keypoints, imgGray, detectorType, detectionTime);
                }

                detectionTimes.push_back(detectionTime);

                // Focus on preceding car
                cv::Rect vehicleRect(535, 180, 180, 150);
                keypoints.erase(
                        std::remove_if(keypoints.begin(), keypoints.end(), [&vehicleRect](const cv::KeyPoint &keypoint)
                        {
                            return !vehicleRect.contains(keypoint.pt);
                        }), keypoints.end());

                // push keypoints and descriptor for current frame to end of data buffer
                (dataBuffer.end() - 1)->keypoints = keypoints;

                keypointsDetections.push_back(keypoints);
                std::sort(keypoints.begin(), keypoints.end(), [](const cv::KeyPoint& lhs, const cv::KeyPoint& rhs){
                    return lhs.size < rhs.size;
                });

                for(const auto& keypoint : keypoints)
                {
                    neighborSizes.push_back(keypoint.size);
                }

                cv::Mat descriptors;
                double descTime = 0.0;
                descKeypoints((dataBuffer.end() - 1)->keypoints, (dataBuffer.end() - 1)->cameraImg, descriptors, descriptorType, descTime);
                descTimes.push_back(descTime);

                // push descriptors for current frame to end of data buffer
                (dataBuffer.end() - 1)->descriptors = descriptors;

                if (dataBuffer.size() > 1) // wait until at least two images have been processed
                {
                    vector<cv::DMatch> matches;

                    matchDescriptors((dataBuffer.end() - 2)->keypoints, (dataBuffer.end() - 1)->keypoints,
                                     (dataBuffer.end() - 2)->descriptors, (dataBuffer.end() - 1)->descriptors,
                                     matches, descriptorTypeBin, matcherType, selectorType);

                    numOfMatches.push_back(matches.size());

                    // store matches in current data frame
                    (dataBuffer.end() - 1)->kptMatches = matches;
                }

            } // eof loop over all images

            if(printM7StatsFlag)
            {
                printStats(detectorType, keypointsDetections, neighborSizes);
            }

            if(printM8StatsFlag)
            {
                printM8Stats(detectorType, descriptorType, numOfMatches);
            }

            if(printM9StatsFlag)
            {
                printM9Stats(detectorType, descriptorType, numOfMatches, detectionTimes, descTimes);
                prepareM9StatsRanking(detectorType, descriptorType, numOfMatches, detectionTimes, descTimes, records);
            }
        }
    }

    if(printM9StatsFlag)
    {
        std::cout << "======================= TOP 3 ================================" << std::endl;
        std::sort(records.begin(), records.end(), [](const StatHolder& lhs, const StatHolder& rhs){
            return lhs.totalScore > rhs.totalScore;
        });

        if(records.size() > 0)
        {
            for(auto i = 0; i < 3; i++)
            {
                std::cout << "| " << records[i].detector << " / " << records[i].descriptor
                          << " | " << records[i].averageMatches
                          << " | " << records[i].averageDetTime << " ms"
                          << " | " << records[i].averageDescTime << " ms"
                          << " | " << records[i].averageDetTime + records[i].averageDescTime << " ms"
                          << " | " << records[i].totalScore // Total score avg matches / avg time
                          << std::endl;
            }
        }
    }
}

/* MAIN PROGRAM */
int main(int argc, const char *argv[])
{
    bool measure = true;

    if(!measure)
    {
        debugRoutine();
    }
    else
    {
        measurementRoutine();
    }

    return 0;
}
