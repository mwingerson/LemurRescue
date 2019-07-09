#include "GPSDecoder.h"

GPSDecoder::GPSDecoder(){ }

GPSDecoder::~GPSDecoder()
{
	UARTStream.Close();
	this->closeFile();
}

int GPSDecoder::initDecoder(std::string paramInput)
{
	int ret = 0;
	ret = initGPS(paramInput);
	ret += initFiles();
	if(ret)
		return 1;
	else
		return 0;

}

int GPSDecoder::initGPS(std::string paramInput)
{
	//start
	std::cout << "Initializing GPS on: " << paramInput << std::endl;

	UARTStream.Open(paramInput);
	UARTStream.SetBaudRate(SerialStreamBuf::BAUD_38400);

	if(UARTStream.IsOpen()) {
		//std::cout << "Serial Port Opened" << std::endl;
		return 0;
	}
	else {
		//std::cout << "Port not opened" << std::endl;
		return 1;
	}
}

int GPSDecoder::initFiles()
{
	//initialize the KML file
	file.open("KMLOutput.kml", std::ios::out);\
	if(file.is_open())
	{
		file 	<< "<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n"
					<< "<kml xmlns=\"http://www.opengis.net/kml/2.2\">\n"
					<< "\t<Document>\n"
					<< "\t\t<name>testing kml</name>\n"
					<< "\t\t<description/>\n"
					<< "\t\t<Style id=\"test_line\">"
					<< "\t\t\t<IconStyle><Icon>\n"
					<< "\t\t\t\t<href>http://maps.google.com/mapfiles/kml/pal3/icon19.png</href>\n"
					<< "\t\t\t</Icon></IconStyle>\n"
					<< "\t\t\t<LineStyle><color>ff0000ff</color><width>8</width></LineStyle>\n"
					<< "\t\t</Style>\n"
					<< "\t\t\t<Placemark>\n"
					<< "\t\t\t\t<name>Placemark Name</name>\n"
					<< "\t\t\t\t<styleUrl>#test_line</styleUrl>\n"
					<< "\t\t\t\t<LineString>\n"
					<< "\t\t\t\t\t<tessellate>1</tessellate>\n"
					<< "\t\t\t\t\t<coordinates>\n";

		file.close();
		return 1;
	}
	else
		return 0;
}

int GPSDecoder::printKMLData()
{
	file.open("KMLOutput.kml", std::ios::app);
	if(file.is_open() && ((GGAData.gps_fix == 1) || (GGAData.gps_fix == 2)))
	{
		file.precision(10);

		file << "\t\t\t\t\t\t"
		<< GGAData.GGALongitudeNum << ","
		<< GGAData.GGALatitudeNum << "," << 0
		<< "\n";

		// std::cout << "print KML "
		// << GGAData.GGALongitudeNum << ","
		// << GGAData.GGALatitudeNum << "," << 0
		// << std::endl;
		file.close();
		return 1;
	}
	else
		return 0;
}

