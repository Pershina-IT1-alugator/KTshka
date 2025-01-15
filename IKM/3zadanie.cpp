#include <iostream>
#include <string>
#include <windows.h>
#include <locale>

using namespace std;

// Функция для проверки, является ли введённая пользователем строка числом
bool this_is_good(const string& cifra) {
    if (cifra.empty()) return false; // если пустая строчка, то выход
    int dot_count = 0; // счетчик для точек
    for (char c : cifra) {
        if (c == '-') {
            if (&c != &cifra[0]) return false; // знак должен быть только в начале
        } else if (c == '.') {
            dot_count++;
            if (dot_count > 1) return false; // только одна точка допустима
        } else if (!isdigit(c)) {
            return false; // если символ не цифра - выход
        }
    }
    return true; // Все символы - цифры
}

// Функция деления чисел (результат записывается в chastnoe и ostatok)
void delenie_chisel(const string& a, const string& b, string& chastnoe, string& ostatok) {
    if (b == "0") {
        throw invalid_argument("Деление на ноль невозможно."); // деление на 0 невозможно
    }
    // определяем знак результата
    bool znak = (a[0] == '-') ^ (b[0] == '-'); //определение знака
    // убираем знак для дальнейших вычислений
    string num_a = (a[0] == '-' || a[0] == '+') ? a.substr(1) : a;
    string num_b = (b[0] == '-' || b[0] == '+') ? b.substr(1) : b;

    // обработка дробной части
    int decimal_places_a = 0;
    int decimal_places_b = 0;

    size_t dot_pos_a = num_a.find('.');
    size_t dot_pos_b = num_b.find('.');

    if (dot_pos_a != string::npos) {
        decimal_places_a = num_a.length() - dot_pos_a - 1;
        num_a.erase(dot_pos_a, 1); // Убираем точку
    }

    if (dot_pos_b != string::npos) {
        decimal_places_b = num_b.length() - dot_pos_b - 1;
        num_b.erase(dot_pos_b, 1); // Убираем точку
    }

    ostatok = "0";  // остаток
    chastnoe = "";  // частное

    for (size_t i = 0; i < num_a.size(); ++i) { // идём по каждой цифре всего числа
        // Добавляем следующую цифру к остатку
        ostatok += num_a[i];

        // убираем ведущие нули
        while (ostatok.length() > 1 && ostatok[0] == '0') {
            ostatok.erase(0, 1);
        }

        // находим текущую цифру частного
        int current_quotient_digit = 0;
        while (ostatok.length() > num_b.length() || (ostatok.length() == num_b.length() && ostatok >= num_b)) {
            ostatok = to_string(stoll(ostatok) - stoll(num_b));
            current_quotient_digit++;
        }

        // добавляем цифру частного к результату
        chastnoe += to_string(current_quotient_digit);
    }

    // убираем ведущие нули из частного
    while (chastnoe.length() > 1 && chastnoe[0] == '0') {
        chastnoe.erase(0, 1);
    }

    // если частное пустое, устанавливаем его в "0"
    if (chastnoe.empty()) {
        chastnoe = "0";
    }

    // меняем знак к частному
    if (znak && chastnoe != "0") {
        chastnoe = "-" + chastnoe;
    }

    // Добавляем дробную часть к результату
    int total_decimal_places = decimal_places_a - decimal_places_b;

    if (total_decimal_places > 0) {
        chastnoe.insert(chastnoe.length() - total_decimal_places, ".");
    } else if (total_decimal_places < 0) {
        chastnoe.insert(0, "0.");
        for (int i = 0; i < abs(total_decimal_places); ++i) {
            chastnoe.insert(2, "0"); // Добавляем нули перед целой частью
        }
    }
}

int main() {
    string a, b;
    string chastnoe, ostatok;

    SetConsoleOutputCP(1251);
    SetConsoleCP(1251);
    setlocale(LC_ALL, "");

    // Запрос ввода чисел у пользователя с проверкой на корректность
    while (true) {
        wcout << L"Введите первое многозначное число (делимое): ";
        cin >> a;
        if (this_is_good(a)) break;
        wcout << L"Ошибка: введите корректное число." << endl;
    }

    while (true) {
        wcout << L"Введите второе многозначное число (делитель): ";
        cin >> b;
        if (this_is_good(b)) break;
        wcout << L"Ошибка: введите корректное число." << endl;
    }

    try {
        delenie_chisel(a, b, chastnoe, ostatok);
        wcout << L"Частное: ";
        cout << chastnoe;
        wcout << L", Остаток: ";
        cout << ostatok << endl;
    } catch (const invalid_argument& e) {
        wcerr << e.what();
    }

    return 0;
}