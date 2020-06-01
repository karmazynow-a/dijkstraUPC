UPCC = upcc -gupc -network=udp -pthreads=4

ALG_NAME = DijkstraUPC

ALG_OBJ = \
	main.o \
	dijkstra.o \
	utility.o \
	columnsToProcess.o

%.o: %.c
	$(UPCC) -c -o $@ $< -I.

$(ALG_NAME): $(ALG_OBJ)
	$(UPCC) $(ALG_OBJ) -o $(ALG_NAME)

.PHONY: all clean

all: $(ALG_OBJ)

clean: 
	@rm -rf *.o $(ALG_NAME)
