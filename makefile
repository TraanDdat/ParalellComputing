CXX = g++
CXXFLAGS = -std=c++17 -O0 -Wall -pthread
CPPFLAGS = -Iinclude
TARGET = benchmark
SRCS = src/main.cpp src/benchmark.cpp src/TASLock.cpp src/TTASLock.cpp src/MCSLock.cpp
SRCS_TEST = test/test.cpp src/TASLock.cpp src/TTASLock.cpp src/MCSLock.cpp
TARGET_TEST = test
$(TARGET): $(SRCS)
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) $(SRCS) -o $(TARGET)

test: $(SRCS_TEST_TASLOCK)
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) $(SRCS_TEST) -o $(TARGET_TEST)

clean:
	rm -f $(TARGET) $(TARGET_TEST_TASLOCK)
