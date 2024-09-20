#include <iostream>
#include <fstream>
#include <map>
#include <queue>
#include <vector>
#include <bitset>
#include <sstream>
#include <string>

using namespace std;

struct Node
{
    string data;
    int freq;
    Node* left;// 0
    Node* right;// 0
    Node(const string& data, int freq) :
        data(data),
        freq(freq),
        left(nullptr),
        right(nullptr)
    {}
    Node(Node* left, Node* right):
        data(""),
        freq(left->freq + right->freq), left(left), right(right)
    {}
};
struct CompareNodes{
    bool operator()(Node* left, Node* right)
    {
        return left->freq > right->freq;
    }
};

string readBinaryFile(const string& inputPath)
{
    ifstream input(inputPath, ios::binary);
    if (input.is_open())
    {
        stringstream buffer;

        buffer << input.rdbuf();

        return buffer.str();
    }
    return "0";
}

std::string textToBinary(const std::string& text) {
    std::string binary;
    for (char c : text) {
        binary += std::bitset<8>(c).to_string();
    }
    return binary;
}

string binaryToString(const string& binary) {
    string text = "";
    for (size_t i = 0; i < binary.length(); i += 8)
    {
        unsigned char c = static_cast<unsigned char>(std::bitset<8>(binary.substr(i, 8)).to_ulong());
        text += c;
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

map<string, int> buildFrequencyTable(std::vector<std::string>& splittedBinary)
{
    map<string, int> frequencyTable;
    for (auto block : splittedBinary)
    {
        frequencyTable[block]++;
    }
    return frequencyTable;
}

Node* buildHaffmanTree(const map<string, int>& frequencyTable)
{
    priority_queue<Node*, vector<Node*>, CompareNodes> priorityQueue;
    for (auto& obj:frequencyTable)
    {
        priorityQueue.push(new Node(obj.first, obj.second));
    }
    if (priorityQueue.size() == 1)
    {
        Node* root = new Node("", priorityQueue.top()->freq);
        root->left = priorityQueue.top();
        return root;
    }
    else
    {
        while (priorityQueue.size() > 1)
        {
            Node* left = priorityQueue.top();
            priorityQueue.pop();

            Node* right = priorityQueue.top();
            priorityQueue.pop();

            Node* parent = new Node(left, right);

            priorityQueue.push(parent);
        }
        return priorityQueue.top();
    }
}

map<string, string> buildDict(Node* root, string code = "")
{
    map<string, string> Dict;
    if (root->left)
    {
        map<string, string> leftDict = buildDict(root->left, code+"0");
        Dict.insert(leftDict.begin(), leftDict.end());
    }
    if (root->right)   
    {
        map<string, string> rihgtDict = buildDict(root->right, code + "1");
        Dict.insert(rihgtDict.begin(), rihgtDict.end());
    }
    if (!root->data.empty())
    {
        Dict[root->data] = code;
    }
    return Dict;

}

string compress(map<string, string> dict, const vector<string>& splittedBinary)
{
    string compressedString = "";
    for (auto obj: splittedBinary)
    {
        compressedString += dict[obj];
    }
    return compressedString;
}

string decompress(Node* rootHaffman, const string& compressed)
{
    Node* currentNode = rootHaffman;
    string decompressed;
    for (size_t i = 0; i < compressed.length(); i++)
    {
        if (compressed[i] == '0')
        {
            currentNode = currentNode->left;
        }
        else
        {
            currentNode = currentNode->right;
        }
        if (!currentNode->data.empty())
        {
            decompressed += currentNode->data;
            currentNode = rootHaffman;
        }
    }
    return decompressed;
}

void printTree(Node* root, int level = 0)
{
    if (root == nullptr)
    {
        return;
    }
    printTree(root->right, level + 1);
    for (size_t i = 0; i < level; i++)
    {
        cout << "\t ";
    }
    if (root->data.empty())
    {
        cout << '(' << root->freq << ')';
    }
    else
    {
        cout << '[' << root->data << ']';
    }
    cout << '|' << level << '|' << endl;
    printTree(root->left, level + 1);
}



int main()
{
    setlocale(0, "rus");
    string inputPath = "input.txt";
    string inputFile = readBinaryFile(inputPath);//Считываем файл(1)
    cout << "Input file:\n" << inputFile << endl;

    string binaryFile = textToBinary(inputFile);//Приводим файл к бинарному виду(2)

    
    int bitsSplit;
    
    do{
        cout << "Введите количество бит для разбиения на сегменты: ";
        cin >> bitsSplit;
    } while (bitsSplit < 0);
    
    vector<std::string> splittedBinary = splitBinary(binaryFile, bitsSplit);//Разбиваем строку по n бит(3)
    
    map<string, int> frequencyTable = buildFrequencyTable(splittedBinary);//Формируем таблицу частотности(4)

    Node* rootHaffman = buildHaffmanTree(frequencyTable);//Формируем дерево(5)
    
    cout << "Таблица частотности: \n";

    for (auto cail : frequencyTable)
    {
        cout << cail.first << "\t" << cail.second << endl;
    }
    
    map<string, string> dict = buildDict(rootHaffman);//Формируем словарь(6)

    cout << "Словарик: \n";
    for (auto cail : dict)
    {
        cout << cail.first << "\t\t" << cail.second << endl;
    }
    cout << "Дерево: \n";

    printTree(rootHaffman);



    string compressed = compress(dict, splittedBinary);

    cout << compressed << endl;
        

    int zeros = 0;
    if (compressed.length() % 8 != 0)
    {
        zeros = 8 - (compressed.length() % 8);
        compressed += string(zeros, '0');
    }
    string compressedBinString = binaryToString(compressed);
    ofstream output("output.bin", ios::binary);
    
    output.write(compressedBinString.c_str(), compressedBinString.length());
    output.close();
    string compressedFile = readBinaryFile("output.bin");

    compressedFile = textToBinary(compressedFile);
    if (zeros != 0)
    {
        compressedFile.erase(compressedFile.end() - zeros, compressedFile.end());
    }

    string decompressed = decompress(rootHaffman, compressedFile);
    cout << binaryToString(decompressed) << endl;
}