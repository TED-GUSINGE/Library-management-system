#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include <stdexcept>
#include <limits>

// Base class: common details of every library item.
class LibraryItem {
private:
    int id;
    std::string title;

public:
    LibraryItem(int bookId, std::string bookTitle)
        : id(bookId), title(bookTitle) {}

    virtual ~LibraryItem() = default;

    int getId() const { return id; }
    std::string getTitle() const { return title; }

    virtual void display() const = 0;
};

// Book inherits ID and title from LibraryItem.
class Book : public LibraryItem {
private:
    std::string author;
    bool available;

public:
    Book(int id, std::string title, std::string author)
        : LibraryItem(id, title), author(author), available(true) {}

    Book(int id, std::string title, std::string author, bool status)
        : LibraryItem(id, title), author(author), available(status) {}

    void display() const override {
        std::cout << getId() << " | " << getTitle() << " | "
                  << author << " | "
                  << (available ? "Available" : "Issued") << '\n';
    }

    void issue() {
        if (!available)
            throw std::runtime_error("This book is already issued.");
        available = false;
    }

    void returnBook() {
        if (available)
            throw std::runtime_error("This book is already available.");
        available = true;
    }

    bool isAvailable() const { return available; }
    std::string getAuthor() const { return author; }
};

class Library {
private:
    std::vector<Book> books; // STL vector stores all books.

    Book& findBook(int id) {
        for (Book& book : books) {
            if (book.getId() == id)
                return book;
        }
        throw std::runtime_error("Book ID not found.");
    }

public:
    void loadBooks() {
        std::ifstream file("books.csv");
        if (!file) return; // Start with an empty library if file is missing.

        std::string line;
        std::getline(file, line); // Skip the CSV heading.

        while (std::getline(file, line)) {
            std::stringstream row(line);
            std::string idText, title, author, status;

            std::getline(row, idText, ',');
            std::getline(row, title, ',');
            std::getline(row, author, ',');
            std::getline(row, status, ',');

            try {
                books.emplace_back(std::stoi(idText), title, author,
                                   status == "1");
            } catch (const std::exception&) {
                // Ignore an invalid row and continue loading other books.
            }
        }
    }

    void saveBooks() const {
        std::ofstream file("books.csv");
        if (!file)
            throw std::runtime_error("Could not save books.csv.");

        file << "id,title,author,available\n";
        for (const Book& book : books) {
            file << book.getId() << ',' << book.getTitle() << ','
                 << book.getAuthor() << ','
                 << (book.isAvailable() ? 1 : 0) << '\n';
        }
    }

    void addBook(int id, const std::string& title,
                 const std::string& author) {
        if (id <= 0 || title.empty() || author.empty())
            throw std::runtime_error("Enter a valid ID, title and author.");

        for (const Book& book : books) {
            if (book.getId() == id)
                throw std::runtime_error("That book ID already exists.");
        }

        books.emplace_back(id, title, author);
        saveBooks();
        std::cout << "Book added successfully.\n";
    }

    void listBooks() const {
        if (books.empty()) {
            std::cout << "No books in the library.\n";
            return;
        }

        std::cout << "\nID | Title | Author | Status\n";
        for (const Book& book : books) {
            const LibraryItem* item = &book;
            item->display(); // Runtime polymorphism.
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

int readNumber(const std::string& message) {
    int number;
    while (true) {
        std::cout << message;
        if (std::cin >> number) {
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            return number;
        }

        std::cout << "Please enter a valid number.\n";
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    }
}

std::string readText(const std::string& message) {
    std::string value;
    do {
        std::cout << message;
        std::getline(std::cin, value);
        if (value.empty()) std::cout << "This field cannot be empty.\n";
    } while (value.empty());
    return value;
}

int main() {
    Library library;

    try {
        library.loadBooks();
        int choice;

        do {
            std::cout << "\n===== LIBRARY MANAGEMENT =====\n"
                      << "1. Add Book\n"
                      << "2. List Books\n"
                      << "3. Search Book\n"
                      << "4. Issue Book\n"
                      << "5. Return Book\n"
                      << "0. Save and Exit\n";

            choice = readNumber("Enter choice: ");

            try {
                int id;
                switch (choice) {
                    case 1:
                        id = readNumber("Enter book ID: ");
                        library.addBook(id, readText("Enter title: "),
                                        readText("Enter author: "));
                        break;
                    case 2:
                        library.listBooks();
                        break;
                    case 3:
                        id = readNumber("Enter book ID: ");
                        library.searchBook(id);
                        break;
                    case 4:
                        id = readNumber("Enter book ID: ");
                        library.issueBook(id);
                        break;
                    case 5:
                        id = readNumber("Enter book ID: ");
                        library.returnBook(id);
                        break;
                    case 0:
                        library.saveBooks();
                        std::cout << "Records saved. Goodbye!\n";
                        break;
                    default:
                        std::cout << "Invalid menu choice.\n";
                }
            } catch (const std::exception& error) {
                std::cout << "Error: " << error.what() << '\n';
            }
        } while (choice != 0);

    } catch (const std::exception& error) {
        std::cout << "Application error: " << error.what() << '\n';
        return 1;
    }

    return 0;
}
