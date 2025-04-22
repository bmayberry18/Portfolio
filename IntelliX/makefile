CXX = g++
CXXFLAGS = -std=c++20
SRCS = parse.cpp tokenize.cpp error_messages.cpp array.cpp loops.cpp math.cpp
TARGET = intellix

$(TARGET): $(SRCS)
	$(CXX) $(CXXFLAGS) $(SRCS) -o $(TARGET)

clean:
	rm -f $(TARGET)