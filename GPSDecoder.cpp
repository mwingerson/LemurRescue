#include "GPSDecoder.h"

GPSDecoder::GPSDecoder()
{
}


GPSDecoder::~GPSDecoder()
{
}

int GPSDecoder::initGPS(std::string serialString)
{
	// 
	// std::cout << "Init: " << serialString << std::endl;
	//
	// UARTFile.open(serialString);

	//this->UARTFile = new std::basic_fstream(serialString);

	//UARTFile.open(serialString);

	// if(UARTFile->is_open()) {
	// 	//std::cout << "Serial Port Opened" << std::endl;
	// 	return 1;
	// }
	// else {
	// 	//std::cout << "Port not opened" << std::endl;
	// 	return 0;
	// }
	return 0;
}

void GPSDecoder::run()
{


}
