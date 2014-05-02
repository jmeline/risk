## Simple makefile

CC=mpicxx
CFLAGS=-g -Wall
C11=-std=c++11
VERBOSE=-DVERBOSE_STRATEGIES

INCLUDE_PATH=src/
LIBS=src/game src/gamemap src/locations src/strategies
BUILD_DIR=build
TARGET=risk_exec

main: $(BUILD_DIR)
	$(CC) $(C11) src/GameManager.cpp src/GameSlave.cpp src/TrueMain.cpp -I$(INCLUDE_PATH) $(BUILD_DIR)/*.o -o $(TARGET)

test: $(BUILD_DIR)
	$(CC) $(C11) tests/MainTest.cpp -I$(INCLUDE_PATH) $(BUILD_DIR)/*.o -o $(TARGET)

simple: $(BUILD_DIR)
	$(CC) $(C11) tests/newsimpletest.cpp -I$(INCLUDE_PATH) $(BUILD_DIR)/*.o -o $(TARGET)

$(BUILD_DIR):
	@echo "Building Risk for CS5500..."
	mkdir $(BUILD_DIR)/
	for dir in $(LIBS); do \
		cd $$dir; \
		#$(CC) $(VERBOSE) $(C11) -c *.cpp -I../; \
		$(CC) $(C11) -c *.cpp -I../; \
		mv *.o ../../$(BUILD_DIR); \
		cd -; \
	done
	
clean:
	rm -rf $(BUILD_DIR)/ $(TARGET)
