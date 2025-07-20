X86CodeFolder=
X86_64CodeFolder=.
X86SRC=$(foreach folder,$(X86CodeFolder),$(shell find ./$(folder) -type f -name "*.cpp") $(shell find ./$(folder) -type f -name "*.asm")) 
X86OBJ=$(patsubst %, %.x86.o, $(X86SRC))
X86_64SRC=$(foreach folder,$(X86_64CodeFolder),$(shell find ./$(folder) -type f -name "*.cpp") $(shell find ./$(folder) -type f -name "*.asm")) 
X86_64OBJ=$(patsubst %, %.x86_64.o, $(X86_64SRC))
CXX=g++
CXX_X86_FLAGS=-m32 -ffreestanding -O2 -Wall -Wextra -fno-exceptions -fno-rtti -I ./
CXX_X86_64_FLAGS=-I ./ -Wall -Wextra -ffreestanding -fno-exceptions -fno-stack-protector -fno-stack-check -fno-lto -fno-PIC -m64 -march=x86-64 -mno-80387 -mno-mmx -mno-sse -mno-sse2 -mno-red-zone -mcmodel=kernel -fno-rtti
ASM=nasm
ASM_X86_FLAGS=-f elf32
ASM_X86_64_FLAGS=-f elf64
LD=ld
LD_X86_FLAGS=-m elf_i386 -O2 -nostdlib -T linker.ld
LD_X86_64_FLAGS=-m elf_x86_64 -nostdlib -static -T linker.ld
OUTPUT=grandidierite
IMAGE=grandidierite.img

test: $(IMAGE)
	@qemu-system-x86_64 $< -m 1G -serial stdio

$(IMAGE): $(OUTPUT)
	@cp test.img $@
	@cp $< tmp/kernel
	@mcopy -i $@ tmp/kernel ::BOOT
	@rm -f tmp/kernel 

$(OUTPUT): $(X86_64OBJ)
	@printf "\e[0;34m[\e[0m...\e[0;34m] Linking \e[1;36m$@\e[0m"
	@$(LD) $(LD_X86_64_FLAGS) $(X86_64OBJ) -o $@
	@printf "\r\e[0;34m[DONE] Linking \e[1;36m$@\e[0m\n"

%.cpp.x86_64.o: %.cpp
	@printf "\e[0;34m[\e[0m...\e[0;34m] CXX \e[1;36m$<\e[0m"
	@$(CXX) $(CXX_X86_64_FLAGS) -c $< -o $@
	@printf "\r\e[0;34m[DONE] CXX \e[1;36m$<\e[0m\n"

%.asm.x86_64.o: %.asm
	@printf "\e[0;34m[\e[0m...\e[0;34m] ASM \e[1;36m$<\e[0m"
	@$(ASM) $(ASM_X86_64_FLAGS) $< -o $@
	@printf "\r\e[0;34m[DONE] ASM \e[1;36m$<\e[0m\n"

clean:
	rm $(shell find ./ -type f -name "*.o") $(IMAGE) $(OUTPUT)