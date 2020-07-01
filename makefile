FLAGS   = -Wall -Wextra
CC      = gcc
PROG_1  = projeto
OBJS_1  = main.o functions.o
PROG_2  = readBin
OBJS_2  = readBin.o

all:	${PROG_1} ${PROG_2}

clean:
		rm ${OBJS_1} ${OBJS_2}

${PROG_1}:	${OBJS_1}
		 ${CC} ${FLAGS} ${OBJS_1} -o $@

${PROG_2}:	${OBJS_2}
		${CC} ${FLAGS} ${OBJS_2} -o $@

.c.o:
		${CC} ${FLAGS} $< -c

##########################

main.o:					main.c
functions.o:			functions.c functions.h

readBin.o:				readBin.c
