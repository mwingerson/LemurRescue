

#include <iostream>
#include <fstream>
#include <cstring>
#include <cstdio>

#include "GPSDecoder.h"
#include <unistd.h>
#include <SerialPort.h>
#include <SerialStream.h>

#include <thread>

#include <opencv2/opencv.hpp>  // OpenCV window I/O

using namespace LibSerial;
using namespace cv;

int main(int argc, char** argv )
{

  std::string paramInput = "/dev/ttyACM0";

  GPSDecoder GPSWorker;
  if(!GPSWorker.initDecoder(paramInput))
  {
    std::cout << "Failed to initialize GPSWorker. Closing" << std::endl;
    waitKey(5000);
    return 0;
  }

  std::thread GPSThread(&GPSDecoder::run, std::ref(GPSWorker));

  char c;

  while(GPSWorker.runGPSWorker)
  {
    std::system("clear");
    std::cout << "Search and Rescue Menu V0.1" << std::endl;
    GPSWorker.printGGAData();

    namedWindow("Display Image", WINDOW_AUTOSIZE );

    if((char)waitKey(200) == 27)
      GPSWorker.runGPSWorker = false;
  }

	GPSThread.join();
  GPSWorker.stopGPS();

  std::cout << "PROG END" << std::endl;
	return 0;
}
