#include <iostream>
#include <string>
#include <vector>
#include <cstdlib>
#include <ctime>
#include <thread>
#include <chrono>
#include <mutex>
#include <random>

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

vector<string> current_words(10, "");  // 10줄의 단어 목록
mutex mtx;  // 벡터를 보호하기 위한 뮤텍스

void drop(WordCreator* wordCreator, vector<string>& current_words) {
    random_device rd;
    mt19937 gen(rd());
    std::uniform_int_distribution<> distr(500, 2000); // 0.5초에서 2초 사이의 랜덤한 시간을 생성

    while (true) {
        this_thread::sleep_for(chrono::milliseconds(distr(gen)));  // 랜덤한 시간 동안 대기
        mtx.lock();
        // 맨 아래 줄부터 문자열을 하나씩 위로 올림
        for (int i = current_words.size() - 1; i > 0; i--) {
            current_words[i] = current_words[i - 1];
        }

        // 맨 위 줄에 새로운 문자열 추가
        int space = rand() % 4 * 20;  // 콘솔창을 x축으로 4등분하여 랜덤한 위치에 문자열 출력
        int length = rand() % 8 + 1;  // 단어의 길이를 랜덤하게 생성
        current_words[0] = string(space, ' ') + wordCreator->createWord(length);
        mtx.unlock();
    }
}

bool isInputting = false;  // 사용자의 입력 상태를 나타내는 전역 변수

void printWords(const vector<string>& current_words) {
    while (true) {
        if (!isInputting)
        {
            system("cls");  // 화면 지우기
            mtx.lock();  // 뮤텍스 잠그기
            for (const auto& word : current_words) {  // 단어 목록 출력
                cout << word << endl;
            }
            cout << "Enter a word (" << "\"exit\"" << " to quit): ";  // 입력 프롬프트 출력
            mtx.unlock();  // 뮤텍스 풀기
        }
        
        this_thread::sleep_for(chrono::milliseconds(500));  // 0.5초 대기
    }
}

void userInput(vector<string>& current_words, int& miss, int& score, const string& exitWord, WordCreator* wordCreator) {
    string inputWord;
    while (true) {
        //cout << "Enter a word (" << "\"exit\"" << " to quit): ";
        cin >> inputWord;
        cin.ignore();

        // exit 입력 시 게임 종료
        if (inputWord == exitWord) {
            cout << "Game over!" << endl;
            cout << "Missed: " << miss << endl;  // 더 이상 이동할 수 없는 단어의 수 출력
            cout << "Score: " << score << endl;  // 점수 출력
            exit(0);
        }

        // 입력한 단어가 목록에 있는지 확인하고, 있다면 해당 단어를 공백으로 변경
        mtx.lock();  // 뮤텍스 잠그기
        if (!current_words.back().empty() && current_words.back() != inputWord) {  // 마지막 줄의 단어가 비어있지 않고, 입력한 단어와 다른 경우
            miss++;  // miss 증가
        }
        for (auto it = current_words.begin(); it != current_words.end(); ++it) {
            size_t pos = it->find(inputWord);
            if (pos != string::npos) {
                it->replace(pos, inputWord.length(), string(inputWord.length(), ' '));  // 단어를 공백으로 변경
                score += 10;  // 점수 증가

                // 맨 아래 줄부터 문자열을 하나씩 위로 올림
                for (int i = current_words.size() - 1; i > 0; i--) {
                    current_words[i] = current_words[i - 1];
                }

                // 맨 위 줄에 새로운 문자열 추가
                int length = rand() % 8 + 1;  // 단어의 길이를 랜덤하게 생성
                current_words[0] = string(rand() % 4 * 20, ' ') + wordCreator->createWord(length);
                break;  // 단어를 찾아서 변경하고 새로운 단어를 추가했으므로 더 이상 반복할 필요 없음
            }
        }
        mtx.unlock();  // 뮤텍스 풀기
    }
}

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

    vector<string> used_words;  // 사용된 단어들을 저장하는 벡터
    int miss = 0;  // 더 이상 이동할 수 없는 단어의 수
    int score = 0;  // 점수

    thread dropThread(drop, wordCreator, ref(current_words));  // 문자열 하강 스레드 시작
    dropThread.detach();

    thread printThread(printWords, ref(current_words));  // 화면 출력 스레드 시작
    printThread.detach();

    thread userInputThread(userInput, ref(current_words), ref(miss), ref(score), "exit", wordCreator);  // 사용자 입력 스레드 시작
    userInputThread.join();

    delete wordCreator;  // 메모리 해제
    return 0;
}
