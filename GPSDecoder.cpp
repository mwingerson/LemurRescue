#include "GPSDecoder.h"

GPSDecoder::GPSDecoder(std::string paramInput)
{
	initGPS(paramInput);
}

GPSDecoder::~GPSDecoder()
{
	UARTStream.Close();
}

void GPSDecoder::crunchGPSSentence(std::string inputString)
{
	//get GPS senetence name
	std::string GPSSentenceName = inputString.substr(3,3);

	//Make copy of inputstring in a char array
	char* GPSSentence = new char[inputString.length()];
	for(int i=0; i<inputString.length();i++)
		GPSSentence[i]=inputString[i];
	GPSSentence[inputString.length()] = '\0';

  if(GPSSentenceName.compare("GGA")==0)
    readGGAData(GPSSentence);
  else if(GPSSentenceName.compare("GSA")==0)
    readGSAData(GPSSentence);
  else if(GPSSentenceName.compare("GSV")==0)
		readGSVData(GPSSentence);
  else if(GPSSentenceName.compare("GLL")==0)
		readGLLData(GPSSentence);
  else if(GPSSentenceName.compare("RMC")==0)
    readRMCData(GPSSentence);
  else if(GPSSentenceName.compare("TXT")==0)
		readTXTData(GPSSentence);
  else if(GPSSentenceName.compare("VTG")==0)
		readVTGData(GPSSentence);
  else{}
    //std::cout << "Other message: " << GPSSentenceName << std::endl;
}

void GPSDecoder::printGGAData()
{
  std::cout << "GGAData--------------------"
	 	<< "\nfixTime: " << GGAData.GGAfixTime
		<< "\nlatitude: " << GGAData.GGALatitude
		<< "\nlongitude: " << GGAData.GGALongitude
		<< "\ngps_fix: " << GGAData.gps_fix
    << "\nsatNum: " << GGAData.satNum
		<< "\nhorzDOP: " << GGAData.horzDOP
    << "\nalt: " << GGAData.alt
		<< "\nheightOfGeoid: " << GGAData.heightOfGeoid
    << std::endl;
}

void GPSDecoder::printGSAData()
{
	std::cout << "GSAData--------------------"
		<< "\nautoSelect: " << GSAData.autoSelect
		<< "\nfixTime: "<< GSAData.GPSFix
		<< "\nPDOP: " << GSAData.PDOP
		<< "\nHDOP: "<< GSAData.HDOP
		<< "\nVDOP: " << GSAData.VDOP
		<< std::endl;
}

void GPSDecoder::printGSVData()
{

	std::cout << "GSVData--------------------"
						<< "\nFullDataSentNum: " 	<< GSVData.fullDataSentNum
						<< "\nsentence: "					<< GSVData.sentence
						<< "\nsateliteInView: " 	<< GSVData.sateliteInView
						<< "\nsatPRNNum: " 				<< GSVData.satPRNNum
						<< "\nelevation: " 				<< GSVData.elevation
						<< "\nazimuth: " 					<< GSVData.azimuth
						<< std::endl;
}

void GPSDecoder::printGLLData()
{
	std::cout << "GLLData--------------------"
						<< "\nGLLLatitude: " 			<< GLLData.GLLLatitude
						<< "\nGLLLongitude: "			<< GLLData.GLLLongitude
						<< "\nGLLfixTakenAt: "		<< GLLData.GLLfixTakenAt
						<< "\ndataActive: "				<< GLLData.dataActive
						<< std::endl;
}

void GPSDecoder::printRMCData()
{
	std::cout << "RMCData--------------------"
						<< "\nRMCFixTaken: " 			<< RMCData.RMCFixTaken
						<< "\nRMCStatus: "			<< RMCData.RMCStatus
						<< "\nRMCLatitude: "		<< RMCData.RMCLatitude
						<< "\nRMCLongitude: "			<< RMCData.RMCLongitude
						<< "\nRMCGNDSpeed: "			<< RMCData.RMCGNDSpeed
						<< "\nRMCTrackAngle: "			<< RMCData.RMCTrackAngle
						<< "\nRMCMagneticVar: "			<< RMCData.RMCMagneticVar
						<< std::endl;
}

