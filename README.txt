MTH9815 Final Project: Bond Trading System for US Treasuries
Gal Weitz Fall 2023

### Compiling and Running the Program ###

Run the executable in a UNIX environment.
- At project directory: make build directory and change to it.
- Edit Boost path as detailed in CMakeLists.txt
- Run command: cmake ..
- Run command: make
- Run command: ./TradingSystem
Resulting .txt files will be found in the build directory.

### Overview ###

This document describes the bond trading system developed for US Treasuries, covering securities ranging from 2Y to 30Y.
While each service was supposed to run as its own separate program, I did not know how to parallelize it and decided to demonstrate their connectivity and functionality by running them all sequentially.

### System Components ###

The system comprises several interconnected services, each managing specific aspects of bond trading:

- Bond Pricing, Trading, and Execution Services: Handle pricing updates, trade bookings, and execution of trades.
- Bond Risk and Position Services: Manage risk calculations and position tracking.
- Bond Market and Streaming Services: Process market data and manage streaming of prices.
- Bond Inquiry and GUI Services: Handle customer inquiries and provide a some graphical user interface for real-time data.
- Bond Historical Data Services: Record historical data for various trading activities.

### Input Files: ###

prices.txt: Contains pricing data for the bonds.
trades.txt: Includes trade details for each security.
marketdata.txt: Holds market data updates.
inquiries.txt: Contains customer inquiries about bond trades.

### Output Files: ###

- allinquiries.txt: Stores processed inquiries.
- executions.txt: Records details of trade executions.
- gui.txt: Captures GUI updates for streaming prices.
- positions.txt: Logs position data for each trade.
- risk.txt: Contains risk assessments for each bond.
- streaming.txt: Records streaming data for bond prices.

### Usage ###

- Initialization: The program starts by initializing services, connectors, and listeners for handling different aspects of bond trading.
- Listener Registration: Services register their respective listeners to ensure synchronized data flow.
- Connector Subscriptions: Connectors subscribe to input .txt files for real-time data updates.
- Performance Tracking: The system outputs the mentioned .txt files and logs the time for each subscription process to monitor efficiency.
= Completion: The program outputs the time used for each process and waits for user input before exiting.

