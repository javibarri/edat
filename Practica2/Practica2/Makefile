CC = gcc
CFLAGS = -Wall -Wextra -pedantic -ansi
LDLIBS = -lodbc

EXE = dvdreq_customer dvdfilm_remove dvdrent_new dvdreq_film dvdreq_recommend dvdreq_rent dvdrent_remove

all : $(EXE)

clean :
	rm -f *.o core $(EXE)

$(EXE) : % : %.o odbc.o
