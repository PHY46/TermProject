#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <cstdlib>
#include <ctime>

using namespace std;

// ȭ���� ����� �Լ�
void clearScreen() {
    cout << "\033[2J\033[1;1H";
}

int main() {
    srand(static_cast<unsigned int>(time(0)));
    vector<string> words = { "apple", "banana", "cherry", "durian", "elephant", "falcon", "giraffe", "orange", "grape", "bear", "cat", "dog", "duck" };
    vector<string> current_words(10, "");  // 10���� �ܾ� ���
    vector<string> used_words; // ���� �ܾ���� �����ϴ� ����
    int miss = 0;  // �� �̻� �̵��� �� ���� �ܾ��� ��
    int score = 0;  // ����

    // �� �ܾ� �߰�
    for (auto& word : current_words) {
        if (words.empty()) break; // �ܾ �� �̻� ������ ����
        int index = rand() % words.size();
        int space = rand() % 4 * 10;  // �ܼ�â�� x������ 4����Ͽ� ������ ��ġ�� ���ڿ� ���
        word = string(space, ' ') + words[index];  // �ش� ��ġ�� �ܾ� ��Ͽ� �߰�
        words.erase(words.begin() + index);
    }

    string input = "";
    while (true) {
        clearScreen();  // ȭ�� �����

        for (const auto& word : current_words) {  // �ܾ� ��� ���
            cout << word << endl;
        }

        cout << "Enter a word (" << "\"exit\"" << " to quit): ";
        cin >> input;
        cin.ignore();

        // exit �Է� �� ���� ����
        if (input == "exit") {
            cout << "Game over!" << endl;
            cout << "Missed: " << miss << endl;  // �� �̻� �̵��� �� ���� �ܾ��� �� ���
            cout << "Score: " << score << endl;  // ���� ���
            break;
        }

        // �Է��� �ܾ ��Ͽ� �ִ��� Ȯ���ϰ�, �ִٸ� �ش� �ܾ �������� ���� �� �� �̵�
        for (auto it = current_words.begin(); it != current_words.end(); ++it) {
            size_t pos = it->find(input);
            if (pos != string::npos) {
                it->replace(pos, input.length(), string(input.length(), ' '));  // �ܾ �������� ����
                used_words.push_back(input);  // ���� �ܾ� �߰�
                score += 10;  // ���� ����

                // �� �Ʒ� �ܾ �ƴϸ� �� �Ʒ� �ܾ ���Ϳ��� �����ϰ�, �� ���� �ִ� �ܾ���� �� �پ� �Ʒ��� �������� �ϰ�, miss ����
                if (it != current_words.end() - 1) {
                    current_words.erase(current_words.end() - 1);
                    miss++;

                    // �� �̵�
                    for (auto iter = it; iter != current_words.begin(); --iter) {
                        iter->swap(*(iter - 1));
                    }
                }
                else {
                    // �� �Ʒ� �ܾ ���Ϳ��� ����
                    current_words.erase(it);
                }

                // �� �ܾ� �߰�
                if (!words.empty()) {
                    int index = rand() % words.size();
                    int space = rand() % 4 * 10;  // �ܼ�â�� x������ 4����Ͽ� ������ ��ġ�� ���ڿ� ���
                    current_words.insert(current_words.begin(), string(space, ' ') + words[index]);  // �� ���� �� �ܾ� �߰�
                    words.erase(words.begin() + index);
                }
                else {
                    // �� �ܾ ������ �� ���ڿ� �߰�
                    current_words.insert(current_words.begin(), "");
                }

                break;  // �ܾ ã�Ƽ� �����ϰ� ���ο� �ܾ �߰������Ƿ� �� �̻� �ݺ��� �ʿ� ����
            }
        }
    }

    return 0;
}
