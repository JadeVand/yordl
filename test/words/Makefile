OBJFILES = test.o ../../urand.o ../../actor.o ../../words.o
TARGET = test
CXXFLAGS = -std=c++17 -I../../json/include -I../../include -I../../uWebSockets/src -I../../uSockets/src
LDFLAGS =
DESTDIR ?=
prefix ?= /usr/local

.PHONY : all
all : $(OBJFILES)
	$(CXX) -flto -O3 -o $(TARGET) $(CXXFLAGS) $(OBJFILES) $(LDFLAGS)

.PHONE : clean
clean:
	rm -rf $(TARGET)
	rm -rf $(TARGET).o

