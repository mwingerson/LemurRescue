//#include <opencv2/opencv.hpp>
#include <iostream>
#include <fstream>
#include <cstring>
#include <cstdio>

//#include "GPSDecoder.h"
#include <unistd.h>
#include <SerialPort.h>
#include <SerialStream.h>

using namespace LibSerial;
//using namespace cv;


struct GGA_struct{
  std::string fixTime;
  std::string latitude;
  std::string latDirection;
  std::string longitude;
  std::string longDirection;
  int gps_fix;
  int satNum;
  float horzDOP;
  float alt;
  float heightOfGeoid;
  int empty1;
  int empty2;
};

struct GSA_struct{
  std::string autoSelect;
  int GPSFix;
};

GGA_struct GGAData;

void readFFFData(char* sentence)
{
    char* token;

    // token = std::strtok(sentence, ",");
    // //std::cout << "preambl: " << token << std::endl;
    //
    // token = std::strtok(NULL, ",");
    // //std::cout << "fixTime: " << token << std::endl;
    // GGAData.fixTime = token;

    std::cout << "FFF: " << sentence << std::endl;
}

void readGGAData(char* sentence)
{
  char* token;

  std::cout << "GGA: " << sentence << std::endl;

  token = std::strtok(sentence, ",");
  //std::cout << "preambl: " << token << std::endl;

  token = std::strtok(NULL, ",");
  //std::cout << "fixTime: " << token << std::endl;
  GGAData.fixTime = token;

  token = std::strtok(NULL, ",");
  //std::cout << "latitude: " << token << std::endl;
  GGAData.latitude = token;

  token = std::strtok(NULL, ",");
  //std::cout << "latDirection: " << token << std::endl;
  GGAData.latDirection = token;

  token = std::strtok(NULL, ",");
  //std::cout << "longitude: " << token << std::endl;
  GGAData.longitude = token;

  token = std::strtok(NULL, ",");
  //std::cout << "longDirection: " << token << std::endl;
  GGAData.longDirection = token;

  token = std::strtok(NULL, ",");
  //std::cout << "gps_fix: " << token << std::endl;
  GGAData.gps_fix = atoi(token);

  token = std::strtok(NULL, ",");
  //std::cout << "sat_num: " << token << std::endl;
  GGAData.satNum = atoi(token);

  token = std::strtok(NULL, ",");
  //std::cout << "horzDOP: " << token << std::endl;
  GGAData.horzDOP = atof(token);

  token = std::strtok(NULL, ",");
  //std::cout << "alt: " << token << std::endl;
  GGAData.alt = atoi(token);
  token = std::strtok(NULL, ",");

  token = std::strtok(NULL, ",");
  //std::cout << "heightOfGeoid: " << token << std::endl;
  GGAData.heightOfGeoid = atof(token);
  token = std::strtok(NULL, ",");

  token = std::strtok(NULL, ",");
  //std::cout << "empty1: " << token << std::endl;
  GGAData.empty1 = atoi(token);

//  std::cout << "GGAData Read" << std::endl;
  // std::cout << "\nfixTime: " << GGAData.fixTime
  //           << "\nlatitude: " << GGAData.latitude
  //           << "\nlatDirection: " << GGAData.latDirection
  //           << "\nlongitude: " << GGAData.longitude
  //           << "\nlongDirection: " << GGAData.longDirection
  //           << "\ngps_fix: " << GGAData.gps_fix
  //           << "\nsatNum: " << GGAData.satNum
  //           << "\nhorzDOP: " << GGAData.horzDOP
  //           << "\nalt: " << GGAData.alt
  //           << "\nheightOfGeoid: " << GGAData.heightOfGeoid
  //           << std::endl;
}

void readGSAData(char* sentence)
{
    char* token;

    // token = std::strtok(sentence, ",");
    // //std::cout << "preambl: " << token << std::endl;
    //
    // token = std::strtok(NULL, ",");
    // //std::cout << "fixTime: " << token << std::endl;
    // GGAData.fixTime = token;

    std::cout << "GSA: " << sentence << std::endl;

}

