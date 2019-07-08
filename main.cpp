//#include <opencv2/opencv.hpp>
#include <iostream>
#include <fstream>
#include <cstring>
#include <cstdio>

#include "GPSDecoder.h"
#include <unistd.h>
#include <SerialPort.h>
#include <SerialStream.h>

#include <thread>

#include <opencv2/highgui.hpp>  // OpenCV window I/O

using namespace LibSerial;
using namespace cv;

int main(int argc, char** argv )
{
    std::cout << "Hello World" << std::endl;

    std::string paramInput = "/dev/ttyACM0";

    GPSDecoder GPSWorker(paramInput);
    std::thread GPSThread(&GPSDecoder::run, &GPSWorker);

    usleep(100000); //100ms
    std::cout << "Search and Rescue Menu V0.1" << std::endl;

    while(GPSWorker.runGPSWorker)
    {
      //std::system("clear");
      //GPSWorker.printGGAData();
      //usleep(100000); //100ms

      //char c = (char)waitKey(100);
      // if(c == 27)
      // break;
    }

  	GPSThread.join();

		return 0;
}
