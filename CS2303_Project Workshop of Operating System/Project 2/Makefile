Objs = faneuil.o LarryCurlyMoe.o

.PHONY: all
all: faneuil LCM $(Objs)

faneuil : faneuil.o
	cc -o faneuil faneuil.c -lpthread

faneuil.o : faneuil.c
	cc -c faneuil.c

LCM : LarryCurlyMoe.o
	cc -o LCM LarryCurlyMoe.o -lpthread

LarryCurlyMoe.o : LarryCurlyMoe.c
	cc -c LarryCurlyMoe.c

.PHONY : clean
clean:
	rm faneuil LCM $(Objs)

