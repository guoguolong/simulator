BASE_DIR=.
LIB_CSV_PATH=$(BASE_DIR)/lib/csv_parser
LIB_HASHMAP_PATH=$(BASE_DIR)/lib/c_hashmap
SRC_PATH=$(BASE_DIR)/src
BIN_PATH=$(BASE_DIR)/bin
EXE_NAME=vending

# all 或 main 是默认执行任务 
all: clean compile
compile: lib-build
	gcc $(SRC_PATH)/*.c -o $(BIN_PATH)/$(EXE_NAME) -I$(LIB_CSV_PATH) -I$(LIB_HASHMAP_PATH) -L$(BIN_PATH) -lcsv -lhashmap

lib-build: lib-compile
	ar cr $(BIN_PATH)/libcsv.a $(BIN_PATH)/csv.o $(BIN_PATH)/split.o $(BIN_PATH)/fread_csv_line.o
	ar cr $(BIN_PATH)/libhashmap.a $(BIN_PATH)/hashmap.o
lib-compile: init
	gcc -c $(LIB_CSV_PATH)/csv.c -o $(BIN_PATH)/csv.o
	gcc -c $(LIB_CSV_PATH)/split.c -o $(BIN_PATH)/split.o
	gcc -c $(LIB_CSV_PATH)/fread_csv_line.c -o $(BIN_PATH)/fread_csv_line.o

	gcc -c $(LIB_HASHMAP_PATH)/hashmap.c -o $(BIN_PATH)/hashmap.o
init: 
	mkdir -p $(BIN_PATH)
	cp -R $(BASE_DIR)/data $(BIN_PATH)
clean:
	rm -rf $(BIN_PATH)
run:
	$(BASE_DIR)/bin/$(EXE_NAME)