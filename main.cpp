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

using namespace LibSerial;
//using namespace cv;

int main(int argc, char** argv )
{
    std::cout << "Hello World" << std::endl;

    std::string paramInput = "/dev/ttyACM0";

    GPSDecoder GPSWorker(paramInput);
    std::thread GPSThread(&GPSDecoder::run, &GPSWorker);

    usleep(100000); //100ms

    while (GPSWorker.iterator < 15)
  	{
  		std::cout << "-" << std::endl;
      usleep(30000); //30ms
  	}

  	GPSWorker.runGPSWorker = false;

  	GPSThread.join();


		return 0;
}
