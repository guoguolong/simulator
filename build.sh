BASE_DIR=.
SRC_PATH=${BASE_DIR}/src
BIN_PATH=${BASE_DIR}/bin
LIB_CSV_PATH=${BASE_DIR}/lib/csv_parser
LIB_HASHMAP_PATH=${BASE_DIR}/lib/c_hashmap

rm -rf $BIN_PATH && mkdir -p $BIN_PATH/data
cp -R ${BASE_DIR}/data/* $BIN_PATH/data

gcc $SRC_PATH/*.c ${LIB_CSV_PATH}/*.c ${LIB_HASHMAP_PATH}/hashmap.c -o $BIN_PATH/vending -I$LIB_CSV_PATH -I$LIB_HASHMAP_PATH

cd $BIN_PATH 
./vending
