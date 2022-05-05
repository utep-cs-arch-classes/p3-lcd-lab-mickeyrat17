# makfile configuration
COMMON_OBJECTS		= draw_shapes.o wdtInterruptHandler.o switches.o p2_interrupt_handler.o
CPU             	= msp430g2553
CFLAGS          	= -mmcu=${CPU} -Os -I../h
LDFLAGS		= -L../lib -L/opt/ti/msp430_gcc/include/ 

#switch the compiler (for the internal make rules)
CC              = msp430-elf-gcc
AS              = msp430-elf-gcc -mmcu=${CPU} -c

all: moving_demo.elf moving_demo_s.elf

#additional rules for files
moving_demo.elf: ${COMMON_OBJECTS} change_color.o moving_demo.o switches.o
	${CC} ${CFLAGS} ${LDFLAGS} -o $@ $^ -lTimer -lLcd

moving_demo_s.elf: ${COMMON_OBJECTS} change_color_s.o moving_demo.o switches.o
	${CC} ${CFLAGS} ${LDFLAGS} -o $@ $^ -lTimer -lLcd

load_c: moving_demo.elf
	msp430loader.sh $^

load_s: moving_demo_s.elf
	msp430loader.sh $^

clean:
	rm -f *.o *.elf
