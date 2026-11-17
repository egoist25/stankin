#include <iostream>
#include <memory>
#include <cstddef>

int main() {
    int n;

    std::cout << "Введите количество элементов массива: ";

    if (!(std::cin >> n) || n <= 0) {
        std::cout << "Ошибка: количество элементов должно быть больше нуля.\n";
        return 1;
    }

    std::unique_ptr<int[]> arr = std::make_unique<int[]>(n);

    for (int i = 0; i < n; ++i) {
        std::cout << "Введите элемент [" << i << "]: ";

        if (!(std::cin >> arr[i])) {
            std::cout << "Ошибка ввода числа.\n";
            return 1;
        }
    }

    const int* start = arr.get();

    std::cout << "\nСодержимое массива:\n";

    for (int i = 0; i < n; ++i) {
        std::cout << "arr[" << i << "] = "
                  << arr[i] << '\n';
    }

    std::cout << "\nАдрес начала массива: "
              << static_cast<const void*>(start) << '\n';

    std::cout << "\nРасстояние элементов от начала массива:\n";

    for (int i = 0; i < n; ++i) {
        const int* current = &arr[i];

        std::ptrdiff_t distanceElements = current - start;

        std::ptrdiff_t distanceBytes =
            reinterpret_cast<const char*>(current) -
            reinterpret_cast<const char*>(start);

        std::cout << "Элемент arr[" << i << "]"
                  << "\nАдрес: "
                  << static_cast<const void*>(current)
                  << "\nРасстояние в элементах: "
                  << distanceElements
                  << "\nРасстояние в байтах: "
                  << distanceBytes
                  << "\n\n";
    }

    return 0;
}
