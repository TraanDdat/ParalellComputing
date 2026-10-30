CXX = g++
CXXFLAGS = -std=c++17 -O2 -Wall -pthread -fopenmp
LDFLAGS  = -static-libgcc -static-libstdc++ -static -fopenmp
CPPFLAGS = -Iinclude
TARGET = build/benchmark.exe
SRCS = src/main.cpp src/benchmark.cpp src/TASLock.cpp src/TTASLock.cpp src/MCSLock.cpp src/PTHREADMUTEXLock.cpp src/PTHREADSPINLock.cpp src/OPENMPLock.cpp
SRCS_TEST = test/test.cpp src/TASLock.cpp src/TTASLock.cpp src/MCSLock.cpp src/PTHREADMUTEXLock.cpp src/PTHREADSPINLock.cpp src/OPENMPLock.cpp
TARGET_TEST = build/test_out.exe
$(TARGET): $(SRCS)
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) $(SRCS) -o $(TARGET) $(LDFLAGS)

buildtest: $(SRCS_TEST)
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) $(SRCS_TEST) -o $(TARGET_TEST)

clean:
	rm -f $(TARGET) $(TARGET_TEST)