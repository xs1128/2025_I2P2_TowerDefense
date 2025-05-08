C := gcc-14
CXX := g++-14
SOURCE_DIR := $(CURDIR)
BUILD_DIR := $(CURDIR)/build
PROJECT_NAME := 2025_I2P2_TowerDefense

ifeq ($(OS),Windows_NT)
	GENERATOR := Unix Makefiles
	GENERATOR_PROGRAM := $(CURDIR)/ninja.exe
else
	GENERATOR := Ninja
	GENERATOR_PROGRAM := ninja
endif

.PHONY: load build run clean

all: load build run

# Load the project configuration
load:
	@echo "Loading project configuration..."
	@cmake -DCMAKE_BUILD_TYPE=Debug -DCMAKE_EXPORT_COMPILE_COMMANDS=TRUE -DCMAKE_C_COMPILER=${C} -DCMAKE_CXX_COMPILER=${CXX} -DCMAKE_MAKE_PROGRAM=${GENERATOR_PROGRAM} -G "${GENERATOR}" -S ${SOURCE_DIR} -B ${BUILD_DIR}
	@echo "Project loaded successfully."

# Build the project
build:
	@echo Using generator: ${GENERATOR}
	@echo "Building project..."
	@cmake --build ${BUILD_DIR} --target ${PROJECT_NAME} --config Debug -j 8
	@echo "Build completed successfully."

# Run the project
run:
	@echo "Running project..."
	@${BUILD_DIR}/${PROJECT_NAME}

# Check (for testing)
check:
	@echo "Running tests..."
		@cmake --build $(BUILD_DIR) --target test

# Distcheck (simulate release build + test)
distcheck:
	@echo "Running distribution check..."
		@cmake --build $(BUILD_DIR) --target package
			@echo "Distribution package created."

# cmake -DCMAKE_BUILD_TYPE=Debug -DCMAKE_EXPORT_COMPILE_COMMANDS=TRUE -DCMAKE_MAKE_PROGRAM="%cd%/ninja" -S "%cd%/" -B "%cd%/build" -G "Unix Makefiles" &&
# cmake --build "./build" --target all --config Debug -j 8 &&
# start "./build/2025_I2P2_TowerDefense_with_answer"
