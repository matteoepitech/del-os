##
## DELOS PROJECT, 2025
## Makefile
## File description:
## MAKEFILE DELOS
##

ARCH		:=	i386

BOOT_DIR	:=	boot
SRC_DIR		:=	src
BUILD_DIR	:=	build
INCLUDE_DIR	:=	include
LINKER_DIR	:=	linker

BOOT_FILE	:=	$(BOOT_DIR)/bootsector.s
ENTRY_FILE	:=	$(BOOT_DIR)/kernel_entry.s
ZERO_FILE 	:=	$(BOOT_DIR)/padding_zeroes.s

# Recursive function to find files
rwildcard	=	$(wildcard $1$2) $(foreach d,$(wildcard $1*),$(call rwildcard,$d/,$2))
SRC_C		:=	$(call rwildcard,$(SRC_DIR)/,*.c)
SRC_S		:=	$(call rwildcard,$(SRC_DIR)/,*.s)
OBJ_C		:=	$(patsubst $(SRC_DIR)/%.c,$(BUILD_DIR)/$(SRC_DIR)/%.o,$(SRC_C))
OBJ_S		:=	$(patsubst $(SRC_DIR)/%.s,$(BUILD_DIR)/$(SRC_DIR)/%.o,$(SRC_S))

OS_BIN		:=	$(BUILD_DIR)/delos.bin

NASM		:=	nasm
CC		:=	i386-elf-gcc
QEMU		:=	qemu-system-i386

CFLAGS		:=	-ffreestanding -m32 -g -c -Wall -Wextra \
			-fno-pie -fno-pic \
			-fno-stack-protector \
			-nostdlib -nostdinc \
			-nostartfiles \
			-flto \
			-O0 \
			-I$(INCLUDE_DIR)

LDFLAGS		:=	-m32 -T $(LINKER_DIR)/kernel_$(ARCH).ld \
			-nostdlib \
			-static \
			-flto \
			-ffreestanding

# all RULE : do the whole process
all: prepare $(OS_BIN)

# prepare RULE : prepare the build directory
prepare:
	@mkdir -p $(BUILD_DIR)

# $(BUILD_DIR)/boot_sector.bin RULE : create the boot sector for the bin
$(BUILD_DIR)/boot_sector.bin: $(BOOT_FILE) $(BUILD_DIR)/full_kernel.bin
	@kernel_size_bytes=$$(wc -c < $(BUILD_DIR)/full_kernel.bin); \
	kernel_sectors=$$((($$kernel_size_bytes + 511) / 512)); \
	echo "Assembling bootloader (kernel sectors: $$kernel_sectors)..."; \
	$(NASM) -I $(BOOT_DIR) -f bin $< -o $@ -DKERNEL_SECTORS=$$kernel_sectors

# $(BUILD_DIR)/kernel_entry.o RULE : create the kernel entry
$(BUILD_DIR)/kernel_entry.o: $(ENTRY_FILE)
	@echo "Assembling kernel entry..."
	@$(NASM) -f elf32 $< -o $@

# $(BUILD_DIR)/padding_zeroes.bin RULE : create the padding for the bin
$(BUILD_DIR)/padding_zeroes.o: $(ZERO_FILE)
	@echo "Assembling padding..."
	@$(NASM) -f elf32 $< -o $@

# Compilation des fichiers C
$(BUILD_DIR)/$(SRC_DIR)/%.o: $(SRC_DIR)/%.c
	@echo "Compiling $< ..."
	@mkdir -p $(dir $@)
	@$(CC) $(CFLAGS) $< -o $@

# Assemblage des fichiers ASM
$(BUILD_DIR)/$(SRC_DIR)/%.o: $(SRC_DIR)/%.s
	@echo "Assembling $< ..."
	@mkdir -p $(dir $@)
	@$(NASM) -f elf32 $< -o $@

# Link du kernel
$(BUILD_DIR)/full_kernel.bin: $(BUILD_DIR)/kernel_entry.o $(BUILD_DIR)/padding_zeroes.o $(OBJ_C) $(OBJ_S)
	@echo "Linking kernel... (may take time due to LTO)"
	@$(CC) $(LDFLAGS) -o $@ $^

# $(OS_BIN) RULE : create the final $(OS_BIN) file
$(OS_BIN): $(BUILD_DIR)/boot_sector.bin $(BUILD_DIR)/full_kernel.bin
	@echo "Creating OS image..."
	@cat $^ > $@
	@echo "Build complete: $@"

# run RULE : run the OS
run:
	@echo "Starting $(OS_BIN)..."
	@$(QEMU) -drive format=raw,file=$(OS_BIN),index=0,media=disk -m 128M

# clean RULE : clean everything in $(BUILD_DIR) directory
clean:
	@echo "Cleaning build directory..."
	@rm -rf $(BUILD_DIR)

# re RULE : redo the whole build + clean
re: clean all

# compile_commands RULE : create a compile_commands.json file for LSP
compile_commands:
	@echo "[" > compile_commands.json
	@first=1; \
	for f in $(SRC_C); do \
		[ $$first -eq 1 ] || echo "," >> compile_commands.json; \
		first=0; \
		echo "{" >> compile_commands.json; \
		echo "  \"directory\": \"$$PWD\"," >> compile_commands.json; \
		echo "  \"command\": \"$(CC) $(CFLAGS) -c $$f -o build/$$f.o\"," >> compile_commands.json; \
		echo "  \"file\": \"$$f\"" >> compile_commands.json; \
		echo "}" >> compile_commands.json; \
	done; \
	echo "]" >> compile_commands.json

# info RULE : show build information
info:
	@echo "=== DELOS BUILD INFO ==="
	@echo "Bootloader: $(BOOT_FILE)"
	@echo "Kernel Entry: $(ENTRY_FILE)"
	@echo "Sources C: $(SRC_C)"
	@echo "Sources ASM: $(SRC_S)"
	@echo "Objects C: $(OBJ_C)"
	@echo "Objects ASM: $(OBJ_S)"
	@echo "Include dir: $(INCLUDE_DIR)"
	@echo "Output: $(OS_BIN)"
	@echo "CC: $(CC)"
	@echo "LD: $(LD)"
	@echo "QEMU: $(QEMU)"

.PHONY: all clean run re prepare info compile_commands
