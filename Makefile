
C++ = g++

CCFLAGS = -Wall -finline-functions -g -lcrypto -lpthread-D_LARGE_FILE_SOURCE=1
LDFLAGS =  -L ../src -lstdc++ -lpthread -lm -lssl -lcrypto -g

# %.o: %.cpp 
	# $(C++) $(CCFLAGS) $< -c

example: crypto.o example.o
	$(C++) $^ -o example $(LDFLAGS)

clean:
	rm -f *.o $(APPOUT) 