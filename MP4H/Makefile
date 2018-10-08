CC := gcc
CFLAGS :=-Wall -g
LDFLAGS := -lpng -g
HEADERS := mp4.h
EXE := mp4
SOURCES := mp4.o main.o

.ALL: ${EXE}

%.o: %.c ${HEADERS} Makefile
	${CC} -c ${CFLAGS} -o $@ $<

${EXE}: ${SOURCES}
	$(CC) ${SOURCES} -o ${EXE} ${LDFLAGS}

clean::
	rm -f ${SOURCES} mp4 image.png