void GPSDecoder::closeFile()
{
	file.open("KMLOutput.kml", std::ios::app);
	if(file.is_open())
	{
		file 	<< "\t\t\t\t\t</coordinates>\n"
					<< "\t\t\t\t</LineString>\n"
					<< "\t\t\t</Placemark>\n"
					<< "\t</Document>\n"
					<< "</kml>"
					<< std::endl;
		file.close();
	}
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
	// Fix quality:
	// 		0 = invalid
	// 		1 = GPS fix (SPS)
	// 		2 = DGPS fix
	// 		3 = PPS fix
	// 		4 = Real Time Kinematic
	// 		5 = Float RTK
	// 		6 = estimated (dead reckoning) (2.3 feature)
	// 		7 = Manual input mode
	// 		8 = Simulation mode
  std::cout << "GGAData--------------------"
	 	<< "\nfixTime:\t" << GGAData.GGAfixTime
		<< "\nlatitude:\t" << GGAData.GGALatitudeNum
		<< "\nlongitude:\t" << GGAData.GGALongitudeNum
		<< "\nGPS fix:\t" << GGAData.gps_fix
    << "\nSatelinte num:\t" << GGAData.satNum
		<< "\nHorzDOP:\t" << GGAData.horzDOP
    << "\nAltitude:\t" << GGAData.alt
		<< "\nHeightOfGeoid:\t" << GGAData.heightOfGeoid
    << std::endl;

		std::ofstream file("GPSOutput.txt", std::ios::app);
		if(file.is_open())
		{
			std::cout << "Opened file successfully" << std::endl;
			file << "GGAData--------------------"
			 	<< "\nfixTime:\t" << GGAData.GGAfixTime
				<< "\nlatitude:\t" << GGAData.GGALatitudeNum
				<< "\nlongitude:\t" << GGAData.GGALongitudeNum
				<< "\nGPS fix:\t" << GGAData.gps_fix
		    << "\nSatelinte num:\t" << GGAData.satNum
				<< "\nHorzDOP:\t" << GGAData.horzDOP
		    << "\nAltitude:\t" << GGAData.alt
				<< "\nHeightOfGeoid:\t" << GGAData.heightOfGeoid
		    << std::endl;

			file.close();
		}
		else
		{
			std::cout << "Cannot open file" << std::endl;
		}

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
	int latlonglength;
	//std::cout << "sentence: " << sentence << std::endl;
	float tempFlt;
	token = std::strtok(sentence, ",");
  token = std::strtok(NULL, ",");
  GGAData.GGAfixTime = token;

	//Computing GGAData.Latitude
  GGAData.GGALatitude = std::strtok(NULL, ",");
	latlonglength = GGAData.GGALatitude.length();

	std::string GPSdegrees = GGAData.GGALatitude.substr(0,latlonglength-8);
	std::string GPSMinutes = GGAData.GGALatitude.substr(latlonglength-8,8);

	tempFlt = atof(GPSdegrees.c_str());
	GGAData.GGALatitudeNum = (float)tempFlt;
	std::cout.precision(10);

	tempFlt = (atof(GPSMinutes.c_str())/60.000000);
	GGAData.GGALatitudeNum += tempFlt;

	token = std::strtok(NULL, ",");
	if(strcmp(token, "S") == 0)
		GGAData.GGALatitudeNum *= -1;

	// std::cout << "GGALatitude: " << GGAData.GGALatitude << std::endl;
	// std::cout << "GPSdegrees: "<< GPSdegrees <<std::endl;
	// std::cout << "GPSminutes: "<< GPSMinutes <<std::endl;
	// std::cout << "GGALatitudeNum: " << GGAData.GGALatitudeNum << std::endl;

  GGAData.GGALongitude = std::strtok(NULL, ",");
	latlonglength = GGAData.GGALongitude.length();

	GPSdegrees = GGAData.GGALongitude.substr(0,latlonglength-8);
	GPSMinutes = GGAData.GGALongitude.substr(latlonglength-8,8);

	GGAData.GGALongitudeNum = atof(GPSdegrees.c_str());
	GGAData.GGALongitudeNum += (atof(GPSMinutes.c_str())/60.000000);
	token = std::strtok(NULL, ",");
	if(strcmp(token, "W") == 0)
		GGAData.GGALongitudeNum *= -1;

	// std::cout << "longitude: " << GGAData.GGALongitude << std::endl;
	// std::cout << "GPSdegrees: "<< GPSdegrees <<std::endl;
	// std::cout << "GPSminutes: "<< GPSMinutes <<std::endl;
	// std::cout << "GGALongitudeNum: " << GGAData.GGALongitudeNum << std::endl;

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
void GPSDecoder::run()
{
	std::string inputString;

	while(runGPSWorker)
	{
		UARTStream >> inputString;
		if(inputString.length() > 6)
		{
			crunchGPSSentence(inputString);

			if(!printKMLData())
			{
				//std::cout << "ERROR" << std::endl;
			}
		}
		usleep(50);
	}

}
