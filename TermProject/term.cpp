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
    while (!current_words.empty()) {
        clearScreen();  // ȭ�� �����

        for (const auto& word : current_words) {  // �ܾ� ��� ���
            cout << word << endl;
        }

        cout << "Enter a word: ";
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

                // �� �̵�
                for (auto iter = it; iter != current_words.begin(); --iter) {
                    iter->swap(*(iter - 1));
                }

                // �� �ܾ� �߰�
                if (!words.empty()) {
                    int index = rand() % words.size();
                    int space = rand() % 4 * 10;  // �ܼ�â�� x������ 4����Ͽ� ������ ��ġ�� ���ڿ� ���
                    current_words[0] = string(space, ' ') + words[index];  // �� ���� �� �ܾ� �߰�
                    words.erase(words.begin() + index);
                }

                // �ܾ ��� ������ �� ��� ���Ϳ��� ����
                if (it->find_first_not_of(' ') == string::npos) {
                    current_words.erase(it);
                }

                break;  // �ܾ ã�Ƽ� �����ϰ� ���ο� �ܾ �߰������Ƿ� �� �̻� �ݺ��� �ʿ� ����
            }
        }

        // �� �Ʒ� �ܾ �����Ǵ� ��� miss ����
        if (current_words.size() == 10 && current_words.back().find_first_not_of(' ') == string::npos) {
            current_words.pop_back();
            miss++;
        }

        // ��� �ܾ ���Ǿ����� Ȯ��
        if (used_words.size() == 13) {
            cout << "All words have been used and deleted. Game over!" << endl;
            cout << "Missed: " << miss << endl;  // �� �̻� �̵��� �� ���� �ܾ��� �� ���
            cout << "Score: " << score << endl;  // ���� ���
            break;
        }
    }

    return 0;
}
