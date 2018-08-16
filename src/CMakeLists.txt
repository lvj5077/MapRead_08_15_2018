SET(CMAKE_CXX_FLAGS "-std=c++0x")

SET("OpenCV_DIR" "/home/jin/Third_Party_Packages/opencv-2.4.13/build")
FIND_PACKAGE( OpenCV REQUIRED )
include_directories(/usr/include/eigen3)


ADD_LIBRARY( imageOperators imageOperators.cpp )

ADD_EXECUTABLE( main main.cpp )
TARGET_LINK_LIBRARIES( main
    imageOperators
    ${OpenCV_LIBS})


# ADD_EXECUTABLE( testH testH.cpp )
# TARGET_LINK_LIBRARIES( 
#     ${OpenCV_LIBS})