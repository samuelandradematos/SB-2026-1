#Compilador, flags e linker
NASM = nasm
NASM_FLAGS = -f elf32
NASM_FLAGS_DEBUG = -f elf32 -g
LD = ld
LD_FLAGS = -m elf_i386

#Targets
MAIN = CALCULADORA
MODULES = AUXILIARES SOMA SUBTRACAO MULTIPLICACAO DIVISAO EXPONENCIACAO MOD
TARGET = calculadora

#OBJs
OBJS = $(MAIN).o $(MODULES:=.o)

#Regra padrão
all: $(TARGET)

#Regra de linking
$(TARGET): $(OBJS)
	$(LD) $(LD_FLAGS) $(OBJS) -o $(TARGET)

#Regra genérica de compilação
%.o : %.asm
	$(NASM) $(NASM_FLAGS) $< -o $@

#Regra de debug
debug: clean
	$(MAKE) OBJS="$(MAIN).o $(MODULES:=.o)" NASM_FLAGS="$(NASM_FLAGS_DEBUG)" $(TARGET)
	gdb ./$(TARGET)
#Regra de limpeza
clean:
	rm -f *.o $(TARGET)

#Regra para executar
run: $(TARGET)
	./$(TARGET)