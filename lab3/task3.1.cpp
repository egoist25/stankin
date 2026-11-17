#include <iostream>
#include <vector>
#include <memory>
#include <random>

using namespace std;

class Base {
protected:
    int value;

public:
    explicit Base(int v) : value(v) {}

    virtual ~Base() = default;

    virtual void show() const {
        cout << "Объект Base, значение: " << value << endl;
    }

    virtual unique_ptr<Base> clone() const {
        return make_unique<Base>(*this);
    }
};

class Derived : public Base {
private:
    int extraValue;

public:
    Derived(int v, int extra)
        : Base(v), extraValue(extra) {}

    void show() const override {
        cout << "Объект Derived, значение: " << value
             << ", дополнительное значение: " << extraValue << endl;
    }

    unique_ptr<Base> clone() const override {
        return make_unique<Derived>(*this);
    }
};

vector<unique_ptr<Base>> storage;

void add(const Base& object) {
    storage.push_back(object.clone());
}

int main() {
    random_device rd;
    mt19937 generator(rd());
    uniform_int_distribution<int> distribution(0, 99);

    cout << "Создание объектов:\n\n";

    for (int i = 0; i < 6; ++i) {
        int randomNumber = distribution(generator);

        if (randomNumber % 2 == 0) {
            Base object(randomNumber);
            add(object);
            cout << "Создан Base\n";
        } else {
            Derived object(randomNumber, randomNumber * 2);
            add(object);
            cout << "Создан Derived\n";
        }
    }

    cout << "\nПоведение объектов в хранилище:\n\n";

    for (size_t i = 0; i < storage.size(); ++i) {
        cout << "Объект " << i + 1 << ": ";
        storage[i]->show();
    }

    storage.clear();

    cout << "\nХранилище очищено, ресурсы освобождены.\n";

    return 0;
}
