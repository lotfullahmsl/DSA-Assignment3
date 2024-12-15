#include <iostream>
#include <queue>
#include <unordered_map>
#include <string>
using namespace std;

// Node class for the Huffman Tree
class Node {
public:
    char data;
    int freq;
    Node* left;
    Node* right;

    Node(char data, int freq) {
        this->data = data;
        this->freq = freq;
        left = right = nullptr;
    }
};

// Compare class for the priority queue
class Compare {
public:
    bool operator()(Node* a, Node* b) {
        return a->freq > b->freq;
    }
};

// Function to build the frequency table
unordered_map<char, int> buildFrequencyTable(const string& str) {
    unordered_map<char, int> freqTable;
    for (char ch : str) {
        freqTable[ch]++;
    }
    return freqTable;
}

// Function to build the Huffman Tree
Node* buildHuffmanTree(unordered_map<char, int>& freqTable) {
    priority_queue<Node*, vector<Node*>, Compare> pq;

    for (auto pair : freqTable) {
        pq.push(new Node(pair.first, pair.second));
    }

    while (pq.size() > 1) {
        Node* left = pq.top(); pq.pop();
        Node* right = pq.top(); pq.pop();

        Node* newNode = new Node('\0', left->freq + right->freq);
        newNode->left = left;
        newNode->right = right;
        pq.push(newNode);
    }

    return pq.top();
}

// Function to generate Huffman codes
void generateHuffmanCodes(Node* root, string code, unordered_map<char, string>& huffmanCodes) {
    if (!root) return;

    if (!root->left && !root->right) {
        huffmanCodes[root->data] = code;
    }

    generateHuffmanCodes(root->left, code + "0", huffmanCodes);
    generateHuffmanCodes(root->right, code + "1", huffmanCodes);
}

// Function to encode the input string
string encodeString(const string& str, unordered_map<char, string>& huffmanCodes) {
    string encodedString = "";
    for (char ch : str) {
        encodedString += huffmanCodes[ch];
    }
    return encodedString;
}

// Function to decode the binary string
string decodeString(const string& encodedString, Node* root) {
    string decodedString = "";
    Node* current = root;
    for (char bit : encodedString) {
        if (bit == '0') {
            current = current->left;
        }
        else {
            current = current->right;
        }

        if (!current->left && !current->right) {
            decodedString += current->data;
            current = root;
        }
    }
    return decodedString;
}

// Menu-driven program
int main() {
    string input;
    cout << "==========================================" << endl;
    cout << "           HUFFMAN CODING PROGRAM         " << endl;
    cout << "==========================================" << endl;
    cout << "\nPlease enter the input string to encode:" << endl;
    cout << ">>> ";
    getline(cin, input);


    // Step 1: Build the frequency table
    unordered_map<char, int> freqTable = buildFrequencyTable(input);
    cout << "\n==========================================" << endl;
    cout << "               FREQUENCY TABLE            " << endl;
    cout << "==========================================" << endl;

    for (auto pair : freqTable) {
        cout << pair.first << ": " << pair.second << endl;
    }

    // Step 2: Build the Huffman Tree
    Node* root = buildHuffmanTree(freqTable);

    // Step 3: Generate Huffman codes
    unordered_map<char, string> huffmanCodes;
    generateHuffmanCodes(root, "", huffmanCodes);

    cout << "\n==========================================" << endl;
    cout << "                HUFFMAN CODES             " << endl;
    cout << "==========================================" << endl;

    for (auto pair : huffmanCodes) {
        cout << pair.first << ": " << pair.second << endl;
    }

    // Step 4: Encode the input string
    string encodedString = encodeString(input, huffmanCodes);
    cout << "==========================================" << endl;
    cout << "\nOriginal String: " << input << endl;
    cout << "==========================================" << endl;
    cout << "Encoded String: " << encodedString << endl;
    cout << "==========================================" << endl;

    // Step 5: Decode the encoded string
    string decodedString = decodeString(encodedString, root);
    cout << "Decoded String: " << decodedString << endl;
    cout << "==========================================" << endl;

    // Step 6: Analyze compression
    int originalSize = input.length() * 8; // 1 char = 8 bits
    int compressedSize = encodedString.length();
    cout << "==========================================" << endl;
    cout << "\nCompression Analysis:\n";
    cout << "==========================================" << endl;
    cout << "Original Size: " << originalSize << " bits\n";
    cout << "Compressed Size: " << compressedSize << " bits\n";

    if (decodedString == input) {
        cout << "\nThe decoded string matches the original string.\n";
    }
    else {
        cout << "\nError: Decoded string does not match the original string.\n";
    }

    return 0;
}