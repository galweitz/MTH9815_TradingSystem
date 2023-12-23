cmake_minimum_required(VERSION 3.5)
project(TradingSystem)

# Specify the C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Set the location of the Boost installation.
set(BOOST_ROOT "C:/local/boost")
find_package(Boost REQUIRED COMPONENTS date_time) # specify needed Boost components here

# If you want to include Boost header files
include_directories(${Boost_INCLUDE_DIRS})

# Add all the source files needed to build the executable
add_executable(
    TradingSystem main.cpp
)

file(COPY prices.txt marketdata.txt trades.txt inquiries.txt
     DESTINATION ${CMAKE_CURRENT_BINARY_DIR})

# Link Boost libraries to your project
target_link_libraries(TradingSystem ${Boost_LIBRARIES})

