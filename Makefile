#
# Les variables d'environnement libG3X, incG3X
# sont definies dans le fichier ~/.bashrc par le script ../install.sh
#
#compilateur
CC = gcc

CCEXT=
ifeq ($(CC),g++) #en mode debug
  CCEXT=++
endif

#compil en mode 'debug' ou optmisée (-O2)
DBG = no

ifeq ($(DBG),yes) #en mode debug
  CFLAGS = -g -Wpointer-arith -Wall -ansi
else               #en mode normal
  CFLAGS = -O2 -ansi
endif

# assemblage des infos de lib. et inc.
lib =   $(libG3X)$(CCEXT)
# fichiers *.c locaux
src = src/
# fichiers *.h locaux et lib.
inc = -I./include $(incG3X) 

exec = g3x_example demo_scene3D

all : $(exec) clean

# règle générique de création de xxx.o à partir de src/xxx.c
%.o : $(src)%.c
	@echo "module $@"
	@$(CC) $(CFLAGS) $(inc) -c $< -o $@
	@echo "------------------------"

# règle générique de création de l'executable xxx à partir de src/xxx.c (1 seul module)
% : %.o
	@echo "assemblage [$^]->$@"
	@$(CC) $^ $(lib) -o $@
	@echo "------------------------"
	
.PHONY : clean cleanall ?

# informations sur les paramètres de compilation       
? :
	@echo "---------compilation informations----------"
	@echo "  processor      : $(PROCBIT)"
	@echo "  compiler       : $(CC)"
	@echo "  options        : $(CFLAGS)"
	@echo "  lib g3x/OpenGl : $(lib)"
	@echo "  headers        : $(incG3X)"
clean : 
	@rm -f *.o
cleanall :
	@rm -f *.o $(exec) *~ */*~

lray:
	@$(CC) $(CFLAGS) $(inc) -c $(src)level1.c -o level1.o
	@$(CC) $(CFLAGS) $(inc) -c $(src)level2.c -o level2.o
	@$(CC) $(CFLAGS) $(inc) -c $(src)level3.c -o level3.o
	@$(CC) $(CFLAGS) $(inc) -c $(src)lray.c -o lray.o
	@$(CC) lray.o level1.o level2.o level3.o $(lib) -o lray
	@rm -f *.o

fractal_generator:
	@$(CC) $(CFLAGS) $(inc) -c $(src)level1.c -o level1.o
	@$(CC) $(CFLAGS) $(inc) -c $(src)fractal_generator.c -o fractal_generator.o
	@$(CC) fractal_generator.o level1.o $(lib) -o fractal_generator
	@rm -f *.o


