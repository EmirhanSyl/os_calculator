# OS_calculator
 Calculator Project for OS course using C and linux pipes

 
## Overview
The **Distributed Calculator Project** is a system where mathematical operations (addition, subtraction, multiplication, division) are distributed across separate subprocesses. The main program communicates with these subprocesses using pipes, and a dedicated `saver` program records the results into a file.

## Features
- Distributed handling of mathematical operations.
- Menu-driven user interface for operation selection.
- Persistent result saving in `results.txt` in the format: `Result: <operand1> <operation> <operand2> = <result>`.
- Modular design for scalability and maintainability.

## How It Works
1. The **main program** (`calculator`) starts and creates four subprocesses for handling operations.
2. The user selects an operation and inputs two numbers.
3. The main program communicates the input to the appropriate subprocess via a dedicated pipe.
4. The subprocess performs the calculation and sends the result back to the main program.
5. The subprocess calls the `saver` program to save the result in `results.txt`.
6. The main program displays the result and continues until the user selects "exit."

## Subprocesses
- `addition`: Handles addition operations.
- `subtraction`: Handles subtraction operations.
- `multiplication`: Handles multiplication operations.
- `division`: Handles division operations (with division by zero handling).
- `saver`: Saves the results to `results.txt`.

## Menu Options
- **1- Addition**: Adds two numbers.
- **2- Subtraction**: Subtracts the second number from the first.
- **3- Division**: Divides the first number by the second (error message if dividing by zero).
- **4- Multiplication**: Multiplies two numbers.
- **5- Exit**: Terminates the program.

## Installation
1. Clone the repository:
   ```bash
   git clone <repository_url>
   cd distributed-calculator
   ```

2. Compile the project using the Makefile:
   ```bash
   make
   ```

3. Run the main program:
   ```bash
   ./calculator
   ```

## File Structure
- **`calculator.c`**: Main program.
- **`addition.c`**: Subprocess for addition.
- **`subtraction.c`**: Subprocess for subtraction.
- **`multiplication.c`**: Subprocess for multiplication.
- **`division.c`**: Subprocess for division.
- **`saver.c`**: Program to save results to `results.txt`.
- **`Makefile`**: Automates the compilation of all components.
- **`results.txt`**: Output file storing calculation results.

## Usage Example
```
Calculator
1- Addition
2- Subtraction
3- Division
4- Multiplication
5- Exit
Enter your choice: 1
Enter two integers: 12 8
Result: 20
```
**Saved Result in `results.txt`:**
```
Result: 12 + 8 = 20
```

## Error Handling
- Division by zero is not allowed; an appropriate error message is displayed.
- System calls (`pipe`, `fork`, `exec`) are monitored for failure, with descriptive error messages.

## Development Notes
- Each subprocess has dedicated pipes for communication with the main program.
- The `saver` program operates independently and accepts results as command-line arguments.
- Graceful termination ensures no resource leaks.

## References
- Operating Systems Course Material.
- [Advanced Programming in the UNIX Environment](https://www.apuebook.com/) by W. Richard Stevens.
- GNU Make documentation for `Makefile` creation.
- Linux Programmer’s Manual (man pages for `fork`, `pipe`, `exec`, `wait`).

## Screenshots
![Ekran görüntüsü 2024-12-20 144911](https://github.com/user-attachments/assets/68671d93-2413-4013-bae1-f5fbf724a7a2)
![Ekran görüntüsü 2024-12-20 144841](https://github.com/user-attachments/assets/bd44a462-7149-4735-a9ae-efaf2f83c8d0)


---
