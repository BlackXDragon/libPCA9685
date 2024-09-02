# PCA9685 C/C++ library
cmake_minimum_required(VERSION 3.5)

project(PCA9685)

set(CMAKE_C_STANDARD 99)
set(CMAKE_C_STANDARD_REQUIRED ON)

# Dependencies
find_library(I2C_LIB i2c)

# Library
add_library(PCA9685 SHARED src/PCA9685.c)
target_include_directories(PCA9685 PUBLIC
	$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
	$<INSTALL_INTERFACE:${CMAKE_INSTALL_PREFIX}/include>
)
target_link_libraries(PCA9685 
	${I2C_LIB}
)

# Install
install(
	TARGETS PCA9685
	EXPORT PCA9685Targets
	LIBRARY DESTINATION lib
	ARCHIVE DESTINATION lib
	RUNTIME DESTINATION bin
	INCLUDES DESTINATION include
)
install(
	DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/include/
	DESTINATION include
)
install(
	EXPORT PCA9685Targets
	DESTINATION share/PCA9685/cmake
	FILE PCA9685Targets.cmake
)

include(CMakePackageConfigHelpers)

configure_package_config_file(
	${CMAKE_CURRENT_SOURCE_DIR}/Config.cmake.in
	${CMAKE_CURRENT_BINARY_DIR}/PCA9685Config.cmake
	INSTALL_DESTINATION share/PCA9685/cmake
)

install(
	FILES
	${CMAKE_CURRENT_BINARY_DIR}/PCA9685Config.cmake
	DESTINATION share/PCA9685/cmake
)

# Test executable
add_executable(testPCA9685 test/testPCA9685.c)
target_link_libraries(testPCA9685 PCA9685)
