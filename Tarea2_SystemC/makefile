##******************************************************************************
#                           Universidad de Costa Rica
#                 		     IE-1119 
# 				Verificación Funcional
#                                  I Ciclo 2018
#
# Autor: Pablo Avila B30724
#
# Descripcion:
# Este makefile compila el detector: PabloAvila_B30724_detector2.v
# creando un binario B30724_detector1 y un archivo con la simulacion
# prueba.vcd
#
#  Para compilar y visualizar:  
#    make 
#
#  Para limpiar la carpeta:
#    make clean
#*******************************************************************************


OBJS = PabloAvila_B30724_detector2.v

CC = iverilog
CC1 = vvp
VISOR= gtkwave
DEBUG = -o
CFLAGS = -gspecify -Ttyp

TARGET = B30724_detector2
VCD_FILE = prueba.vcd

all: $(TARGET)

$(TARGET): $(OBJS)	
	$(CC) $(OBJS) $(CFLAGS) $(DEBUG) $(TARGET) 
	$(CC1) $(TARGET)
	$(VISOR) $(VCD_FILE)

clean:
	rm -f -R *.o *.out *~ *.db *.aux *.log $(TARGET) $(VCD_FILE)
