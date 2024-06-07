XX := g++
CXXFLAGS := -std=c++11 -Wall
LDFLAGS := -lcurl
SRC := client.cpp
OBJ := $(SRC:.cpp=.o)
TARGET := assignment_3

.PHONY: all clean

all: $(TARGET)

$(TARGET): $(OBJ)
	$(CXX) $(CXXFLAGS) -o $@ $^ $(LDFLAGS)

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c -o $@ $<

clean:
	rm -f $(OBJ) $(TARGET)

