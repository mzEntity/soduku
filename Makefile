CXX = g++
CXXFLAGS = -std=c++20 -Wall -Wextra -g -Iinclude

TARGET = main
SRCS = main.cpp src/debug_utils/debug_utils.cpp
OBJS = $(SRCS:.cpp=.o)

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CXX) $(CXXFLAGS) -o $@ $^

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	rm -f $(TARGET) $(OBJS)