void readGSVData(char* sentence)
{
    char* token;

    // token = std::strtok(sentence, ",");
    // //std::cout << "preambl: " << token << std::endl;
    //
    // token = std::strtok(NULL, ",");
    // //std::cout << "fixTime: " << token << std::endl;
    // GGAData.fixTime = token;

    std::cout << "GSV: " << sentence << std::endl;

}

void readGLLData(char* sentence)
{
    char* token;

    // token = std::strtok(sentence, ",");
    // //std::cout << "preambl: " << token << std::endl;
    //
    // token = std::strtok(NULL, ",");
    // //std::cout << "fixTime: " << token << std::endl;
    // GGAData.fixTime = token;

    std::cout << "GLL: " << sentence << std::endl;

}

void readRMCData(char* sentence)
{
    char* token;

    // token = std::strtok(sentence, ",");
    // //std::cout << "preambl: " << token << std::endl;
    //
    // token = std::strtok(NULL, ",");
    // //std::cout << "fixTime: " << token << std::endl;
    // GGAData.fixTime = token;

    std::cout << "RMC: " << sentence << std::endl;

}

int main(int argc, char** argv )
{
    std::cout << "Hello World" << std::endl;

    std::string paramInput = "/dev/ttyACM0";

    SerialStream UARTStream;
    UARTStream.Open(paramInput);
    UARTStream.SetBaudRate(SerialStreamBuf::BAUD_38400);
    usleep(100000); //100ms

    for(int i=0; i<30; i++)
    {
      std::string inputString;
      UARTStream >> inputString;

      if(inputString.length() > 6)
      {
//        std::cout <<"String: " << inputString << std::endl;
        std::string GPSSentenceName = inputString.substr(3,3);
        char* GPSSentence = new char[inputString.length()];

        for(int i=0; i<inputString.length();i++)
          GPSSentence[i]=inputString[i];
        GPSSentence[inputString.length()] = '\0';

        if(GPSSentenceName.compare("FFF")==0)
        {
          readGSAData(GPSSentence);
        }
        else if(GPSSentenceName.compare("GGA")==0)
        {
          readGGAData(GPSSentence);
        }
        else if(GPSSentenceName.compare("GSA")==0)
        {
          readGSAData(GPSSentence);
        }
        else if(GPSSentenceName.compare("GSV")==0)
        {
          std::cout << "GSV: " << GPSSentence << std::endl;
        }
        else if(GPSSentenceName.compare("GLL")==0)
        {
          std::cout << "GLL: " << GPSSentence << std::endl;
        }
        else if(GPSSentenceName.compare("RMC")==0)
        {
          std::cout << "RMC: " << GPSSentence << std::endl;
        }
        else if(GPSSentenceName.compare("TXT")==0)
        {
          std::cout << "TXT: " << GPSSentence << std::endl;
        }
        else if(GPSSentenceName.compare("VTG")==0)
        {
          std::cout << "VTG: " << GPSSentence << std::endl;
        }
        else
        {
          std::cout << "Other message: " << GPSSentenceName << std::endl;
        }
      }
      usleep(5000);
    }


  	// if(UARTFille.is_open()) {
  	// 	std::cout << "Serial Port Opened" << std::endl;
  	// 	return 1;
  	// }
  	// else {
  	// 	std::cout << "Port not opened" << std::endl;
  	// 	return 0;
  	// }

    //if(GPSWorker.initGPS(GPSPortString))
    //if(GPSWorker.initGPS("/dev/ttyACM0"))
    // {
    //   std::cout << "GPS Port opened" << std::endl;
    // }
    // else
    // {
    //   std::cout << "GPS Port opened" << std::endl;
    // }


    //  std::string GPSSentenceName = str.substr(3,3);

		return 0;
}
