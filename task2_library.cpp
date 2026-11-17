#include <iostream>
#include <string>
#include <vector>
#include <limits>
#include <stdexcept>

enum class BookType {
    Fiction,
    Technical
};

class Book {
private:
    std::string author;
    std::string title;
    BookType type;

public:
    Book(const std::string& authorName,
         const std::string& bookTitle,
         BookType bookType)
        : author(authorName), title(bookTitle), type(bookType) {
        if (author.empty() || title.empty()) {
            throw std::invalid_argument("Автор и название книги не могут быть пустыми.");
        }
    }

    const std::string& getAuthor() const { return author; }
    const std::string& getTitle() const { return title; }
    BookType getType() const { return type; }

    void print() const {
        std::cout << "Название: " << title
                  << " | Автор: " << author
                  << " | Тип: "
                  << (type == BookType::Fiction ? "художественная" : "техническая")
                  << '\n';
    }
};

class Library {
private:
    std::vector<Book> books;

public:
    void addBook(const Book& book) {
        books.push_back(book);
    }

    void printAll() const {
        if (books.empty()) {
            std::cout << "Библиотека пуста.\n";
            return;
        }

        std::cout << "\nСписок книг в библиотеке:\n";
        for (std::size_t i = 0; i < books.size(); ++i) {
            std::cout << i + 1 << ". ";
            books[i].print();
        }
    }
};

int main() {
    setlocale(LC_ALL, "Russian");

    Library library;

    library.addBook(Book("Фёдор Достоевский", "Преступление и наказание",
                         BookType::Fiction));
    library.addBook(Book("Михаил Булгаков", "Мастер и Маргарита",
                         BookType::Fiction));
    library.addBook(Book("Бьярне Страуструп", "Язык программирования C++",
                         BookType::Technical));
    library.addBook(Book("Эндрю Таненбаум", "Архитектура компьютера",
                         BookType::Technical));

    library.printAll();
    return 0;
}
