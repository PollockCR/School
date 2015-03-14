simulator: OS_Phase_1.o Process_Control_Block.o
	g++ -std=c++11 -Wall OS_Phase_1.o Process_Control_Block.o -o simulator -lpthread
Process_Control_Block.o: Process_Control_Block.cpp Process_Control_Block.h
	g++ -std=c++11 -Wall -c Process_Control_Block.cpp Process_Control_Block.h -lpthread
OS_Phase_1: OS_Phase_1.cpp 
	g++ -std=c++11 -Wall -c OS_Phase_1.cpp -lpthread