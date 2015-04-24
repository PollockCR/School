OS_Phase_3: clean OS_Phase_3.o Process_Control_Block.o OS_Control_Block.o
	g++ -std=c++0x -Wall OS_Phase_3.o Process_Control_Block.o OS_Control_Block.o -o OS_Phase_3 -lpthread
Process_Control_Block.o: Process_Control_Block.cpp Process_Control_Block.h
	g++ -std=c++0x -Wall -c Process_Control_Block.cpp Process_Control_Block.h -lpthread
OS_Control_Block.o: OS_Control_Block.cpp OS_Control_Block.h
	g++ -std=c++0x -Wall -c OS_Control_Block.cpp OS_Control_Block.h -lpthread
OS_Phase_3.o: OS_Phase_3.cpp 
	g++ -std=c++0x -Wall -c OS_Phase_3.cpp -lpthread
clean:
	rm -f *.o
	rm -f *.gch
	@echo "clean complete"