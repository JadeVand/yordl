OBJFILES = main.o fileio.o deps/uSockets/*.o
TARGET = league-l
CXXFLAGS = -Ideps/include -lpthread -Wpedantic -Wall -Wextra -Wsign-conversion -Wconversion -std=c++ -Ideps/include -Ideps/uSockets/src -Iinclude -g
LDFLAGS = -Ldeps/lib -lstdc++ -lssl -lcrypto -llibuv -lz
DESTDIR ?=
prefix ?= /usr/local

.PHONY : all
all : $(OBJFILES)
	$(CXX) -flto -O3 -o $(TARGET) $(CXXFLAGS) $(OBJFILES) $(LDFLAGS)

.PHONE : clean
clean:
	rm -rf $(TARGET)
	rm -rf $(TARGET).o

