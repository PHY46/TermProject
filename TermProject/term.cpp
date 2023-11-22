#include <iostream>
#include <string>
#include <vector>
#include <cstdlib>
#include <ctime>

using namespace std;

class WordCreator {
public:
    virtual string createWord(int length) = 0;
};

class BasicWordCreator : public WordCreator {
public:
    string createWord(int length) override {
        return string(length, ' ');  // 공백 문자로 이루어진 단어 생성
    }
};

class AlphabetWordCreator : public WordCreator {
public:
    string createWord(int length) override {
        string word = "";
        for (int i = 0; i < length; i++) {
            word += 'a' + rand() % 26;
        }
        return word;
    }
};

class NumberDecorator : public WordCreator {
    WordCreator* wordCreator;
public:
    NumberDecorator(WordCreator* wordCreator) : wordCreator(wordCreator) {}
    string createWord(int length) override {
        string word = wordCreator->createWord(length);
        if (length > 1) {
            word[rand() % length] = '0' + rand() % 10; // 랜덤 위치를 숫자로 대체
        }
        else {
            word = '0' + rand() % 10;
        }
        return word;
    }
};

class SpecialCharacterDecorator : public WordCreator {
    WordCreator* wordCreator;
public:
    SpecialCharacterDecorator(WordCreator* wordCreator) : wordCreator(wordCreator) {}
    string createWord(int length) override {
        string specialCharacters = "!@#$%^&*()";
        string word = wordCreator->createWord(length);
        if (length > 1) {
            word[rand() % length] = specialCharacters[rand() % specialCharacters.size()]; // 랜덤 위치를 특수문자로 대체
        }
        else {
            word = specialCharacters[rand() % specialCharacters.size()];
        }
        return word;
    }
};

int main() {
    srand(static_cast<unsigned int>(time(0)));
    bool useAlphabet, useNumber, useSpecialCharacter;

    cout << "알파벳을 사용하시겠습니까? (yes/no): ";
    string input;
    cin >> input;
    useAlphabet = (input == "yes");

    cout << "숫자를 사용하시겠습니까? (yes/no): ";
    cin >> input;
    useNumber = (input == "yes");

    cout << "특수문자를 사용하시겠습니까? (yes/no): ";
    cin >> input;
    useSpecialCharacter = (input == "yes");

    WordCreator* wordCreator;
    if (useAlphabet) {
        wordCreator = new AlphabetWordCreator();
    }
    else {
        wordCreator = new BasicWordCreator();  // 알파벳을 사용하지 않는 경우 BasicWordCreator 사용
    }

    if (useNumber) {
        wordCreator = new NumberDecorator(wordCreator);
    }
    if (useSpecialCharacter) {
        wordCreator = new SpecialCharacterDecorator(wordCreator);
    }

    vector<string> current_words(10, "");  // 10줄의 단어 목록
    vector<string> used_words;  // 사용된 단어들을 저장하는 벡터
    int miss = 0;  // 더 이상 이동할 수 없는 단어의 수
    int score = 0;  // 점수

    // 새 단어 추가
    for (auto& word : current_words) {
        int space = rand() % 4 * 20;  // 콘솔창을 x축으로 4등분하여 랜덤한 위치에 문자열 출력
        int length = rand() % 8 + 1;  // 단어의 길이를 랜덤하게 생성
        word = string(space, ' ') + wordCreator->createWord(length);
    }

    string inputWord = "";
    while (true) {
        cout << "\033[2J\033[1;1H";  // 화면 지우기

        for (const auto& word : current_words) {  // 단어 목록 출력
            cout << word << endl;
        }

        cout << "Enter a word (" << "\"exit\"" << " to quit): ";
        cin >> inputWord;
        cin.ignore();

        // exit 입력 시 게임 종료
        if (inputWord == "exit") {
            cout << "Game over!" << endl;
            cout << "Missed: " << miss << endl;  // 더 이상 이동할 수 없는 단어의 수 출력
            cout << "Score: " << score << endl;  // 점수 출력
            break;
        }

        // 입력한 단어가 목록에 있는지 확인하고, 있다면 해당 단어를 공백으로 변경 후 줄 이동
        for (auto it = current_words.begin(); it != current_words.end(); ++it) {
            size_t pos = it->find(inputWord);
            if (pos != string::npos) {
                it->replace(pos, inputWord.length(), string(inputWord.length(), ' '));  // 단어를 공백으로 변경
                used_words.push_back(inputWord);  // 사용된 단어 추가
                score += 10;  // 점수 증가

                // 맨 아래 단어가 아니면 맨 아래 단어를 벡터에서 제거하고, 그 위에 있는 단어들을 한 줄씩 아래로 내려오게 하고, miss 증가
                if (it != current_words.end() - 1) {
                    current_words.erase(current_words.end() - 1);
                    miss++;

                    // 줄 이동
                    for (auto iter = it; iter != current_words.begin(); --iter) {
                        iter->swap(*(iter - 1));
                    }
                }
                else {
                    // 맨 아래 단어를 벡터에서 제거
                    current_words.erase(it);
                }

                // 새 단어 추가
                int space = rand() % 4 * 10;  // 콘솔창을 x축으로 4등분하여 랜덤한 위치에 문자열 출력
                int length = rand() % 8 + 1;  // 단어의 길이를 랜덤하게 생성
                current_words.insert(current_words.begin(), string(space, ' ') + wordCreator->createWord(length));  // 맨 위에 새 단어 추가

                break;  // 단어를 찾아서 변경하고 새로운 단어를 추가했으므로 더 이상 반복할 필요 없음
            }
        }
    }

    delete wordCreator;  // 메모리 해제
    return 0;
}
