cmake_minimum_required(VERSION 2.8)
project( SearchNRescue )
find_package( OpenCV REQUIRED )

include_directories( ${OpenCV_INCLUDE_DIRS} )
add_library(GPSDecoder GPSDecoder.cpp)

add_executable( SearchNRescue main.cpp )

message(WARNING "OPENCV_LIBS: ${OpenCV_LIBS}")

target_link_libraries( SearchNRescue GPSDecoder serial pthread ${OpenCV_LIBS} )
