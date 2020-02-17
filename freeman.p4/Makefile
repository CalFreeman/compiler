CC = g++
CFLAGS = -g
TARGET = compile
OBJS = main.o testScanner.o scanner.o parser.o treePrint.o st.o codeGenerator.o

$(TARGET): $(OBJS)
	$(CC) -o $(TARGET) $(OBJS)

main.o: main.cpp
	$(CC) $(CFLAGS) -c main.cpp

testScanner.o: testScanner.cpp
	$(CC) $(CLFAGS) -c testScanner.cpp

scanner.o: scanner.cpp
	$(CC) $(CFLAGS) -c scanner.cpp

parser.o: parser.cpp
	$(CC) $(CFLAGS) -c parser.cpp

treePrint.o: treePrint.cpp
	$(CC) $(CFLAGS) -c treePrint.cpp

st.o: st.cpp
	$(CC) $(CFLAGS) -c st.cpp

codeGenerator.o: codeGenerator.cpp
	$(CC) $(CFLAGS) -c codeGenerator.cpp

clean:
	/bin/rm -f *.o $(TARGET)
