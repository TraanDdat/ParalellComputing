CPP = g++

CXXFLAGS = -std=c++20 -g -I./include

LDFLAGS = -pthread

# Optional sanitizer: make SANITIZE=thread
ifeq ($(SANITIZE),thread)
    CXXFLAGS += -O1 -fsanitize=thread
    LDFLAGS += -fsanitize=thread
endif

TARGET = exe/mycpp

ifeq ($(STD_BARRIER),true)
	SOURCE = main.cpp
	SOURCE += common_cp.cpp
	SOURCE += thread_design_cp.cpp
else
	SOURCE = main.cpp
	SOURCE += common.cpp
	SOURCE += thread_design.cpp
endif

# VAR = variant of barrier for testing
# TEST_PROB = test problem to run

# initialization of default values for VAR, and TEST_PROB
ifndef VAR
    VAR = 1
endif

ifndef TEST_PROB
    TEST_PROB = 1
endif

ifndef NUM_OF_THREAD
    NUM_OF_THREAD = 3
endif

ifndef BACKOFF
    BACKOFF = 0
endif

ifndef NUM_OF_BARRIER # turn of barrier throughput testing if no needed
	NUM_OF_BARRIER = 0
endif

ifndef THREAD_SLEEP
	THREAD_SLEEP = 0
endif

ifndef STD_BARRIER
    STD_BARRIER = 0
endif

ifndef TOTAL_IT
    TOTAL_IT = 1000
endif

OBJECT = $(SOURCE:%.cpp=obj/%.o)
CXXFLAGS += -DVAR=$(VAR)
CXXFLAGS += -DTEST_PROB=$(TEST_PROB)
CXXFLAGS += -DNUM_OF_THREAD=$(NUM_OF_THREAD)
CXXFLAGS += -DSTD_BARRIER=$(STD_BARRIER)
CXXFLAGS += -DBACKOFF=$(BACKOFF)
CXXFLAGS += -DNUM_OF_BARRIER=$(NUM_OF_BARRIER)
CXXFLAGS += -DTHREAD_SLEEP=$(THREAD_SLEEP)
CXXFLAGS += -DTOTAL_IT=$(TOTAL_IT)

.PHONY: all obj clean run run_tsan

all: $(TARGET)

$(TARGET): $(OBJECT)
	mkdir -p exe
	$(CPP) $(OBJECT) $(LDFLAGS) -o $(TARGET)
	@echo "Linked target $(TARGET) with LDFLAGS=$(LDFLAGS)"
	@echo "----------------------------------------"

obj/%.o: %.cpp FORCE
	mkdir -p obj
	$(CPP) $(CXXFLAGS) -c $< -o $@
	@echo "Output object file $@"
	@echo "----------------------------------------"

FORCE:

clean:
	rm -f obj/*.o
	rm -f $(TARGET)

run_tsan: $(TARGET)
	@echo "Running with TSAN options"
	@echo "----------------------------------------"
	TSAN_OPTIONS="verbosity=2" ./$(TARGET)

run: $(TARGET)
	@echo "Running with default options"
	@echo "----------------------------------------"
	./$(TARGET)