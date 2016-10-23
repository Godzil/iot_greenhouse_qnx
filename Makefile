CC = gcc
CFLAGS = -Wall -O2
#CFLAGS = -Wall -g
LIBS = -L/lib -L/usr/lib -lsocket
DESTDIR = /home/interface
#/usr/lib/libds.a /lib/ibsocket.a
OBJS = chtbl.o hashpjw.o list.o cgi_func.o cgi_theme_gest.o test_serre.o  /usr/lib/libds.a
OBJS1 = $(OBJS) modprofil.o
OBJS2 = $(OBJS) about.o
OBJS3 = $(OBJS) cgibase.o
OBJS4 = $(OBJS) commander.o

EXENAME = cgibase.cgi modprofil.cgi about.cgi commander.cgi
	
all: dep $(EXENAME)

help:
	@echo "Aide sur se Makefile"
	@echo "target reconnus :"
	@echo "  help : Ce message d'aide"
	@echo "  all : Genere tout les executables"
	@echo "  debug : Comme all mais avec le debug actif"
	@echo "  clean : Efface les fichiers objets et fichier temporaires"
	@echo "  cleandist : Comme clean mais efface en plus les executables généré"
	@echo "  install : installe tout les fichiers la ou il faut"
	@echo "  refreshweb : installe les fichier utilisé par le cgi la ou il faut"
	@echo "  uninstall : désinstalle le cgi \"proprement\""
	@echo "  uninstallmore : Comme uninstall mais un peu plus \"aggressif\""

commander.cgi: $(OBJS4)
	@echo --====:::::: Compiling $(@)... ::::::====--
	$(CC) $(CFLAGS) $(LIBS) -o $(@) $(OBJS4)

cgibase.cgi: $(OBJS3)
	@echo --====:::::: Compiling $(@)... ::::::====--
	$(CC) $(CFLAGS) $(LIBS) -o $(@) $(OBJS3)

modprofil.cgi: $(OBJS1)
	@echo --====:::::: Compiling $(@)... ::::::====--
	$(CC) $(CFLAGS) $(LIBS) -o $(@) $(OBJS1)

about.cgi: $(OBJS2)
	@echo --====:::::: Compiling $(@)... ::::::====--
	$(CC) $(CFLAGS) $(LIBS) -o $(@) $(OBJS2)

clean:
	@echo	--====:::::: Cleaning... ::::::====--
	rm -f *.o *~

cleandist: clean
	@echo --====:::::: Clean more... ::::::====--
	rm -f $(EXENAME) depend

mkdestdir:
	@echo --====:::::: Make directory... ::::::====--
	mkdir $(DESTDIR)	
	
install: all files.tgz mkdestdir refreshweb
	@echo --====:::::: Installing ::::::====--
	@echo --====:::::: Creating startup.sh ::::::====--
	echo \#!/bin/sh $(DESTDIR)/startup.sh > $(DESTDIR)/startup.sh
	echo echo Exporting system vars... >> $(DESTDIR)/startup.sh
	echo export HTTPD_ROOT_DIR=$(DESTDIR)/www >> $(DESTDIR)/startup.sh
	echo export HTTPD_ROOT_DOC=index.html >> $(DESTDIR)/startup.sh
	echo export HTTPD_SCRIPTALIAS=$(DESTDIR)/cgi-bin >> $(DESTDIR)/startup.sh
	echo echo Starting ds... >> $(DESTDIR)/startup.sh
	echo ds \& >> $(DESTDIR)/startup.sh
	echo echo Starting slinger... >> $(DESTDIR)/startup.sh
	echo slinger -e -c -s \& >> $(DESTDIR)/startup.sh
	echo echo Finish >> $(DESTDIR)/startup.sh
	@echo --====:::::: Installing CGI... ::::::====--
	cp $(EXENAME) $(DESTDIR)/cgi-bin
	@echo "."
	@echo Launch startup.sh in $(DESTDIR) to launch the web interface
	
files.tgz:
	@echo --====:::::: Make files.tgz... ::::::====--
	tar -czf $(@) cgi-bin www
	
uninstallmore: uninstall clean	
	@echo --====:::::: Remove more... ::::::====--
	rm -f files.tgz

refreshcgi: $(EXENAME)
	@echo --====:::::: Refreshing cgi files... ::::::====--
	cp $(EXENAME) $(DESTDIR)/cgi-bin

refreshweb: files.tgz
	@echo --====:::::: Uncompressing files... ::::::====--
	tar -zxvf files.tgz -C $(DESTDIR)
	
uninstall: 
	@echo --====:::::: Uninstalling... ::::::====--
	rm -Rf $(DESTDIR)

dep: depend *.c

depend:
	@echo --====:::::: Make dependency... ::::::====--
	gcc -M *.c > depend

cafe:
	@echo Inserez la tasse dans le compartiment prevu...

compress: cleandist files.tgz
	tar -czf../interface.tgz *.c *.h Makefile *.sh cgi-bin www
	

include depend
