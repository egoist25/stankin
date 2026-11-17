#include <iostream>
#include <string>
#include <clocale>

using namespace std;

class Publication {
protected:
    string title;
    float price;

public:
    void getdata() {
        cout << "Введите название издания: ";
        getline(cin >> ws, title);

        cout << "Введите цену издания: ";
        cin >> price;
    }

    void putdata() const {
        cout << "Название: " << title << endl;
        cout << "Цена: " << price << " руб." << endl;
    }
};

class sales {
protected:
    float monthlySales[3];

public:
    void getdata() {
        for (int i = 0; i < 3; i++) {
            cout << "Введите продажи за месяц " << i + 1 << ": ";
            cin >> monthlySales[i];
        }
    }

    void putdata() const {
        cout << "Продажи за три месяца:\n";

        for (int i = 0; i < 3; i++) {
            cout << "Месяц " << i + 1 << ": "
                 << monthlySales[i] << " руб." << endl;
        }
    }
};

class book : public Publication, public sales {
private:
    int pages;

public:
    void getdata() {
        cout << "\n=== Ввод данных о книге ===\n";

        Publication::getdata();

        cout << "Введите количество страниц: ";
        cin >> pages;

        sales::getdata();
    }

    void putdata() const {
        cout << "\n=== Информация о книге ===\n";

        Publication::putdata();

        cout << "Количество страниц: " << pages << endl;

        sales::putdata();
    }
};

class type : public Publication, public sales {
private:
    int minutes;

public:
    void getdata() {
        cout << "\n=== Ввод данных об аудиокниге ===\n";

        Publication::getdata();

        cout << "Введите продолжительность записи в минутах: ";
        cin >> minutes;

        sales::getdata();
    }

    void putdata() const {
        cout << "\n=== Информация об аудиокниге ===\n";

        Publication::putdata();

        cout << "Продолжительность записи: "
             << minutes << " мин." << endl;

        sales::putdata();
    }
};

int main() {
    setlocale(LC_ALL, "");

    book printedBook;
    type audioBook;

    printedBook.getdata();
    audioBook.getdata();

    cout << "\n========== РЕЗУЛЬТАТЫ ==========\n";

    printedBook.putdata();
    audioBook.putdata();

    return 0;
}
