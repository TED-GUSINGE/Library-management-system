#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include <memory>
#include <stdexcept>
#include <limits>
#include <cctype>

// Base class: common information shared by all library items.
class LibraryItem {
private:
    int id;
    std::string title;

public:
    LibraryItem(int bookId, const std::string& bookTitle)
        : id(bookId), title(bookTitle) {}

    virtual ~LibraryItem() = default;

    int getId() const {
        return id;
    }

    std::string getTitle() const {
        return title;
    }

    // A pure virtual function makes LibraryItem an abstract class.
    virtual void display() const = 0;
};

// Book inherits common fields from LibraryItem.
class Book : public LibraryItem {
private:
    std::string author;
    bool available;

public:
    Book(int id, const std::string& title, const std::string& bookAuthor,
         bool isAvailable = true)
        : LibraryItem(id, title), author(bookAuthor), available(isAvailable) {}

    std::string getAuthor() const {
        return author;
    }

    bool isAvailable() const {
        return available;
    }

    void issue() {
        if (!available) {
            throw std::runtime_error("This book is already issued.");
        }
        available = false;
    }

    void returnBook() {
        if (available) {
            throw std::runtime_error("This book is already in the library.");
        }
        available = true;
    }

    void display() const override {
        std::cout << getId() << " | " << getTitle() << " | " << author
                  << " | " << (available ? "Available" : "Issued") << '\n';
    }
};

// Custom exceptions make common library errors easier to identify.
class InvalidBookId : public std::runtime_error {
public:
    InvalidBookId() : std::runtime_error("No book exists with that ID.") {}
};

class DuplicateBookId : public std::runtime_error {
public:
    DuplicateBookId() : std::runtime_error("A book with that ID already exists.") {}
};

class Library {
private:
    std::vector<Book> books;
    const std::string fileName = "books.csv";

    Book& findBook(int id) {
        for (Book& book : books) {
            if (book.getId() == id) {
                return book;
            }
        }
        throw InvalidBookId();
    }

    // Handles quoted CSV fields, including commas inside titles or authors.
    std::vector<std::string> parseCsvLine(const std::string& line) const {
        std::vector<std::string> fields;
        std::string field;
        bool insideQuotes = false;

        for (std::size_t i = 0; i < line.size(); ++i) {
            char ch = line[i];

            if (ch == '"') {
                if (insideQuotes && i + 1 < line.size() && line[i + 1] == '"') {
                    field += '"';
                    ++i;
                } else {
                    insideQuotes = !insideQuotes;
                }
            } else if (ch == ',' && !insideQuotes) {
                fields.push_back(field);
                field.clear();
            } else {
                field += ch;
            }
        }

        fields.push_back(field);
        return fields;
    }

    std::string escapeCsv(const std::string& value) const {
        std::string escaped = "\"";
        for (char ch : value) {
            if (ch == '"') {
                escaped += "\"\"";
            } else {
                escaped += ch;
            }
        }
        escaped += "\"";
        return escaped;
    }

public:
    void loadBooks() {
        std::ifstream file(fileName);

        // The file is created when the first save happens.
        if (!file.is_open()) {
            return;
        }

        std::string line;
        while (std::getline(file, line)) {
            if (line.empty() || line.rfind("id,", 0) == 0) {
                continue; // Skip blank lines and the CSV header.
            }

            try {
                std::vector<std::string> fields = parseCsvLine(line);
                if (fields.size() != 4) {
                    continue;
                }

                int id = std::stoi(fields[0]);
                bool available = (fields[3] == "1");

                bool alreadyExists = false;
                for (const Book& book : books) {
                    if (book.getId() == id) {
                        alreadyExists = true;
                        break;
                    }
                }

                if (!alreadyExists) {
                    books.emplace_back(id, fields[1], fields[2], available);
                }
            } catch (const std::exception&) {
                // Ignore a malformed row so one bad line does not stop loading.
            }
        }
    }

