CXX = arm-linux-gnueabihf-g++
CXXFLAGS = -std=c++11 -Wall -g

# path #
SRC_PATH = src
BUILD_PATH = build
INC_PATH = include
BIN_PATH = $(BUILD_PATH)/bin
SRC_EXT = cpp #extension

SOURCES := $(shell find $(SRC_PATH) -type f -name *.$(SRC_EXT))
# SOURCES := $(filter-out $(SRC_PATH)/mpu9250.cpp, $(SOURCES))
OBJECTS = $(SOURCES:$(SRC_PATH)/%.$(SRC_EXT)=$(BUILD_PATH)/%.o)
DEPS = $(OBJECTS:.o=.d)

INCLUDE = -I include

print:
	@echo "$(CXX)"
	@echo "$(SOURCES)"
	@echo "$(OBJECTS)"
	@echo "$(INCLUDE)"

test:
	@echo "Build: $@"
	$(CXX) $(CXXFLAGS) $(INCLUDE) $(SOURCES) -o $(BUILD_PATH)/$@


.PHONY: clean
clean:
	$(RM) -f *.o
	$(RM) build/*