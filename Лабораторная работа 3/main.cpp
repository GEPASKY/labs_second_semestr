#include <iostream>
#include <fstream>
#include <string>
#include <bitset>
#include <vector>
#include <set>
#include <cmath>
#include <sstream>

using namespace std;

int controlBits = 0;

// Управляющие последовательности ANSI для цветов
#define ANSI_COLOR_RED     "\x1b[31m"
#define ANSI_COLOR_GREEN   "\x1b[32m"
#define ANSI_COLOR_YELLOW  "\x1b[33m"
#define ANSI_COLOR_BLUE    "\x1b[34m"
#define ANSI_COLOR_MAGENTA "\x1b[35m"
#define ANSI_COLOR_CYAN    "\x1b[36m"
#define ANSI_COLOR_RESET   "\x1b[0m"
static string controls = "";

std::string textToBinary(const std::string& text) {
    std::string binary;
    for (char c : text) {
        binary += std::bitset<8>(c).to_string();
    }
    return binary;
}
std::string binaryToText(const std::string& binary)
{
    std::string text;
    for (int i = 0; i < binary.length(); i += 8) {
        text += static_cast<char>(std::bitset<8>(binary.substr(i, 8)).to_ulong());
    }
    return text;
}

std::vector<std::string> splitBinary(const std::string& binary, int n) {
    std::vector<std::string> segments;
    for (size_t i = 0; i < binary.size(); i += n) {
        segments.push_back(binary.substr(i, n));
    }
    return segments;
}

string readBinaryFile(const string& filename = "input.txt") {
    ifstream file(filename, ios::binary);
    if (!file.is_open())
    {
        cerr << "Не удалось считать данные из файла: " << filename << endl;;
    }
    else
    {
        stringstream buffer;
        buffer << file.rdbuf();
        return buffer.str();
    }
    return string(8, '0');
}

bool writeBinaryFile(const string& binaryString, const string& filename = "output.bin")
{
    ofstream output(filename, ios::binary);
    if (output.is_open())
    {
        for (int unsigned i = 0; i < binaryString.length(); i += 8) {
            char c = static_cast<char>(bitset<8>(binaryString.substr(i, 8)).to_ulong());
            output.write(&c, 1);
        }
        return true;
    }
    return false;

}
string hammingCoding(const std::vector<std::string>& segments)
{
    bool controlBit = false;
    string hammingCode;
    for (const string& segment : segments)
    {
        string codingSegment = segment;
        bool bit = 0;
        for (auto c : codingSegment)
        {
            if (c == '1')
            {
                bit = !bit;
            }
        }
        controls += (bit ? '1' : '0');


        hammingCode += codingSegment;
    }
    return hammingCode + controls;
}
std::string hammingDecoding(const std::vector<std::string>& segments)
{
    string hammingDecodetText;
    int a = 0;
    for (const string& word : segments) {
        string codeWord = word;
        bool bit = 0;
        //bool preBit = (codeWord[codeWord.size()-1] == '0'? 0:1);
        bool preBit = (controls[a] == '0' ? 0 : 1);
        a++;
        //codeWord.erase(codeWord.length() - 1);
        for (auto c : codeWord)
        {
            if (c == '1')
            {
                bit = !bit;
            }
        }

        if (preBit != bit)
        {
            cout << ANSI_COLOR_RED << codeWord << ANSI_COLOR_RESET << endl;
        }
        else
        {
            cout << codeWord << endl;
        }

        hammingDecodetText += codeWord;
    }
    return hammingDecodetText;
}
int main() {
    setlocale(0, "rus");

    std::string inputPath = "input.txt";
    std::string outputPath;

    std::string inputText = readBinaryFile(inputPath);

    std::cout << "Входные данные из файла " << inputPath << ": \n" << inputText << std::endl;

    std::string binaryText = textToBinary(inputText);

    std::cout << "Входные данные из файла " << inputPath << " в бинарном виде: \n" << binaryText << std::endl;

    int n;
    std::cout << "Введите число бит для разбиения на сегменты: ";
    std::cin >> n;
    if (n <= 0)
    {
        std::cerr << "Невозвожно закодировать по 0 и меньше бит!\n";
        return 1;
    }

    std::vector<std::string> binarySegments = splitBinary(binaryText, n);


    std::cout << "Бинарные сегменты: \n";
    cout << ANSI_COLOR_CYAN;
    for (auto segment : binarySegments)
    {
        cout << segment << endl;
    }
    cout << ANSI_COLOR_RESET << endl;
    string hammingCode = hammingCoding(binarySegments);

    int encodedBitsSplit = n;


    std::vector<std::string> splittedHammingCode = splitBinary(hammingCode, encodedBitsSplit);
    cout << "Сегменты с контрольными битами (контрольные биты отмечены зелёным):\n";
    for (size_t i = 0; i < hammingCode.length(); i++)
    {
        if (i >= hammingCode.length() - controls.length())
        {
            cout << ANSI_COLOR_GREEN;
        }
        if ((i) % n == 0)
        {
            cout << endl;
        }
        cout << hammingCode[i];
        cout << ANSI_COLOR_RESET;
    }
    cout << endl;
    int outputZeros = 8 - hammingCode.length() % 8;
    cout << "Бинарная строка с контрольными битами:" << endl;
    if (outputZeros != 8)
    {
        cout << "(Желтым отмечены незначащие нули)" << endl;
        string zeros(outputZeros, '0');
        cout << "-";
        for (int i = 0; i < hammingCode.length(); i += 8)
        {
            cout << hammingCode.substr(i, 8) << "-";
        }
        hammingCode += zeros;
        cout << ANSI_COLOR_YELLOW;
        cout << zeros;
        cout << ANSI_COLOR_RESET << "-\n\n";;

    }
    else
    {
        cout << "-";
        for (int i = 0; i < hammingCode.length(); i += 8)
        {
            cout << hammingCode.substr(i, 8) << "-";
        }
        cout << endl << endl;

    }




    cout << "Введите путь для сохранения бинарного файла с контрольными битами."
            "\nК примеру \"C:\\data\\outputFile.bin\"\nИли оставьте это поле пустым(путь по умолчанию \"outpu.bin\")" << endl;
    cin.ignore();
    getline(cin, outputPath);
    if (outputPath.empty())
    {
        writeBinaryFile(hammingCode);
    }
    else
    {
        writeBinaryFile(hammingCode, outputPath);
    }
    cout << ANSI_COLOR_GREEN << "Файл сохранён" << ANSI_COLOR_RESET << endl;
    cout << "Измените бинарные данные, для проверки работоспособности кода\n";
    system("pause");

    string newHammingCode = textToBinary(readBinaryFile((outputPath.empty() ? "output.bin" : outputPath)));

    if (outputZeros != 8)
        newHammingCode.erase(newHammingCode.end() - outputZeros, newHammingCode.end());
    cout << controls << endl;
    controls = newHammingCode.substr(newHammingCode.length() - controls.length(), controls.length());
    newHammingCode.erase(newHammingCode.end() - controls.length(), newHammingCode.end());
    std::vector<std::string> newSplittedHammingCode = splitBinary(newHammingCode, n);
    cout << "Сегменты с контрольными битами (контрольные биты отмечены зелёным)\n(Красным отмеченны изменённые биты):\n";


    string decoded = hammingDecoding(newSplittedHammingCode);
    cout << "Декодированый текст:\n";
    cout << decoded << endl;
    cout << "Декодированый текст в char:\n";
    cout << binaryToText(decoded) << endl;





    return 0;
}