    void saveBooks() const {
        std::ofstream file(fileName);

        if (!file.is_open()) {
            throw std::runtime_error("Could not open books.csv for writing.");
        }

        file << "id,title,author,available\n";
        for (const Book& book : books) {
            file << book.getId() << ','
                 << escapeCsv(book.getTitle()) << ','
                 << escapeCsv(book.getAuthor()) << ','
                 << (book.isAvailable() ? "1" : "0") << '\n';
        }

        if (!file) {
            throw std::runtime_error("An error occurred while saving the book records.");
        }
    }

    void addBook(int id, const std::string& title, const std::string& author) {
        if (id <= 0 || title.empty() || author.empty()) {
            throw std::invalid_argument("ID, title and author must be valid.");
        }

        for (const Book& book : books) {
            if (book.getId() == id) {
                throw DuplicateBookId();
            }
        }

        books.emplace_back(id, title, author);
        saveBooks();
    }

    void listBooks() const {
        if (books.empty()) {
            std::cout << "\nNo books found in the library.\n";
            return;
        }

        std::cout << "\nID | Title | Author | Status\n";
        std::cout << "------------------------------------------------------------\n";

        // Runtime polymorphism: display() is called through a base-class pointer.
        for (const Book& book : books) {
            const LibraryItem* item = &book;
            item->display();
        }
    }

    void searchBook(int id) {
        findBook(id).display();
    }

    void issueBook(int id) {
        findBook(id).issue();
        saveBooks();
        std::cout << "Book issued successfully.\n";
    }

    void returnBook(int id) {
        findBook(id).returnBook();
        saveBooks();
        std::cout << "Book returned successfully.\n";
    }
};

int readInteger(const std::string& prompt) {
    while (true) {
        std::cout << prompt;
        std::string input;
        std::getline(std::cin, input);

        std::stringstream stream(input);
        int value;
        char extra;

        if (stream >> value && !(stream >> extra)) {
            return value;
        }

        std::cout << "Please enter a valid whole number.\n";
    }
}

std::string readText(const std::string& prompt) {
    std::string value;
    while (true) {
        std::cout << prompt;
        std::getline(std::cin, value);

        if (!value.empty()) {
            return value;
        }
        std::cout << "This field cannot be empty.\n";
    }
}

void showMenu() {
    std::cout << "\n========== LIBRARY MANAGEMENT SYSTEM ==========\n";
    std::cout << "1. Add a book\n";
    std::cout << "2. List all books\n";
    std::cout << "3. Search for a book\n";
    std::cout << "4. Issue a book\n";
    std::cout << "5. Return a book\n";
    std::cout << "0. Save and exit\n";
    std::cout << "================================================\n";
}

int main() {
    Library library;

    try {
        library.loadBooks();
        std::cout << "Welcome! Library records loaded.\n";

        bool running = true;
        while (running) {
            showMenu();
            int choice = readInteger("Enter your choice: ");

            try {
                switch (choice) {
                    case 1: {
                        int id = readInteger("Enter book ID: ");
                        std::string title = readText("Enter book title: ");
                        std::string author = readText("Enter author name: ");
                        library.addBook(id, title, author);
                        std::cout << "Book added successfully.\n";
                        break;
                    }
                    case 2:
                        library.listBooks();
                        break;
                    case 3: {
                        int id = readInteger("Enter book ID to search: ");
                        library.searchBook(id);
                        break;
                    }
                    case 4: {
                        int id = readInteger("Enter book ID to issue: ");
                        library.issueBook(id);
                        break;
                    }
                    case 5: {
                        int id = readInteger("Enter book ID to return: ");
                        library.returnBook(id);
                        break;
                    }
                    case 0:
                        library.saveBooks();
                        std::cout << "Records saved. Goodbye!\n";
                        running = false;
                        break;
                    default:
                        std::cout << "Invalid menu choice. Please try again.\n";
                }
            } catch (const InvalidBookId& error) {
                std::cout << "Book error: " << error.what() << '\n';
            } catch (const DuplicateBookId& error) {
                std::cout << "Book error: " << error.what() << '\n';
            } catch (const std::exception& error) {
                std::cout << "Error: " << error.what() << '\n';
            }
        }
    } catch (const std::exception& error) {
        std::cerr << "Application could not start: " << error.what() << '\n';
        return 1;
    }

    return 0;
}
