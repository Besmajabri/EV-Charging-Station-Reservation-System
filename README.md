# EV Charging Station Reservation System

This project is a C++ application that manages reservations for electric vehicle charging stations. The program uses AVL trees & Map trees to efficiently manage reservations for electric charging stations based on power, location, and time. It supports various commands, including creating charging stations, finding available stations based on location and power, and reserving stations for a given time period.

# Compilation

To compile the project, follow these steps:

1. Make sure you have g++ installed on your system. You can check the installation by running g++ --version in your terminal. The project has been tested with g++ version 9.

2. Navigate to the project directory using the terminal.

3. Run the following command to compile the project:

```
make tp4
```

4. The program can be launched with the following command:

```
./tp4 <input_file>
```

where <input_file> is an optional argument that specifies the input file for the program. If no file is specified, the program will read from standard input.

# Input and Output Format

The input consists of a series of commands. Each command starts with a keyword specifying its type. The output depends on the command executed.

# Commands

* `creer` - Adds a charging station and displays "OK".
* `trouver` - Returns charging stations within a given radius and with at least a specified minimum power.
* `reserverI` - Reserves the first available date starting from a specified date for a certain duration.
* `reserverC` - Reserves a charging station located within a given radius and having the specified minimum power, which is the first available starting from a specified date for a given duration.

# Dates Format

All dates in this project are specified in the JJJ_HH:mm:ss format. The provided Date class can be found in the `date.h` and `date.cpp` files.

# Sequential Processing

Commands are processed sequentially, and the output of a command is displayed before reading the next command.

# Important Note on Code Conventions

Please be aware that in the source code for this program, variable names, function names, and comments are primarily written in French. We understand that this might pose some challenges for users who are not familiar with the French language. However, We have ensured that the user documentation, such as this README, is written in English for broader accessibility.

# Authors

The project was developed by Othmane Azzouzi and Besma Jabri.
