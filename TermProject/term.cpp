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
        return string(length, ' ');  // ���� ���ڷ� �̷���� �ܾ� ����
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
            word[rand() % length] = '0' + rand() % 10; // ���� ��ġ�� ���ڷ� ��ü
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
            word[rand() % length] = specialCharacters[rand() % specialCharacters.size()]; // ���� ��ġ�� Ư�����ڷ� ��ü
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
        wordCreator = new BasicWordCreator();  // ���ĺ��� ������� �ʴ� ��� BasicWordCreator ���
    }

    if (useNumber) {
        wordCreator = new NumberDecorator(wordCreator);
    }
    if (useSpecialCharacter) {
        wordCreator = new SpecialCharacterDecorator(wordCreator);
    }

    vector<string> current_words(10, "");  // 10���� �ܾ� ���
    vector<string> used_words;  // ���� �ܾ���� �����ϴ� ����
    int miss = 0;  // �� �̻� �̵��� �� ���� �ܾ��� ��
    int score = 0;  // ����

    // �� �ܾ� �߰�
    for (auto& word : current_words) {
        int space = rand() % 4 * 20;  // �ܼ�â�� x������ 4����Ͽ� ������ ��ġ�� ���ڿ� ���
        int length = rand() % 8 + 1;  // �ܾ��� ���̸� �����ϰ� ����
        word = string(space, ' ') + wordCreator->createWord(length);
    }

    string inputWord = "";
    while (true) {
        cout << "\033[2J\033[1;1H";  // ȭ�� �����

        for (const auto& word : current_words) {  // �ܾ� ��� ���
            cout << word << endl;
        }

        cout << "Enter a word (" << "\"exit\"" << " to quit): ";
        cin >> inputWord;
        cin.ignore();

        // exit �Է� �� ���� ����
        if (inputWord == "exit") {
            cout << "Game over!" << endl;
            cout << "Missed: " << miss << endl;  // �� �̻� �̵��� �� ���� �ܾ��� �� ���
            cout << "Score: " << score << endl;  // ���� ���
            break;
        }

        // �Է��� �ܾ ��Ͽ� �ִ��� Ȯ���ϰ�, �ִٸ� �ش� �ܾ �������� ���� �� �� �̵�
        for (auto it = current_words.begin(); it != current_words.end(); ++it) {
            size_t pos = it->find(inputWord);
            if (pos != string::npos) {
                it->replace(pos, inputWord.length(), string(inputWord.length(), ' '));  // �ܾ �������� ����
                used_words.push_back(inputWord);  // ���� �ܾ� �߰�
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
                int space = rand() % 4 * 10;  // �ܼ�â�� x������ 4����Ͽ� ������ ��ġ�� ���ڿ� ���
                int length = rand() % 8 + 1;  // �ܾ��� ���̸� �����ϰ� ����
                current_words.insert(current_words.begin(), string(space, ' ') + wordCreator->createWord(length));  // �� ���� �� �ܾ� �߰�

                break;  // �ܾ ã�Ƽ� �����ϰ� ���ο� �ܾ �߰������Ƿ� �� �̻� �ݺ��� �ʿ� ����
            }
        }
    }

    delete wordCreator;  // �޸� ����
    return 0;
}
