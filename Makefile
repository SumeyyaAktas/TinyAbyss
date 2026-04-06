CC := arm-none-eabi-gcc
LD := arm-none-eabi-ld
OBJCOPY := arm-none-eabi-objcopy

BUILD_DIR := build
TARGET := $(BUILD_DIR)/rtos.elf

C_SRC := $(shell find . -name "*.c")
C_OBJ := $(patsubst ./%.c,$(BUILD_DIR)/%.o,$(C_SRC))

ASM_SRC := $(shell find . -name "*.s")
ASM_OBJ := $(patsubst ./%.s,$(BUILD_DIR)/%.o,$(ASM_SRC))

OBJ := $(C_OBJ) $(ASM_OBJ)

CFLAGS := -mcpu=cortex-m3 \
          -mthumb \
          -nostdlib \
          -nostartfiles \
          -ffreestanding \
          -fno-builtin \
          -fno-stack-protector \
          -Wall \
          -Wextra \
          -O0 \
          -g \
		  -Iinclude

LDFLAGS := -T linker.ld \
           -nostdlib

all: $(TARGET)

$(BUILD_DIR):
	@mkdir -p $@

$(BUILD_DIR)/%.o: %.c | $(BUILD_DIR)
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -c $< -o $@

$(BUILD_DIR)/%.o: %.s | $(BUILD_DIR)
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -c $< -o $@

$(TARGET): $(OBJ) | $(BUILD_DIR)
	$(CC) $(CFLAGS) $(LDFLAGS) -o $@ $^

run: $(TARGET)
	qemu-system-arm -M lm3s6965evb \
		-kernel $(TARGET) \
		-nographic

debug: $(TARGET)
	qemu-system-arm -M lm3s6965evb \
		-kernel $(TARGET) \
		-nographic \
		-s -S

clean:
	@rm -rf $(BUILD_DIR)

.PHONY: all run debug clean