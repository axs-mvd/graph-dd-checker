CFLAGS = -O3 -pipe -W -Wall -pthread -pedantic -fPIC  #release
#CFLAGS = -g -pipe -W -Wall -pthread -pedantic -fPIC   #debug

################################################################################
## Objects
################################################################################

OBJECTS = graph.o queue.o visited.o 

BINARIES_DIR=../bin

################################################################################
## Executable objects
################################################################################

MAIN_OBJECT=main.o
TEST_VISITED_OBJECT=test_visited.o

################################################################################
## Executable binaries
################################################################################

MAIN_EXECUTABLE = graph-dd-checker
ALL_EXEC = $(MAIN_EXECUTABLE) 

################################################################################

all: $(ALL_EXEC) copy_executables

################################################################################
$(MAIN_EXECUTABLE): $(OBJECTS) $(MAIN_OBJECT)
	$(CC) $(LINK_FLAGS) $^ -o $@

################################################################################
clean: clear

copy_executables: $(ALL_EXEC)
	@echo "deploying binaries..."
	@mkdir -p $(BINARIES_DIR)
	@cp -f $(ALL_EXEC) $(BINARIES_DIR)

clear:
	@echo "cleaning..."
	@rm -f *.o $(ALL_EXEC) 
	@mkdir -p $(BINARIES_DIR)
	@cd $(BINARIES_DIR) && rm -f $(ALL_EXEC) 

rebuild: clear all
