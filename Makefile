PROGNAME="parsehttp"
all: parsehttp.o http_read.o utils.o $(PROGNAME)
$(PROGNAME): parsehttp.o http_read.o utils.o
				$(CC) -o $@ $^
clean:
				rm *.o $(PROGNAME)
