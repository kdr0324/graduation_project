#

CROSS_COMPILE 	= ae32000-elf-uclibc-

AS		= $(CROSS_COMPILE)as
LD		= $(CROSS_COMPILE)ld
CC		= $(CROSS_COMPILE)gcc
CPP		= $(CC) -E
AR		= $(CROSS_COMPILE)ar
NM		= $(CROSS_COMPILE)nm
STRIP	= $(CROSS_COMPILE)strip
OBJCOPY	= $(CROSS_COMPILE)objcopy
OBJDUMP	= $(CROSS_COMPILE)objdump
RANLIB	= $(CROSS_COMPILE)ranlib
ELF2FLT	= $(CROSS_COMPILE)elf2flt

CFLAGS	= -O2 -Wall -Wstrict-prototypes -Dlinux -D__linux__ -Dunix -D__uClinux__ -DEMBED

COMPILER_PREFIX=/usr/local/ae32000-elf-uclibc-tools
LDFLAGS = -r -Xlinker -T$(COMPILER_PREFIX)/lib/ae32000-elf2flt.ld
LDFLAGS_DBG = -Xlinker -T$(COMPILER_PREFIX)/lib/ae32000-elf2flt.ld

LDLIBS	= -lm -lc 


export AS LD CC CPP AR NM STRIP OBJCOPY OBJDUMP RANLIB CFLAGS

#PROJ = ../../../../../tools/SoC_Robotwar_Inyro
PROJ = SoC_Robotwar_Inyro

OBJS = main.o Amazon/graphic_api.o Amazon/uart_api.o Amazon/ImagePixel.o Amazon/ImageConvert.o Amazon/Draw.o Amazon/Line.o Amazon/Labeling.o Amazon/Number.o Amazon/Memory.o Common/ImageManagement.o Common/Bitmap.o Huro/Huro.o Huro/HuroComm.o Huro/AlignPosition.o Huro/Barricade.o Huro/Golf.o Huro/Hurdle.o Huro/Mine.o Huro/RedStair.o Huro/Stair.o Huro/YellowHole.o Taekwon/Taekwon.o Taekwon/TaekwonComm.o Amazon/LUT.o

all: $(PROJ)

$(PROJ): $(OBJS)
	$(CC) $(LDFLAGS) -o $@.elf $^ $(LDLIBS)
	$(CC) $(LDFLAGS_DBG) -o $@.dbg $^ $(LDLIBS)
	$(ELF2FLT) $@.elf
	mv $@.elf.bflt $@
	#${OBJDUMP} -Dtx $@.elf > $@.elf.dis
	#${OBJDUMP} -Dtx $@.dbg > $@.dbg.dis

.c.o:
	$(CC) $(CFLAGS) -c $< -o $@
	
.s.o:
	$(AS) $< -o $@
	
clean:
	rm -f *.o Amazon/*.o Common/*.o Huro/*.o Taekwon/*.o *.elf *.bflt *.dis $(TEST) *.dbg SoC_Robotwar_Inyro
