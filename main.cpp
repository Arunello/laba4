#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <cctype>
#include <limits>
#include <sstream>
#include <algorithm>

using namespace std;

int searchSubstring(string &text, string &pattern) {
    int n = text.length();
    int m = pattern.length();
    int count = 0;

    for (int i = 0; i <= n - m; i++) {
        int j;
        for (j = 0; j < m; j++) {
            if (text[i + j] != pattern[j])
                break;
        }
        if (j == m) {
            cout << "Позиция: " << i << "\n";
            count++;
        }
    }
    return count;
}

vector<int> computeLps(string pattern) {
    int n = pattern.length();
    vector<int> lps(n, 0);
    int len = 0;
    int i = 1;

    while (i < n) {
        if (pattern[i] == pattern[len]) {
            len++;
            lps[i] = len;
            i++;
        } else {
            if (len != 0) len = lps[len - 1];
            else { lps[i] = 0; i++; }
        }
    }
    return lps;
}

vector<int> kmp(string text, string pattern) {
    vector<int> lps = computeLps(pattern);
    vector<int> occurrences;
    int n = text.length();
    int m = pattern.length();
    int i = 0, j = 0;

    while (i < n) {
        if (text[i] == pattern[j]) { i++; j++; }
        if (j == m) { occurrences.push_back(i - j); j = lps[j - 1]; }
        else if (i < n && text[i] != pattern[j]) {
            if (j != 0) j = lps[j - 1];
            else i++;
        }
    }
    return occurrences;
}

int main() {
    cout << "Выберите способ ввода:\n1 - с клавиатуры\n2 - из файла\n> ";
    int choice;
    cin >> choice;
    cin.ignore();

    string input;
    if (choice == 1) {
        cout << "Введите строку:\n";
        getline(cin, input);
    } else if (choice == 2) {
        ifstream file("input.txt");
        if (!file) { cout << "Файл input.txt не найден\n"; return 1; }
        getline(file, input);
        file.close();
    } else { 
        cout << "Неверный выбор\n"; return 1; 
    }

    if (input.empty() || input.back() != '.') {
        cout << "Ошибка: строка должна заканчиваться точкой\n";
        return 1;
    }

    cout << "\nКоличество символов в каждом слове исходной последовательности:\n";
    istringstream issInput(input);
    vector<string> originalWords;
    string word;
    while (issInput >> word) {
        cout << word << ": " << word.length() << "\n";
        originalWords.push_back(word);
    }

    if (originalWords.size() < 1 || originalWords.size() > 50) {
        cout << "Ошибка: количество слов должно быть от 1 до 50\n";
        return 1;
    }
    for (int i = 0; i < originalWords.size(); i++) {
        if (originalWords[i].length() < 1 || originalWords[i].length() > 10) {
            cout << "Ошибка: длина слова \"" << originalWords[i] << "\" должна быть от 1 до 10 символов\n";
            return 1;
        }
    }

    string edited;
    int i = 0;
    while (i < input.length()) {
        char c = input[i];

        if (c == '.' && i + 2 < input.length() && input[i+1] == '.' && input[i+2] == '.') {
            if (!edited.empty() && edited.back() == ' ') {
                edited.pop_back();
            }
            edited += "...";
            i += 3;
            continue;
        }

        if (isspace(c)) {
            if (!edited.empty() && edited.back() != ' ') {
                edited += ' ';
            }
            i++;
            continue;
        }

        if (ispunct(c)) {
            if (!edited.empty() && ispunct(edited.back())) {
                i++;
                continue;
            } else {
                edited += c;
                i++;
                continue;
            }
        }

        edited += tolower(c);
        i++;
    }

    if (edited.size() >= 2 && edited[edited.size()-2] == ' ' && edited.back() == '.') {
        edited.erase(edited.size()-2, 1);
    }

    cout << "\nОтредактированный текст:\n" << edited << "\n";


    vector<string> words;
    istringstream issEdited(edited);
    while (issEdited >> word) {
        words.push_back(word);
    }

    cout << "\nСлова в обратном порядке:\n";
    for (int i = words.size()-1; i >= 0; i--) {
        cout << words[i] << " ";
    }
    cout << "\n";

    vector<string> sortedWords = words;
    sort(sortedWords.begin(), sortedWords.end());
    cout << "\nСлова в алфавитном порядке:\n";
    for (int i = 0; i < sortedWords.size(); i++) {
        cout << sortedWords[i] << " ";
    }
    cout << "\n";

    cout << "\nСлова вертикально:\n";
    for (int i = 0; i < words.size(); i++) {
        cout << words[i] << "\n";
    }

    string pattern;
    cout << "\nВведите подстроку для поиска:\n> ";
    getline(cin, pattern);
    for (int i = 0; i < pattern.length(); i++) {
        pattern[i] = tolower(pattern[i]);
    }

    cout << "\nЛинейный поиск:\n";
    int linearCount = searchSubstring(edited, pattern);
    cout << "Найдено " << linearCount << " вхождений.\n";

    vector<int> kmpPositions = kmp(edited, pattern);
    cout << "\nKMP поиск: найдено " << kmpPositions.size() << " вхождений.\n";
    for (int i = 0; i < kmpPositions.size(); i++) {
        cout << "Позиция: " << kmpPositions[i] << "\n";
    }
    return 0;
}
