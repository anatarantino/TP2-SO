include ../Makefile.inc

MODULE=0000-sampleCodeModule.bin
SOURCES=$(wildcard [^_]*.c)
SOURCES_ASM=$(wildcard asm/*.asm)
SOURCES_LIBRARIES=$(wildcard libraries/*.c)
SOURCES_SHELL=$(wildcard shell/*.c)
SOURCES_CHESS=$(wildcard chess/*.c)

OBJECTS=$(SOURCES:.c=.o)
OBJECTS_ASM=$(SOURCES_ASM:.asm=.o)
OBJECTS_LIBRARIES=$(SOURCES_LIBRARIES:.c=.o)
OBJECTS_SHELL=$(SOURCES_SHELL:.c=.o)
OBJECTS_CHESS=$(SOURCES_CHESS:.c=.o)

all: $(MODULE)

$(MODULE): $(OBJECTS) $(OBJECTS_ASM) $(OBJECTS_LIBRARIES) $(OBJECTS_SHELL) $(OBJECTS_CHESS)
	$(LD) $(LDFLAGS) -T sampleCodeModule.ld -o ../$(MODULE) $^
%.o: %.c
	$(GCC) $(GCCFLAGS) -I./include -c $< -o $@
%.o: %.asm
	$(ASM) $(ASMFLAGS) $< -o $@

clean:
	rm -rf *.o asm/*.o libraries/*.o *.bin shell/*.o chess/*.o

.PHONY: all clean print
