OS_Phase_2: clean OS_Phase_2.o Process_Control_Block.o
	g++ -std=c++11 -Wall OS_Phase_2.o Process_Control_Block.o -o OS_Phase_2 -lpthread
Process_Control_Block.o: Process_Control_Block.cpp Process_Control_Block.h
	g++ -std=c++11 -Wall -c Process_Control_Block.cpp Process_Control_Block.h -lpthread
OS_Phase_2.o: OS_Phase_2.cpp 
	g++ -std=c++11 -Wall -c OS_Phase_2.cpp -lpthread
clean:
	rm -f *.o
	rm -f *.gch
	@echo "clean complete"