void GPSDecoder::readGGAData(char* sentence)
{
  char* token;
	std::string tempStr;

	std::cout << "sentence: " << sentence << std::endl;

	token = std::strtok(sentence, ",");
  token = std::strtok(NULL, ",");
  GGAData.GGAfixTime = token;

	//Computing GGAData.Latitude
  token = std::strtok(NULL, ",");
  GGAData.GGALatitude = token;
	tempStr = token;
	std::string GPSdegrees = tempStr.substr(0,2);
	GGAData.GGALatitudeNum = atof(GPSdegrees.c_str());
	std::string GPSMinutes = tempStr.substr(2,8);
	float tempfloat = (atof(GPSMinutes.c_str())/60.0);
	GGAData.GGALatitudeNum += tempfloat;
	token = std::strtok(NULL, ",");
	if(strcmp(token, "S") == 0)
	{
		GGAData.GGALatitudeNum *= -1;
	}

  token = std::strtok(NULL, ",");
  std::cout << "longitude: " << token << std::endl;
  GGAData.GGALongitude = token;
	tempStr = token;
	GPSdegrees = tempStr.substr(0,2);
	GGAData.GGALongitudeNum = atof(GPSdegrees.c_str());
	GPSMinutes = tempStr.substr(2,8);
	tempfloat = (atof(GPSMinutes.c_str())/60.0);
	GGAData.GGALongitudeNum += tempfloat;
	token = std::strtok(NULL, ",");
	if(strcmp(token, "W") == 0)
	{
		GGAData.GGALongitudeNum *= -1;
	}
	std::cout << "longitude: " << GGAData.GGALongitudeNum << std::endl;


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
}

void GPSDecoder::readGSAData(char* sentence)
{
	//Start toking
	char* token = std::strtok(sentence, ",");
	//std::cout << "preambl: " << token << std::endl;

	token = std::strtok(NULL, ",");
	//std::cout << "autoSelect: " << token << std::endl;
	GSAData.autoSelect = token;

	token = std::strtok(NULL, ",");
	//std::cout << "fixTime: " << token << std::endl;
	GSAData.GPSFix = atof(token);

	token = std::strtok(NULL, ",");
	//std::cout << "PDOP: " << token << std::endl;
	GSAData.PDOP = atof(token);

	token = std::strtok(NULL, ",");
	//std::cout << "HDOP: " << token << std::endl;
	GSAData.HDOP = atof(token);

	token = std::strtok(NULL, ",");
	//std::cout << "VDOP: " << token << std::endl;
	GSAData.VDOP = atof(token);
}

void GPSDecoder::readGSVData(char* sentence)
{
	//Start toking
	char* token = std::strtok(sentence, ",");
	//std::cout << "preambl: " << token << std::endl;

	token = std::strtok(NULL, ",");
	//std::cout << "autoSelect: " << token << std::endl;
	GSVData.fullDataSentNum = atoi(token);

	token = std::strtok(NULL, ",");
	//std::cout << "fixTime: " << token << std::endl;
	GSVData.sentence = atoi(token);

	token = std::strtok(NULL, ",");
	//std::cout << "PDOP: " << token << std::endl;
	GSVData.sateliteInView = atoi(token);

	token = std::strtok(NULL, ",");
	//std::cout << "HDOP: " << token << std::endl;
	GSVData.satPRNNum = atoi(token);

	token = std::strtok(NULL, ",");
	//std::cout << "VDOP: " << token << std::endl;
	GSVData.azimuth = atoi(token);

}

