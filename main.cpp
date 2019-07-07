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
    std::cout << "Search and Rescue Menu V0.1" << std::endl;

    while(GPSWorker.runGPSWorker)
    {
      char c;
      std::system("clear");
      GPSWorker.printGGAData();
      usleep(100000); //100ms

      // std::cout << "Enter Menu Option: ";
      // std::cin >> c;
      //
      // switch (c) {
      //   case 'a':
      //     std::system("clear");
      //     GPSWorker.printGGAData();
      //     break;
      //
      //   case 'q':
      //     std::cout << "Quitting" << std::endl;
      //     GPSWorker.runGPSWorker = false;
      //     break;
      //
      //   default:
      //     std::cout << "Incorrect user input" << std::endl;
      // }
    }

  	GPSThread.join();

		return 0;
}
