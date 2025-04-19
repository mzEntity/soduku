# 编译器和参数
CXX = g++
CXXFLAGS = -std=c++20 -Wall -Wextra -g -Iinclude

# 所有源文件（包括主函数和 src 下的所有 .cpp 文件）
SRCS = main.cpp $(wildcard src/**/*.cpp) $(wildcard src/*.cpp)

# 源文件对应的目标文件（把 .cpp 改为 .o）
OBJS = $(SRCS:.cpp=.o)

# 最终的可执行文件名
TARGET = main

# 默认目标
all: $(TARGET)

# 链接
$(TARGET): $(OBJS)
	$(CXX) $(CXXFLAGS) -o $@ $^

# 生成每个 .o 文件
%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

# 清理编译生成的文件
clean:
	rm -f $(OBJS) $(TARGET)
