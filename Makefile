PROGNAME="parsehttp"
all: parsehttp.o $(PROGNAME)
$(PROGNAME): parsehttp.o
				$(CC) -o $@ $<
clean:
				rm *.o $(PROGNAME)
