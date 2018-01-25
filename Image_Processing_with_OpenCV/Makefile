#####################################################################################################
#                                                                                                   #
#    cible       : nom du fichier créé (par exemple, Programme).                                    #
#     dependances : fichiers (ou règles) nécessaires à la construction de la cible.                 #
#    commandes   : commandes à effectuer pour créer la cible.                                       #
#                                                                                                   #
#    cible: dependances                                                                             #
#            commandes                                                                              #
#                                                                                                   #
#   $@ : nom de la cible ;                                                                          #
#   $< : première dépendance de la liste des dépendances ;                                          #
#   $? : les dépendances plus récentes que la cible ;                                               #
#   $^ : toutes les dépendances ;                                                                   #
#   $* : correspond au ' * ' simple dans le shell, i.e. représente n'importe quel nom.              #
#####################################################################################################

CC = g++
CFLAGS = -W -Wall
LFLAGS = 

##############        Directories    ####################

srcdir        = $(PWD)/src/
bindir        = $(PWD)/bin/
includedir    = -I$(PWD)/include/ \
              #-I/usr/include/ \
              #-I/usr/local/include/ \
              #-I/usr/local/include/opencv2/
datadir       = $(PWD)/images/
libdir        = -L$(PWD)/lib/ \
              # -L/usr/lib/x86_64-linux-gnu/ \
              #-L/usr/local/lib/

##############        Files        ####################

SRC_FILES = $(wildcard $(srcdir)*.cpp)
OBJ_FILES = $(SRC_FILES:.cpp=.o)    
LIB_FILES = -lopencv_core \
            -lopencv_highgui \
            -lopencv_imgproc \
            -lopencv_imgcodecs \
           
EXEC = $(PWD)/Detecting_Lines_Program.exe
 

##############     Compilation Rules    ####################

all : $(EXEC)

$(EXEC) : $(OBJ_FILES)
	$(CC) $^ -o $(EXEC) $(includedir) $(libdir) $(LIB_FILES)
	@echo "                                               "
	@echo "###############################################"
	@echo "#                                             #"
	@echo "#              Executable Made                #"
	@echo "#                                             #"
	@echo "###############################################"
	@echo "                                               "
	@echo "                                               "
	@echo " - WHEN RUNNING THE EXECUTABLE, IF THIS ERROR APPEARS : 'error while loading shared libraries'  "
	@echo "                                                             "
	@echo " - TRY :  export LD_LIBRARY_PATH=$(LD_LIBRARY_PATH):$(PWD)/lib "
	@echo " - OR : sudo echo "$(PWD)/lib" >> /etc/ld.so.conf.d/opencv.conf "
	@echo " - OR BOTH OF THEM"
	@echo "                                               "
	@echo "                                               "
	
	
#export LD_LIBRARY_PATH=$(LD_LIBRARY_PATH):$(PWD)/lib
#echo "$(PWD)/lib" >> /etc/ld.so.conf.d/opencv.conf	

#echo "LD_LIBRARY_PATH=$(LD_LIBRARY_PATH):$(PWD)/lib" >> ~/.bashrc
    
$(OBJ_FILES) : $(SRC_FILES)
	$(CC) $(CFLAGS) -c $< -o $@ $(includedir)
	@echo "                                               "
	@echo "###############################################"
	@echo "#                                             #"
	@echo "#             Object Files Made               #"
	@echo "#                                             #"
	@echo "###############################################"
	@echo "                                               "

clean:
	rm -f $(OBJ_FILES)
	rm -f $(EXEC)
    
echo:
	@echo "Print Working Directory (PWD) : " $(PWD)
	@echo "src directory : " $(srcdir)
	@echo "src files : " $(SRC_FILES)
	@echo "include directory : " $(includedir)
	@echo "obj directory : " $(objdir)
	@echo "obj files : " $(OBJ_FILES)
	@echo "lib directory : " $(libdir)
	@echo "lib files : " $(LIB_FILES)
#@echo "export lib path : " $(LIB_PATH)


