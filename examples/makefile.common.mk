ifeq ($(OS),Windows_NT)
	EXE = .exe
	BUILD_TOOL = -G"MSYS Makefiles"
else
	BUILD_TOOL = -G"Unix Makefiles"
endif

BUILD_DIR = .build

TARGET= cimgui_glfw

CMAKE_CMD = cmake -B $(BUILD_DIR) $(BUILD_TOOL) -DSTATIC_BUILD=true -DCMAKE_BUILD_TYPE=Release

ifeq ($(wildcard $(BUILD_DIR)/),)
	EXEC_CMD = $(CMAKE_CMD); $(MAKE) -C $(BUILD_DIR)
else
	EXEC_CMD = $(MAKE) -C $(BUILD_DIR)
endif

all: build run

build:
	$(EXEC_CMD)
	@-strip $(BUILD_DIR)/$(TARGET)$(EXE)

.PHONY: clean run fmt build

clean:
	rm -fr $(BUILD_DIR)

run:
	@-cp ./imgui.ini $(BUILD_DIR)/
	@(cd $(BUILD_DIR);  ./$(TARGET)$(EXE))
	@-cp -f $(BUILD_DIR)/imgui.ini .

fmt:
	@clang-format --style=file:"../.clang-format" main.c > temp.c
	@rm main.c
	@mv temp.c main.c
