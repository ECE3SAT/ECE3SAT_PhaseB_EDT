#####################################################################################################
#																				#
#	cible 	  : nom du fichier créé (par exemple, Programme).								#
# 	dependances : fichiers (ou règles) nécessaires à la construction de la cible.				#
#    commandes   : commandes à effectuer pour créer la cible.								#
#																				#
#	cible: dependances																#
#        	commandes																	#
#																				#
#   $@ : nom de la cible ;															#
#   $< : première dépendance de la liste des dépendances ;									#
#   $? : les dépendances plus récentes que la cible ;										#
#   $^ : toutes les dépendances ;														#
#   $* : correspond au ' * ' simple dans le shell, i.e. représente n'importe quel nom.			#
#####################################################################################################

CC = g++
CFLAGS = -W -Wall
LFLAGS = 

##############		Directories	####################

prefix   	  = $(PWD)#.
srcdir  	  = $(prefix)/src/
bindir  	  = $(prefix)/bin/
includedir  = -I$(prefix)/include/ \
		    #-I/usr/include/ \
		    #-I/usr/local/include/ \
		    #-I/usr/local/include/opencv2/
datadir 	  = $(prefix)/images/
libdir  	  = -L$(prefix)/lib/ \
		   # -L/usr/lib/x86_64-linux-gnu/ \
		   #-L/usr/local/lib/

##############		Files		####################

SRC_FILES = $(wildcard $(srcdir)*.cpp)
OBJ_FILES = $(SRC_FILES:.cpp=.o)	
LIB_FILES = -lopencv_core \
		  -lopencv_highgui \
		  -lopencv_imgproc \
		  -lopencv_imgcodecs \
		  #-lopencv_photo#$(foreach f, $(INC_DIR), -l$f)
		   
EXEC = $(prefix)/Detecting_Lines_Program

##############	 Règles de Compilation	####################

all : $(EXEC)

$(EXEC) : $(OBJ_FILES)
	$(CC) $^ -o $(EXEC) $(includedir) $(libdir) $(LIB_FILES)
	@echo "----------executable made----------"
	
$(OBJ_FILES) : $(SRC_FILES)
	$(CC) $(CFLAGS) -c $< -o $@ $(includedir)
	@echo "----------object files made----------"

#hough_transform : $(SRC_FILES)
#	$(CC) $(SRC_FILES) -o $(EXEC) `pkg-config --cflags --libs opencv`
#	@echo "----------executable made"----------"
	
clean:
	rm -f $(OBJ_FILES)
	rm -f $(EXEC)
	
echo:
	@echo "src directory : " $(srcdir)
	@echo "src files : " $(SRC_FILES)
	@echo "obj directory : " $(objdir)
	@echo "obj files : " $(OBJ_FILES)
	@echo "lib directory : " $(libdir)
	@echo "lib files : " $(LIB_FILES)


