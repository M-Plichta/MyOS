# cd /home/lancs/task2

SRC=src
OBJ=obj
ISO=isofiles
CPATH=/usr/local/i586-elf-gcc/bin

if [ -e bootable.iso ]; then
   /bin/rm bootable.iso
fi
if [ -e kernel.bin ]; then
   /bin/rm kernel.bin
fi
if [ -d ${ISO} ]; then
   /bin/rm -r ${ISO}/
fi
if [ -d ${OBJ} ]; then
   /bin/rm -r ${OBJ}/
fi
/bin/mkdir -p ${OBJ}
/bin/mkdir -p ${ISO}/boot/grub

AS=${CPATH}/i586-elf-as
CC=${CPATH}/i586-elf-gcc
LD=${CPATH}/i586-elf-ld
CFLAGS="-Wall -Wextra -fno-builtin"
LDFLAGS="-nostdlib"

${AS} -o ${OBJ}/boot.o   ${SRC}/boot.s
${AS} -o ${OBJ}/loader.o ${SRC}/loader.s
nasm ${SRC}/GDTLoader.asm -f elf32 -o  ${OBJ}/GDTLoader.o
nasm ${SRC}/Interrupts.asm -f elf32 -o ${OBJ}/Interrupts.o
#${AS} -o ${OBJ}/GDTLoader.o ${SRC}/GDTLoader.s

${CC} ${CFLAGS} -o ${OBJ}/io.o        -c ${SRC}/io.c
${CC} ${CFLAGS} -o ${OBJ}/kernel.o    -c ${SRC}/kernel.c
${CC} ${CFLAGS} -o ${OBJ}/kprintf.o   -c ${SRC}/kprintf.c
${CC} ${CFLAGS} -o ${OBJ}/multiboot.o -c ${SRC}/multiboot.c
${CC} ${CFLAGS} -o ${OBJ}/vga.o       -c ${SRC}/vga.c
${CC} ${CFLAGS} -o ${OBJ}/globals.o   -c ${SRC}/globals.c
${CC} ${CFLAGS} -o ${OBJ}/isr.o       -c ${SRC}/isr.c
${CC} ${CFLAGS} -o ${OBJ}/descriptor_tables.o       -c ${SRC}/descriptor_tables.c

#temp
${CC} ${CFLAGS} -o ${OBJ}/heap.o     -c ${SRC}/heap.c
${CC} ${CFLAGS} -o ${OBJ}/keyboard.o     -c ${SRC}/keyboard.c

${LD} ${LDFLAGS} -T ${SRC}/linker.ld -o kernel.bin \
	${OBJ}/boot.o \
	${OBJ}/loader.o \
	${OBJ}/kernel.o \
	${OBJ}/io.o \
	${OBJ}/kprintf.o \
	${OBJ}/multiboot.o \
	${OBJ}/vga.o \
	${OBJ}/descriptor_tables.o\
	${OBJ}/GDTLoader.o\
	${OBJ}/globals.o\
	${OBJ}/Interrupts.o\
	${OBJ}/isr.o\
	${OBJ}/keyboard.o\
	${OBJ}/heap.o

/bin/cp grub/stage2_eltorito ${ISO}/boot/grub
/bin/cat > ${ISO}/boot/grub/menu.lst << __ENDMENU__
default 0
timeout 1
 
color white/green yellow/blue

title SCC211 Kernel
kernel /boot/kernel.bin cmdline=any kernel parameters
__ENDMENU__
/bin/cp kernel.bin ${ISO}/boot
/usr/bin/genisoimage -A "MyOS" -input-charset utf-8\
	-R -b boot/grub/stage2_eltorito -no-emul-boot -boot-load-size 4 \
	-boot-info-table -o bootable.iso ${ISO}

/usr/bin/qemu-system-i386 -boot order=d -cdrom bootable.iso
