 
CC=g++
 
CFLAGS= -I/usr/local/include -I/usr/include -L/usr/lib -std=c++11 -lstdc++ -lpthread -L/usr/local/lib -lopencv_imgcodecs -lopencv_highgui -lopencv_core -lopencv_videoio -lopencv_video -lopencv_calib3d -lopencv_imgproc -lopencv_features2d

all: 
	$(CC) *.cpp $(CFLAGS) -o ProgExe


clean:
	rm ProgExe
