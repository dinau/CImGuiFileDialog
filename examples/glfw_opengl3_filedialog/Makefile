ifeq ($(OS),Windows_NT)
	EXE = .exe
endif

BUILD_DIR = .build

TARGET= $(BUILD_DIR)/cimgui_glfw

ifeq ($(wildcard $(BUILD_DIR)/),)
# for GCC
EXEC_CMD = cmake -B $(BUILD_DIR) -DSTATIC_BUILD=true -G"MSYS Makefiles" -DCMAKE_BUILD_TYPE=Release ; cd $(BUILD_DIR) ; make
else
EXEC_CMD = (cd $(BUILD_DIR); make)
endif

all:
	$(EXEC_CMD)
	@-strip $(TARGET)$(EXE)
	./$(TARGET)$(EXE)

clean:
	rm -fr $(BUILD_DIR)

run: all
	./$(TARGET)$(EXE)
