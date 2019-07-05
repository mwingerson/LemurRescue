#pragma once

#include <iostream>
#include <string>


class GPSDecoder
{
public:
	GPSDecoder();
	~GPSDecoder();

	void run();
	int initGPS(std::string);


	//std::basic_ifstream UARTFile;
	int iterator = 0;
	bool runGPSWorker = true;

};
