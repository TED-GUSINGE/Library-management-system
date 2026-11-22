# Library Management System

A console-based Library Management System written in C++17. It is the Practical 12 mini project for applying object-oriented programming, STL containers, exception handling, file I/O and Git-based development.

## Problem and users

The application is intended for a small library operator who needs to maintain a simple catalogue, find books and record when books are issued or returned.

## Features

- Add a book with a unique ID, title and author.
- List all books and their availability.
- Search for a book by ID.
- Issue an available book and return an issued book.
- Save records to `books.csv` and reload them when the program starts.
- Validate menu input and report unknown IDs, duplicate IDs and invalid operations.
- Preserve commas and quotation marks in CSV text fields.

## Concepts used

- **Encapsulation:** book details are private and accessed through public member functions.
- **Inheritance:** `Book` derives from the abstract `LibraryItem` base class.
- **Runtime polymorphism:** the virtual `display()` function is called through a `LibraryItem` pointer.
- **STL:** `std::vector` stores the books.
- **Exception handling:** custom exceptions identify unknown and duplicate book IDs; standard exceptions report other invalid operations and file errors.
- **File I/O:** `std::ifstream` loads records and `std::ofstream` saves them in CSV format.

## Project files

- `main.cpp` — application source code.
- `books.csv` — sample data; the program updates this file when records change.
- `docs/class-diagram.mmd` — UML-style class diagram.
- `docs/sample-output.md` — illustrative console demonstration.

## Requirements

- A C++17 compiler such as GCC or Clang.
- macOS, Linux or Windows terminal.

## Compile and run

Open a terminal in the project folder and run:

```bash
g++ -std=c++17 -Wall -Wextra -pedantic main.cpp -o library
./library
```

On Windows with MinGW, run:

```bash
g++ -std=c++17 -Wall -Wextra -pedantic main.cpp -o library.exe
library.exe
```

The program looks for `books.csv` in its current working directory. Keep the CSV file in the same folder when running the application.

## CSV format

The file has four columns:

```csv
id,title,author,available
```

The availability value is `1` for available and `0` for issued. If `books.csv` is missing, the program starts with an empty catalogue and creates the file when the first change is saved.

## Test checklist

1. List the sample books.
2. Search for book ID `101`.
3. Issue book ID `101`, then list books to confirm its status changes.
4. Try issuing ID `101` again; the program should show an error.
5. Return book ID `101`.
6. Search for an unknown ID such as `999`.
7. Add a new book, exit, restart the program and confirm the new record is still present.

## Class diagram

See [docs/class-diagram.mmd](docs/class-diagram.mmd).

## Sample demonstration

See [docs/sample-output.md](docs/sample-output.md).

## Note

The sample records are provided for demonstration. Add your own realistic records during testing and capture screenshots of your actual program run for submission.
