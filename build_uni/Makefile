.PHONY: all clean docs install

all: build

build:
	@echo "Building DijkstraUPC..."
	@$(MAKE) -C ../src/


install: build
	@echo "Installing DijkstraUPC..."
	@cp ../src/DijkstraUPC .


clean:
	@echo "Cleaning..."
	@cd ../src/ && $(MAKE) clean
	@rm -f DijkstraUPC
	@rm -f resultsUPC.txt
	@rm -rf docs/


docs:
	@echo "Generating docs..."
	@doxygen Doxyfile_uni.in


run: install
	@echo "Running DijkstraUPC..."
	@if [ -z $$VERTEX ] ;\
	then \
		echo "If you want to pass own vertex add argument to command: make run VERTEX=?" ;\
		echo "Running with default value: make run VERTEX=0" ;\
		echo "" ;\
		VERTEX=0 ;\
	fi ;\
	if [ -z $$FILE ] ;\
	then \
		echo "If you want to pass own file add argument to command: make run FILE=?" ;\
		echo "Running with default value: make run FILE=''../data/graph.dat''" ;\
		echo "" ;\
		FILE="../data/graph.dat" ;\
	fi ;\
	if [ -z $$PROC_COUNT ] ;\
	then \
		echo "If you want to specify number of processes add argument to command: make run PROC_COUNT=?" ;\
		echo "Running with default value: make run PROC_COUNT=1" ;\
		echo "" ;\
		PROC_COUNT=1 ;\
	fi ;\
	if [ -z $$HOSTS ] ;\
	then \
		echo "If you want to host file add argument to command: make run HOSTS=?" ;\
		echo "Running with default host file: make run HOSTS=''nodes''" ;\
		echo "" ;\
		HOSTS="nodes" ;\
	fi ;\
	UPC_NODEFILE=$$HOSTS upcrun -n $$PROC_COUNT -N $$(wc -l $$HOSTS | cut -f1 -d' ') ./DijkstraUPC $$VERTEX $$FILE
