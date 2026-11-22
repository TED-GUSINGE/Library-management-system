# Sample Console Output

The following is an illustrative session using the sample records in `books.csv`. Exact spacing may vary by terminal.

```text
Welcome! Library records loaded.

========== LIBRARY MANAGEMENT SYSTEM ==========
1. Add a book
2. List all books
3. Search for a book
4. Issue a book
5. Return a book
0. Save and exit
================================================
Enter your choice: 2

ID | Title | Author | Status
------------------------------------------------------------
101 | The Alchemist | Paulo Coelho | Available
102 | Wings of Fire | A. P. J. Abdul Kalam | Available
103 | The Guide | R. K. Narayan | Issued

Enter your choice: 4
Enter book ID to issue: 101
Book issued successfully.

Enter your choice: 4
Enter book ID to issue: 101
Error: This book is already issued.

Enter your choice: 3
Enter book ID to search: 999
Book error: No book exists with that ID.

Enter your choice: 0
Records saved. Goodbye!
```

## Demonstration cases

- **Normal:** list, search, add, issue and return a valid book.
- **Error:** issue an already-issued book.
- **Error:** search for an ID that does not exist.
- **Persistence:** add or issue a book, exit, restart and check that the CSV record was retained.

Run the program yourself and replace or supplement this illustrative transcript with screenshots of your actual run before submission.
