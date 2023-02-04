test_interpreter: test_interpreter.cpp Call.o Definition.o Pin.o
	g++ -g test_interpreter.cpp Call.cpp Definition.cpp Pin.cpp -o test_interpreter -Wall

%.o: %.cpp %.hpp
	g++ -Wall -g -c -o $@ $<
