#include <iostream>
#include <fstream>
#include <string>
#include <map>
#include <bitset>
#include <vector>
#include <set>
#include <cmath>
// #include <Windows.h>

// Управляющие последовательности ANSI для цветов
#define ANSI_COLOR_RED     "\x1b[31m"
#define ANSI_COLOR_GREEN   "\x1b[32m"
#define ANSI_COLOR_YELLOW  "\x1b[33m"
#define ANSI_COLOR_BLUE    "\x1b[34m"
#define ANSI_COLOR_MAGENTA "\x1b[35m"
#define ANSI_COLOR_CYAN    "\x1b[36m"
#define ANSI_COLOR_RESET   "\x1b[0m"


// Функция для вычисления энтропии
double CalculateEntropy(const std::string& segment) {
    std::map<char, int> charCount;

    // Подсчет частоты встречаемости каждого символа
    for (char c : segment) {
        charCount[c]++;
    }

    double entropy = 0.0;
    int segmentSize = segment.size();

    // Рассчет вероятностей и энтропии
    for (const auto& pair : charCount) {
        double probability = static_cast<double>(pair.second) / segmentSize;
        entropy -= probability * log2(probability);
    }

    return entropy;
}

std::string textToBinary(const std::string& text) {
    std::string binary;
    for (char c : text) {
        binary += std::bitset<8>(c).to_string();
    }
    return binary;
}

std::vector<std::string> splitBinary(const std::string& binary, int n) {
    std::vector<std::string> segments;
    for (size_t i = 0; i < binary.size(); i += n) {
        segments.push_back(binary.substr(i, n));
    }
    return segments;
}

int numberOfUniqueSegments(std::vector<std::string>& binarySegments)
{
    std::set<std::string> Unique;
    for (std::string& segment : binarySegments) {
        Unique.insert(segment);
    }
    return (ceil(log2(Unique.size())));
}
int main() {
    //SetConsoleOutputCP(CP_UTF8);
    setlocale(0, "rus");

    //Пути
    std::string inputPath = "input.txt";

    std::ifstream inputFile(inputPath);
    if (!inputFile) {
        std::cerr << "Ошибка открытия файла input.txt" << std::endl;
        return 1;
    }

    std::string inputText;
    std::string line;
    while (std::getline(inputFile, line)) {
        inputText += line;
    }
    inputFile.close();
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
    int N;
    int NN = numberOfUniqueSegments(binarySegments);
    std::cout << "Введите число бит для выходного словаря: ";
    std::cin >> N;
    std::map<std::string, std::string> codeMap;
    if (N <= 0||N<NN)
    {
        std::cout << "Невозможно закодировать по " << N << " бит\n";
        return 1;
    }

    std::string currentCode;
    std::set<std::string> codes;
    for (int i = 0; i < N; i++) {
        int r = std::rand() % 2;
        currentCode += char(r + '0');

    }
    int zeros = 0;

    std::cout << "Словарь:\n";
    for (std::string& segment : binarySegments) {

        while (segment.size() < n) {
            segment += '0';
            zeros++;
        }
        if (codeMap.find(segment) == codeMap.end()) {
            codeMap[segment] = currentCode;
            codes.insert(currentCode);
            std::string newCurrentCode = "";
            while (codes.find(newCurrentCode) != codes.end() || newCurrentCode == "")
            {
                newCurrentCode.clear();
                for (int i = 0; i < N; i++) {
                    int r = std::rand() % 2;
                    newCurrentCode += char(r + '0');

                }
            }
            currentCode = newCurrentCode;

        }

        std::cout << ANSI_COLOR_CYAN << "Out:" << codeMap[segment] << '\t' << ANSI_COLOR_BLUE << "In:" << segment << std::endl;
        double entropy = CalculateEntropy(segment);
        //std::cout << "Энтропия: " << entropy << std::endl;
    }
    std::cout << ANSI_COLOR_RESET;
    std::cout << "Количество элементов словаря: "<< codeMap.size()<< std::endl;
    
    std::string encodedText;
    for (const std::string& segment : binarySegments) {
        encodedText += codeMap[segment];
    }
    std::cout << ANSI_COLOR_RESET;

    std::ofstream encodedFile("encoded.bin", std::ios::binary);
    int outputZeros = 0;
    if (encodedText.length()%8!=0)
    {
        for (size_t j = 0; encodedText.length() % 8 !=0; j++)
        {
            encodedText += '0';
            outputZeros++;
        }
    }
    std::cout << "\nЗакодированный текст: " << std::endl << encodedText << std::endl;
    for (size_t i = 0; i < encodedText.length(); i += 8) {
        
        std::bitset<8> byte(encodedText.substr(i, 8));
        char byteChar = static_cast<char>(byte.to_ulong());
        encodedFile.write(&byteChar, 1);

    }
    encodedFile.close();

    std::ifstream decodedInputFile("encoded.bin", std::ios::binary);
    if (!decodedInputFile) {
        std::cerr << "Ошибка открытия файла encoded.bin" << std::endl;
        return 1;
    }

    std::string decodedByteText;
    char byteChar;
    std::string binaryByte;
    std::cout << "\nСчитанные данные из закодированного файла:\n";
    while (decodedInputFile.read(&byteChar, 1)) {
        binaryByte = std::bitset<8>(byteChar).to_string();
        std::cout << ANSI_COLOR_MAGENTA << std::bitset<8>(byteChar).to_string() << std::endl;
        decodedByteText += binaryByte;
    }
    std::cout << ANSI_COLOR_RESET;
    decodedInputFile.close();

    std::string decodedOutputText;
    if (outputZeros!=0)
    {
        decodedByteText.erase(decodedByteText.length()-outputZeros);
    }
    for (size_t i = 0; i < decodedByteText.length(); i += N) {
        for (auto pair: codeMap)
        {
            if (pair.second == decodedByteText.substr(i, N))
            {
                decodedOutputText += pair.first;
                break;
            }
            
        }
    }
    if (zeros != 0)
    {
        decodedOutputText.erase(decodedOutputText.length() - zeros);
    }
    std::vector<unsigned char> decodedBytes;
    std::cout << "\nДедодированная bin последовательность:\n" << decodedOutputText << std::endl;
    std::cout << ANSI_COLOR_RESET << std::endl;
    for (size_t i = 0; i < decodedOutputText.size(); i += 8) {

        std::string byteStr = decodedOutputText.substr(i, 8);
        unsigned char byte = static_cast<unsigned char>(std::bitset<8>(byteStr).to_ulong());
        decodedBytes.push_back(byte);
    }

    std::string decodedText(decodedBytes.begin(), decodedBytes.end());

    std::ofstream decodedFile("decoded.txt");

    decodedFile << decodedText;
    std::cout << "\nДедодированная char последовательность:\n" << ANSI_COLOR_GREEN << decodedText << std::endl;
    decodedFile.close();

    std::cout << "Кодирование и декодирование завершено." <<ANSI_COLOR_RESET <<  std::endl;

    return 0;
}
