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

//���� ���ڷ� �̷���� �ܾ� ����
class BasicWordCreator : public WordCreator {
public:
    string createWord(int length) override {
        return string(length, ' ');
    }
};

//���ĺ� �ܾ� ����
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

//���� �ܾ� ����
class NumberDecorator : public WordCreator {
    WordCreator* wordCreator;
public:
    NumberDecorator(WordCreator* wordCreator) : wordCreator(wordCreator) {}
    string createWord(int length) override {
        string word = wordCreator->createWord(length);
        if (length > 1) {
            word[rand() % length] = '0' + rand() % 10;
        }
        else {
            word = '0' + rand() % 10;
        }
        return word;
    }
};

//Ư������ �ܾ� ����
class SpecialCharacterDecorator : public WordCreator {
    WordCreator* wordCreator;
public:
    SpecialCharacterDecorator(WordCreator* wordCreator) : wordCreator(wordCreator) {}
    string createWord(int length) override {
        string specialCharacters = "!@#$%^&*()";
        string word = wordCreator->createWord(length);
        if (length > 1) {
            word[rand() % length] = specialCharacters[rand() % specialCharacters.size()];
        }
        else {
            word = specialCharacters[rand() % specialCharacters.size()];
        }
        return word;
    }
};

vector<string> current_words(10, "");  //10���� �ܾ� ���
mutex mtx;  //���͸� ��ȣ�ϱ� ���� ���ؽ�

void drop(WordCreator* wordCreator, vector<string>& current_words, int& miss) {
    random_device rd;
    mt19937 gen(rd());
    std::uniform_int_distribution<> distr(1000, 2000); //0.5�ʿ��� 2�� ������ ������ �ð��� ����

    while (true) {
        this_thread::sleep_for(chrono::milliseconds(distr(gen)));  //������ �ð� ���� ���
        mtx.lock();
        if (!current_words.back().empty()) {
            miss++; //�ܾ ������ �����Դٸ� miss ����
        }
        //�� �Ʒ� �ٺ��� ���ڿ��� �ϳ��� ���� �ø�
        for (int i = current_words.size() - 1; i > 0; i--) {
            current_words[i] = current_words[i - 1];
        }

        //�� �� �ٿ� ���ο� ���ڿ� �߰�
        int space = rand() % 4 * 20;  //�ܼ�â�� x������ 4���
        int length = rand() % 8 + 1;  //�ܾ��� ���̸� 8���� ���ܷ� �����ϰ� ����
        current_words[0] = string(space, ' ') + wordCreator->createWord(length);
        mtx.unlock();
    }
}

bool isInputting = false;  //������� �Է� ���¸� ��Ÿ���� ���� ����

void printWords(const vector<string>& current_words) {
    while (true) {
        this_thread::sleep_for(chrono::milliseconds(500));  //0.5�� ���
        if (isInputting) continue;  //�Է� ���̸� ȭ�� ���� ����

        system("cls");  //ȭ�� �����
        mtx.lock();  //���ؽ� ��ױ�
        for (const auto& word : current_words) {  //�ܾ� ��� ���
            cout << word << endl;
        }
        cout << "Enter a word (" << "\"exit\"" << " to quit): ";  //�Է� ������Ʈ ���
        mtx.unlock();  //���ؽ� Ǯ��
    }
}

void userInput(vector<string>& current_words, int& miss, int& score, const string& exitWord, WordCreator* wordCreator) {
    string inputWord;
    while (true) {
        cin >> inputWord;

        if (inputWord == exitWord) { //exit �Է� �� ���� ����
            cout << "Game over!" << endl;
            cout << "Missed: " << miss << endl;  //��ģ �ܾ��� �� ���
            cout << "Score: " << score << endl;  //���� ���
            exit(0);
        }

        mtx.lock();  //���ؽ� ��ױ�
        for (auto it = current_words.begin(); it != current_words.end(); ++it) {
            size_t pos = it->find(inputWord);
            if (pos != string::npos) { //�Է��� �ܾ ��Ͽ� �ִ��� Ȯ��
                it->replace(pos, inputWord.length(), string(inputWord.length(), ' '));  //�ܾ �������� ����
                score += 10;  //���� ����

                //�� �Ʒ� �ٺ��� ���ڿ��� �ϳ��� ���� �ø�
                for (int i = current_words.size() - 1; i > 0; i--) {
                    current_words[i] = current_words[i - 1];
                }

                //�� �� �ٿ� ���ο� ���ڿ� �߰�
                int length = rand() % 8 + 1;  //�ܾ��� ���̸� �����ϰ� ����
                current_words[0] = string(rand() % 4 * 20, ' ') + wordCreator->createWord(length);
                break;
            }
        }
        mtx.unlock();  //���ؽ� Ǯ��
    }
}

int main() {
    srand(static_cast<unsigned int>(time(0)));
    bool useAlphabet, useNumber, useSpecialCharacter;

    cout << "���ĺ��� ����Ͻðڽ��ϱ�? (yes/no): ";
    string input;
    cin >> input;
    useAlphabet = (input == "yes");

    cout << "���ڸ� ����Ͻðڽ��ϱ�? (yes/no): ";
    cin >> input;
    useNumber = (input == "yes");

    cout << "Ư�����ڸ� ����Ͻðڽ��ϱ�? (yes/no): ";
    cin >> input;
    useSpecialCharacter = (input == "yes");

    WordCreator* wordCreator;
    if (useAlphabet) {
        wordCreator = new AlphabetWordCreator();
    }
    else {
        wordCreator = new BasicWordCreator();
    }

    if (useNumber) {
        wordCreator = new NumberDecorator(wordCreator);
    }
    if (useSpecialCharacter) {
        wordCreator = new SpecialCharacterDecorator(wordCreator);
    }

    vector<string> used_words;  //���� �ܾ���� �����ϴ� ����
    int miss = 0;  //��ģ �ܾ��� ���� ��Ÿ���� ����
    int score = 0;  //������ ��Ÿ���� ����

    thread dropThread(drop, wordCreator, ref(current_words), ref(miss));
    dropThread.detach();

    thread printThread(printWords, ref(current_words));  //ȭ�� ��� ������ ����
    printThread.detach();

    thread userInputThread(userInput, ref(current_words), ref(miss), ref(score), "exit", wordCreator);  //����� �Է� ������ ����
    userInputThread.join();

    delete wordCreator;  //�޸� ����
    return 0;
}
