BASE_DIR=.
LIB_CSV_PATH=$(BASE_DIR)/lib/csv_parser
SRC_PATH=$(BASE_DIR)/src
BIN_PATH=$(BASE_DIR)/bin
EXE_NAME=vending

# all 或 main 是默认执行任务 
all: clean compile
compile: lib-build
	gcc $(SRC_PATH)/*.c -o $(BIN_PATH)/$(EXE_NAME) -I$(LIB_CSV_PATH) -L$(BIN_PATH) -lcsv

lib-build: lib-compile
	ar cr $(BIN_PATH)/libcsv.a $(BIN_PATH)/*.o
lib-compile: init
	gcc -c $(LIB_CSV_PATH)/csv.c -o $(BIN_PATH)/csv.o
	gcc -c $(LIB_CSV_PATH)/split.c -o $(BIN_PATH)/split.o
	gcc -c $(LIB_CSV_PATH)/fread_csv_line.c -o $(BIN_PATH)/fread_csv_line.o
init: 
	mkdir -p $(BIN_PATH)
	cp -R $(BASE_DIR)/data $(BIN_PATH)
clean:
	rm -rf $(BIN_PATH)
run:
	$(BASE_DIR)/bin/$(EXE_NAME)