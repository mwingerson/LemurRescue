

#include <iostream>
#include <fstream>
#include <cstring>
#include <string>
#include <cstdio>
#include <thread>
#include <vector>

#include "GPSDecoder.h"
#include <unistd.h>
#include <SerialPort.h>
#include <SerialStream.h>

#include <opencv2/opencv.hpp>  // OpenCV window I/O

using namespace LibSerial;
using namespace cv;

std::ofstream SNRlogFile;

int openSNRLogFile()
{
  SNRlogFile.open("logs/SNRlog.txt", std::ios::out);
  if(SNRlogFile.is_open())
  {
    SNRlogFile << "SNRLog Header" << std::endl;
    SNRlogFile.close();
    return 1;
	}
	return 0;
}

int printSNRLogFile(std::string str)
{
  SNRlogFile.open("logs/SNRlog.txt", std::ios::app);
  if(SNRlogFile.is_open())
  {
    SNRlogFile << str << std::endl;
    SNRlogFile.close();
    return 1;
	}
  return 0;
}

int main(int argc, char** argv )
{
  std::cout << "PROG START" << std::endl;

  std::string paramInput = "/dev/ttyACM0";
  GPSDecoder GPSWorker(paramInput);

  float arucoSquareDimension = (77.25/1000); //77.25mm
  Mat cameraMatrix = (Mat_<float>(3, 3) << 1417.6317687622784, 0.0, 974.89841365899792, 0.0, 1417.6317687622784, 553.71775511336546, 0.0, 0.0, 1.0);
  Mat distanceCoefficients = (Mat_<float>(1,5) << 1.0471699416678452e-01, - 1.7399948327396764e-01, 0.0, 0.0, 0.0);
  Mat frame;

  HOGDescriptor hog;
//  hog.setSVMDetector(HOGDescriptor::getDaimlerPeopleDetector());
  hog.setSVMDetector(HOGDescriptor::getDefaultPeopleDetector());

  std::vector<Vec3d> rotVec, transVec;

  int frameNum = 0;
  std::string imageStringPrefix = "logs/imageOut";
  std::string imageStringOut;

  openSNRLogFile();

  if(!GPSWorker.initDecoder())
  {
    std::cout << "Failed to initialize GPSWorker. Closing" << std::endl;
    return 0;
  }

  std::thread GPSThread(&GPSDecoder::run, std::ref(GPSWorker));

  VideoCapture vid(2);
  if (!vid.isOpened()) {
    std::cout << "can't Open camera" << std::endl;
    return -1;
  }

  //4:3 aspect ratio resolutions: 640×480, 800×600, 960×720, 1024×768, 1280×960, 1400×1050, 1440×1080, 1600×1200, 1856×1392, 1920×1440, and 2048×1536.
  //16:10 aspect ratio resolutions: 1280×800, 1440×900, 1680×1050, 1920×1200, and 2560×1600.


  vid.set(CAP_PROP_FRAME_WIDTH, 1024);
  vid.set(CAP_PROP_FRAME_HEIGHT, 768);

  namedWindow("Webcam", WINDOW_AUTOSIZE);

  while(GPSWorker.runGPSWorker)
  {
    std::system("clear");
    std::cout << "Search and Rescue Menu V0.1" << std::endl;
    GPSWorker.printGGA();

    if (!vid.read(frame))
    {
      std::cout << "No Frame read in" << std::endl;
      break;
    }

    std::vector<Rect> found, found_filtered;
    hog.detectMultiScale(frame, found, 0, Size(8, 8), Size(32, 32), 1.05, 2);

    size_t i, j;
    for (i = 0; i < found.size(); i++)
    {
      Rect r = found[i];
      for (j = 0; j < found.size(); j++)
        if (j != i && (r & found[j]) == r)
          break;
      if (j == found.size())
        found_filtered.push_back(r);
    }

    for (i = 0; i < found_filtered.size(); i++)
    {
      Rect r = found_filtered[i];
      r.x += cvRound(r.width * 0.1);
      r.width = cvRound(r.width * 0.8);
      r.y += cvRound(r.height * 0.06);
      r.height = cvRound(r.height * 0.9);
      rectangle(frame, r.tl(), r.br(), cv::Scalar(0, 255, 0), 2);

      imageStringOut= imageStringPrefix + std::to_string(frameNum) + ".jpg";
      //std::cout << "Printing to file: " << imageStringOut << std::endl;

      imwrite(imageStringOut, frame);

      std::string SNRLogOutString;
      SNRLogOutString = "image" + std::to_string(frameNum) + ": " + GPSWorker.GGAData.GGAfixTime
      + " - [" + std::to_string((float)GPSWorker.GGAData.GGALatitudeNum) + "," + std::to_string((float)GPSWorker.GGAData.GGALongitudeNum) + "]";
      printSNRLogFile(SNRLogOutString);

      frameNum++;
    }

    imshow("Webcam", frame);

    if((char)waitKey(50) == 27)
      GPSWorker.runGPSWorker = false;
  }

	GPSThread.join();

  std::cout << "PROG END" << std::endl;
	return 0;
}
