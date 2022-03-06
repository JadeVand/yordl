OBJFILES = main.o actor.o uSockets/*.o words.o urand.o
TARGET = league-l
CXXFLAGS = -Iopenssl/include -Izlib -Ijson/include -IuWebSockets/src -IuSockets/src -lpthread -Wpedantic -Wall -Wextra -Wsign-conversion -Wconversion -std=c++17 -Ideps/include -Ideps/uSockets/src -Iinclude -g
LDFLAGS = -Lopenssl -Lzlib -lstdc++ -lssl -lcrypto -lz
DESTDIR ?=
prefix ?= /usr/local

.PHONY : all
all : $(OBJFILES)
	$(CXX) -flto -O3 -o $(TARGET) $(CXXFLAGS) $(OBJFILES) $(LDFLAGS)

.PHONE : clean
clean:
	rm -rf $(TARGET)
	rm *.o

