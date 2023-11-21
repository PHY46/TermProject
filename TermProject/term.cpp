#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <cstdlib>
#include <ctime>

using namespace std;

// 화면을 지우는 함수
void clearScreen() {
    cout << "\033[2J\033[1;1H";
}

int main() {
    srand(static_cast<unsigned int>(time(0)));
    vector<string> words = { "apple", "banana", "cherry", "durian", "elephant", "falcon", "giraffe", "orange", "grape", "bear", "cat", "dog", "duck" };
    vector<string> current_words(10, "");  // 10줄의 단어 목록
    vector<string> used_words; // 사용된 단어들을 저장하는 벡터
    int miss = 0;  // 더 이상 이동할 수 없는 단어의 수
    int score = 0;  // 점수

    // 새 단어 추가
    for (auto& word : current_words) {
        if (words.empty()) break; // 단어가 더 이상 없으면 종료
        int index = rand() % words.size();
        int space = rand() % 4 * 10;  // 콘솔창을 x축으로 4등분하여 랜덤한 위치에 문자열 출력
        word = string(space, ' ') + words[index];  // 해당 위치의 단어 목록에 추가
        words.erase(words.begin() + index);
    }

    string input = "";
    while (!current_words.empty()) {
        clearScreen();  // 화면 지우기

        for (const auto& word : current_words) {  // 단어 목록 출력
            cout << word << endl;
        }

        cout << "Enter a word: ";
        cin >> input;
        cin.ignore();

        // exit 입력 시 게임 종료
        if (input == "exit") {
            cout << "Game over!" << endl;
            cout << "Missed: " << miss << endl;  // 더 이상 이동할 수 없는 단어의 수 출력
            cout << "Score: " << score << endl;  // 점수 출력
            break;
        }

        // 입력한 단어가 목록에 있는지 확인하고, 있다면 해당 단어를 공백으로 변경 후 줄 이동
        for (auto it = current_words.begin(); it != current_words.end(); ++it) {
            size_t pos = it->find(input);
            if (pos != string::npos) {
                it->replace(pos, input.length(), string(input.length(), ' '));  // 단어를 공백으로 변경
                used_words.push_back(input);  // 사용된 단어 추가
                score += 10;  // 점수 증가

                // 줄 이동
                for (auto iter = it; iter != current_words.begin(); --iter) {
                    iter->swap(*(iter - 1));
                }

                // 새 단어 추가
                if (!words.empty()) {
                    int index = rand() % words.size();
                    int space = rand() % 4 * 10;  // 콘솔창을 x축으로 4등분하여 랜덤한 위치에 문자열 출력
                    current_words[0] = string(space, ' ') + words[index];  // 맨 위에 새 단어 추가
                    words.erase(words.begin() + index);
                }

                // 단어가 모두 공백이 된 경우 벡터에서 제거
                if (it->find_first_not_of(' ') == string::npos) {
                    current_words.erase(it);
                }

                break;  // 단어를 찾아서 변경하고 새로운 단어를 추가했으므로 더 이상 반복할 필요 없음
            }
        }

        // 맨 아래 단어가 삭제되는 경우 miss 증가
        if (current_words.size() == 10 && current_words.back().find_first_not_of(' ') == string::npos) {
            current_words.pop_back();
            miss++;
        }

        // 모든 단어가 사용되었는지 확인
        if (used_words.size() == 13) {
            cout << "All words have been used and deleted. Game over!" << endl;
            cout << "Missed: " << miss << endl;  // 더 이상 이동할 수 없는 단어의 수 출력
            cout << "Score: " << score << endl;  // 점수 출력
            break;
        }
    }

    return 0;
}
