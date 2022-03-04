OBJFILES = test.o words.o
TARGET = league-l
CXXFLAGS = -std=c++17 -I../../json/include
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