void GPSDecoder::readGLLData(char* sentence)
{
	//Start toking
	char* token = std::strtok(sentence, ",");
	//std::cout << "preambl: " << token << std::endl;

	token = std::strtok(NULL, ",");
	//std::cout << "autoSelect: " << token << std::endl;
	GLLData.GLLLatitude = token;
	token = std::strtok(NULL, ",");
	GLLData.GLLLatitude.append(" ");
	GLLData.GLLLatitude.append(token);

	token = std::strtok(NULL, ",");
	//std::cout << "fixTime: " << token << std::endl;
	GLLData.GLLLongitude = token;
	token = std::strtok(NULL, ",");
	GLLData.GLLLongitude.append(" ");
	GLLData.GLLLongitude.append(token);

	token = std::strtok(NULL, ",");
	//std::cout << "PDOP: " << token << std::endl;
	GLLData.GLLfixTakenAt = token;

	token = std::strtok(NULL, ",");
	//std::cout << "VDOP: " << token << std::endl;
	GLLData.dataActive = token;

}

void GPSDecoder::readRMCData(char* sentence)
{
    char* token;

    token = std::strtok(sentence, ",");
    //std::cout << "preambl: " << token << std::endl;

    token = std::strtok(NULL, ",");
    //std::cout << "fixTime: " << token << std::endl;
    RMCData.RMCFixTaken = token;

    token = std::strtok(NULL, ",");
    //std::cout << "fixTime: " << token << std::endl;
    RMCData.RMCStatus = token;

		token = std::strtok(NULL, ",");
		//std::cout << "fixTime: " << token << std::endl;
		RMCData.RMCLatitude = token;
		token = std::strtok(NULL, ",");
		RMCData.RMCLatitude.append(" ");
		RMCData.RMCLatitude.append(token);

		token = std::strtok(NULL, ",");
		//std::cout << "fixTime: " << token << std::endl;
		RMCData.RMCLongitude = token;
		token = std::strtok(NULL, ",");
		RMCData.RMCLongitude.append(" ");
		RMCData.RMCLongitude.append(token);

		token = std::strtok(NULL, ",");
		//std::cout << "fixTime: " << token << std::endl;
		RMCData.RMCGNDSpeed = token;

		token = std::strtok(NULL, ",");
		//std::cout << "fixTime: " << token << std::endl;
		RMCData.RMCTrackAngle = token;

		token = std::strtok(NULL, ",");
		//std::cout << "fixTime: " << token << std::endl;
		RMCData.RMCMagneticVar = token;
}

void GPSDecoder::readTXTData(char* sentence)
{
	//start
  //std::cout << "TXT: " << sentence << std::endl;
}

void GPSDecoder::readVTGData(char* sentence)
{
    char* token;

    token = std::strtok(sentence, ",");
    //std::cout << "preambl: " << token << std::endl;

    token = std::strtok(NULL, ",");
    //std::cout << "fixTime: " << token << std::endl;
    VTGData.VTGTrueTrack = token;

    token = std::strtok(NULL, ",");
    //std::cout << "fixTime: " << token << std::endl;
    VTGData.VTGMagTrack = token;

    token = std::strtok(NULL, ",");
    //std::cout << "fixTime: " << token << std::endl;
    VTGData.VTGGndSpdKnots = token;

    token = std::strtok(NULL, ",");
    //std::cout << "fixTime: " << token << std::endl;
    VTGData.VTGGndSpdkmph = token;

}

int GPSDecoder::initGPS(std::string paramInput)
{
	//start
	std::cout << "Initializing GPS on: " << paramInput << std::endl;

	UARTStream.Open(paramInput);
	UARTStream.SetBaudRate(SerialStreamBuf::BAUD_38400);

	if(UARTStream.IsOpen()) {
		//std::cout << "Serial Port Opened" << std::endl;
		return 1;
	}
	else {
		//std::cout << "Port not opened" << std::endl;
		return 0;
	}
}

void GPSDecoder::run()
{
	while(runGPSWorker)
	{
		std::string inputString;
		UARTStream >> inputString;

		if(inputString.length() > 6)
		{
			crunchGPSSentence(inputString);
		}
		usleep(10000);
	}

}
