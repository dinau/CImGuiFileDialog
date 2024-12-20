ifeq ($(OS),Windows_NT)
	EXE = .exe
	BUILD_TOOL = -G"MSYS Makefiles"
else
	BUILD_TOOL = -G"Unix Makefiles"
endif

BUILD_DIR = .build

TARGET= $(BUILD_DIR)/cimgui_glfw

FEATURE_OPT += "-DCIMGUI_USE_GLFW \
							 -DCIMGUI_USE_OPENGL3 \
							 "

CMAKE_C_OPT   += -DCMAKE_C_FLAGS_RELEASE=$(FEATURE_OPT)
CMAKE_CPP_OPT += -DCMAKE_CPP_FLAGS_RELEASE=$(FEATURE_OPT)

CMAKE_CMD = cmake -B $(BUILD_DIR) $(BUILD_TOOL) -DSTATIC_BUILD=true -DCMAKE_BUILD_TYPE=Release $(CMAKE_C_OPT) $(CMAKE_CPP_OPT)

ifeq ($(wildcard $(BUILD_DIR)/),)
	EXEC_CMD = $(CMAKE_CMD); $(MAKE) -C $(BUILD_DIR)
else
	EXEC_CMD = $(MAKE) -C $(BUILD_DIR)
endif

all:
	$(EXEC_CMD)
	@-strip $(TARGET)$(EXE)
	./$(TARGET)$(EXE)

.PHONY: clean run fmt

clean:
	rm -fr $(BUILD_DIR)

run: all
	./$(TARGET)$(EXE)

fmt:
	@clang-format --style=file:"../.clang-format" main.c > temp.c
	@rm main.c
	@mv temp.c main.c
