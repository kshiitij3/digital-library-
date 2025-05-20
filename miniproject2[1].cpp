#include <iostream>
#include <fstream>
#include <string>
using namespace std;

class Book {
    int bookID;
    string title, author;

public:
    Book() {
        bookID = 0;
        title = "";
        author = "";
    }

    void input() {
        cout << "\nEnter Book ID: ";
        cin >> bookID;
        cin.ignore();

        cout << "Enter Title: ";
        getline(cin, title);

        cout << "Enter Author: ";
        getline(cin, author);
    }

    void display() const {
        cout << "\nBook ID   : " << bookID;
        cout << "\nTitle     : " << title;
        cout << "\nAuthor    : " << author << "\n";
    }

    int getBookID() const {
        return bookID;
    }
    // Write object
    void writeToFile(ofstream& out) const {
        out.write(reinterpret_cast<const char*>(&bookID), sizeof(bookID));

        size_t len = title.size();
        out.write(reinterpret_cast<const char*>(&len), sizeof(len));
        out.write(title.c_str(), len);

        len = author.size();
        out.write(reinterpret_cast<const char*>(&len), sizeof(len));
        out.write(author.c_str(), len);
    }

    // Read object 
    bool readFromFile(ifstream& in) {
        if (!in.read(reinterpret_cast<char*>(&bookID), sizeof(bookID))) return false;

        size_t len;

        if (!in.read(reinterpret_cast<char*>(&len), sizeof(len))) return false;
        title.resize(len);
        in.read(&title[0], len);

        if (!in.read(reinterpret_cast<char*>(&len), sizeof(len))) return false;
        author.resize(len);
        in.read(&author[0], len);

        return true;
    }
};

// Function to add a book to the file
void addBook() {
    ofstream file("library.dat", ios::binary | ios::app);
    if (!file) {
        cerr << "Error opening file to write.\n";
        return;
    }

    Book b;
    b.input();
    b.writeToFile(file);
    file.close();

    cout << "Book added successfully.\n";
}

// Function to display all books
void displayBooks() {
    ifstream file("library.dat", ios::binary);
    if (!file) {
        cerr << "Error opening file to read.\n";
        return;
    }

    Book b;
    cout << "\n--- All Books ---";
    bool any = false;

    while (b.readFromFile(file)) {
        b.display();
        any = true;
    }

    if (!any) {
        cout << "\nNo books found.\n";
    }

    file.close();
}

// Function to delete a book by ID
void deleteBook() {
    ifstream file("library.dat", ios::binary);
    if (!file) {
        cerr << "Error opening file to read.\n";
        return;
    }

    ofstream temp("temp.dat", ios::binary);
    if (!temp) {
        cerr << "Error creating temporary file.\n";
        return;
    }

    Book b;
    int id;
    bool found = false;

    cout << "Enter Book ID to delete: ";
    cin >> id;
    cin.ignore();  

    while (b.readFromFile(file)) {
        if (b.getBookID() != id) {
            b.writeToFile(temp);
        } else {
            found = true;
        }
    }

    file.close();
    temp.close();

    remove("library.dat");
    rename("temp.dat", "library.dat");

    if (found)
        cout << "Book deleted successfully.\n";
    else
        cout << "Book not found.\n";
}

// Main program
int main() {
    int choice;
    do {
        cout << "\n===== Library Management Menu =====";
        cout << "\n1. Add Book";
        cout << "\n2. Display All Books";
        cout << "\n3. Delete Book by ID";
        cout << "\n4. Exit";
        cout << "\nEnter your choice: ";
        cin >> choice;
        cin.ignore(); // clear newline from input buffer

        switch (choice) {
            case 1: addBook(); break;
            case 2: displayBooks(); break;
            case 3: deleteBook(); break;
            case 4: cout << "Exiting program.\n"; break;
            default: cout << "Invalid choice! Try again.\n";
        }
    } while (choice != 4);

    return 0;
}
