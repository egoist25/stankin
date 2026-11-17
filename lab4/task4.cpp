#include <iostream>
#include <string>
#include <stdexcept>
#include <new>
#include <fstream>
#include <limits>

class ProgramError : public std::exception {
private:
    std::string message;

public:
    explicit ProgramError(const std::string& msg) : message(msg) {}

    const char* what() const noexcept override {
        return message.c_str();
    }
};

class MemoryError : public ProgramError {
public:
    MemoryError() : ProgramError("Недостаточно памяти для выделения массива.") {}
};

class IOError : public ProgramError {
public:
    explicit IOError(const std::string& msg) : ProgramError(msg) {}
};

class FileReadError : public IOError {
public:
    explicit FileReadError(const std::string& fileName)
        : IOError("Ошибка чтения файла: " + fileName) {}
};

class FileWriteError : public IOError {
public:
    explicit FileWriteError(const std::string& fileName)
        : IOError("Ошибка записи файла: " + fileName) {}
};

template <typename T>
class PointerArray {
private:
    T** items;
    std::size_t count;
    std::size_t capacity;

    void resize(std::size_t newCapacity) {
        T** newItems = nullptr;

        try {
            newItems = new T*[newCapacity];
        }
        catch (const std::bad_alloc&) {
            throw MemoryError();
        }

        for (std::size_t i = 0; i < count; ++i) {
            newItems[i] = items[i];
        }

        delete[] items;
        items = newItems;
        capacity = newCapacity;
    }

public:
    explicit PointerArray(std::size_t initialCapacity = 2)
        : items(nullptr), count(0), capacity(initialCapacity) {
        if (capacity == 0) {
            capacity = 1;
        }

        try {
            items = new T*[capacity];
        }
        catch (const std::bad_alloc&) {
            throw MemoryError();
        }
    }

    PointerArray(const PointerArray&) = delete;
    PointerArray& operator=(const PointerArray&) = delete;

    ~PointerArray() {
        delete[] items;
    }

    void add(T* object) {
        if (object == nullptr) {
            throw std::invalid_argument("Нельзя добавить нулевой указатель.");
        }

        if (count == capacity) {
            resize(capacity * 2);
        }

        items[count++] = object;
    }

    T*& operator[](std::size_t index) {
        if (index >= count) {
            throw std::out_of_range("Индекс выходит за границы массива указателей.");
        }
        return items[index];
    }

    T* const& operator[](std::size_t index) const {
        if (index >= count) {
            throw std::out_of_range("Индекс выходит за границы массива указателей.");
        }
        return items[index];
    }

    std::size_t size() const {
        return count;
    }
};

class Person {
private:
    int id;
    std::string name;

public:
    Person(int id, const std::string& name) : id(id), name(name) {}

    void print() const {
        std::cout << "ID: " << id << ", имя: " << name << '\n';
    }
};

class Book {
private:
    std::string title;
    int year;

public:
    Book(const std::string& title, int year) : title(title), year(year) {}

    void print() const {
        std::cout << "Книга: " << title << ", год издания: " << year << '\n';
    }
};

void readFile(const std::string& fileName) {
    std::ifstream file(fileName);

    if (!file.is_open()) {
        throw FileReadError(fileName);
    }

    std::string line;
    while (std::getline(file, line)) {
        std::cout << line << '\n';
    }

    if (file.bad()) {
        throw FileReadError(fileName);
    }
}

void writeFile(const std::string& fileName, const std::string& text) {
    std::ofstream file(fileName);

    if (!file.is_open()) {
        throw FileWriteError(fileName);
    }

    file << text;

    if (!file.good()) {
        throw FileWriteError(fileName);
    }
}

int squareWithRangeCheck(int number) {
    const long long result = static_cast<long long>(number) * number;

    if (result > std::numeric_limits<int>::max()) {
        throw std::range_error("Результат возведения в квадрат превышает диапазон типа int.");
    }

    return static_cast<int>(result);
}

int main() {
    setlocale(LC_ALL, "Russian");

    try {
        std::cout << "=== Шаблонный класс с объектами Person ===\n";

        Person person1(1, "Андрей");
        Person person2(2, "Иван");

        PointerArray<Person> people;
        people.add(&person1);
        people.add(&person2);

        for (std::size_t i = 0; i < people.size(); ++i) {
            people[i]->print();
        }

        std::cout << "\n=== Тот же шаблонный класс с объектами Book ===\n";

        Book book1("Программирование на C++", 2022);
        Book book2("Алгоритмы и структуры данных", 2020);

        PointerArray<Book> books;
        books.add(&book1);
        books.add(&book2);

        for (std::size_t i = 0; i < books.size(); ++i) {
            books[i]->print();
        }

        std::cout << "\n=== Демонстрация изменения указателя через [] ===\n";
        Person person3(3, "Мария");
        people[1] = &person3;
        people[1]->print();

        std::cout << "\n=== Обработка range_error (вариант 2) ===\n";
        try {
            std::cout << "Квадрат числа 100: " << squareWithRangeCheck(100) << '\n';
            std::cout << "Квадрат числа 50000: " << squareWithRangeCheck(50000) << '\n';
        }
        catch (const std::range_error& error) {
            std::cout << "Перехвачено исключение range_error: " << error.what() << '\n';
        }

        std::cout << "\n=== Демонстрация ошибок чтения и записи файлов ===\n";

        try {
            readFile("file_that_does_not_exist.txt");
        }
        catch (const FileReadError& error) {
            std::cout << error.what() << '\n';
        }

        try {
            writeFile("result.txt", "Проверка записи в файл.\n");
            std::cout << "Данные успешно записаны в файл result.txt\n";
        }
        catch (const FileWriteError& error) {
            std::cout << error.what() << '\n';
        }
    }
    catch (const MemoryError& error) {
        std::cerr << "Ошибка памяти: " << error.what() << '\n';
        return 1;
    }
    catch (const ProgramError& error) {
        std::cerr << "Ошибка программы: " << error.what() << '\n';
        return 1;
    }
    catch (const std::exception& error) {
        std::cerr << "Стандартное исключение: " << error.what() << '\n';
        return 1;
    }

    return 0;
}
