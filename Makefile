TARGET    := test

SRC_DIR   := src
OBJ_DIR   := build
GCM_DIR   := gcm.cache

GE_MODULE_OBJ := $()

SRC_FILES := $(SRC_DIR)/main.cpp $(shell find $(SRC_DIR) ! -name "main.cpp" -type f -name "*.cpp")
OBJ_FILES := $(patsubst $(SRC_DIR)%.cpp,$(OBJ_DIR)%.o,$(SRC_FILES))
DEP_FILES := $(patsubst $(SRC_DIR)%.cpp,$(OBJ_DIR)%.d,$(SRC_FILES))

SRC_SUBDIRS := $(shell find $(SRC_DIR) -type d)
OBJ_SUBDIRS := $(patsubst $(SRC_DIR)%,$(OBJ_DIR)%,$(SRC_SUBDIRS))

CPPFLAGS += -Wall -Wextra -pedantic -std=c++20 -I $(SRC_DIR)/GE -Wno-unused -fno-exceptions -O2 
LDFLAGS += -lglfw -lGL -lGLEW

.PHONY: all clean segf

all: $(TARGET)
segf: CPPFLAGS += -Og -g -fsanitize=address
segf: LDFLAGS += -Og -g -fsanitize=address
segf: $(TARGET)

$(TARGET): $(SYSTEM_MODULES) $(OBJ_SUBDIRS) $(OBJ_FILES)
	clang++ -o $@ $(OBJ_FILES) $(LDFLAGS)

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp
	clang++ -MMD -MP $(CPPFLAGS) -c $< -o $@

$(OBJ_SUBDIRS):
	mkdir -p $@

clean:
	rm -rf $(TARGET) $(OBJ_SUBDIRS) $(SYSTEM_MODULES)

-include $(DEP_FILES)
