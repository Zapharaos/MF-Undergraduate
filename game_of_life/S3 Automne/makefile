CC = gcc

IDIR = include
ODIR = obj
BDIR = bin
SDIR = src
LDIR = lib

MODE=CAIRO

ifeq ($(TYPE), MAC)
	CPPFLAGS = -L /usr/X11/lib -I/usr/X11/include -I/usr/X11/include/cairo -Iinclude
else
	CPPFLAGS = -Iinclude -I/usr/include/cairo
endif

LDFLAGS = -lcairo -lm -lX11
CFLAGS = $(CPPFLAGS) -Wall -g

vpath %.h $(IDIR)
vpath %.c $(SDIR)
vpath %.o $(ODIR)
	
main: main.o jeu.o io.o grille.o
	@mkdir -p $(BDIR)

	@mkdir -p $(LDIR)
	ar -crv $(LDIR)/libjeu.a $(ODIR)/jeu.o $(ODIR)/grille.o

	$(CC) -D $(MODE) $(CFLAGS) -o $(BDIR)/$@ $(ODIR)/main.o $(ODIR)/jeu.o $(ODIR)/io.o $(ODIR)/grille.o $(LDFLAGS)
	@echo "\n----> Compilation effectuée\n"

	rm -f $(ODIR)/*.o
	@echo "\n----> Nettoyage (*.o) effectué\n"

%.o: %.c
	@mkdir -p $(ODIR)
	$(CC) -D $(MODE) $(CFLAGS) -o $(ODIR)/$@ -c $<

dist:
	@mkdir -p dist
	tar -J -cvf dist/MatthieuFreitag-GoL-v5.0.tar.xz grilles include src makefile README.md Doxyfile
	@echo "\n----> Archivage effectué\n"
clean:
	rm -f $(ODIR)/*.o
	rm -f $(BDIR)/*
	rm -f $(LDIR)/*.a
	rm -rf dist/
	@echo "\n----> Nettoyage effectué\n"

