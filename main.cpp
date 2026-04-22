#include <iostream>
#include <fstream>
#include <queue>
#include <map>
#include <vector>
using namespace std;

// Node structure
struct Node {
    char ch;
    int freq;
    Node *left, *right;

    Node(char c, int f) {
        ch = c;
        freq = f;
        left = right = nullptr;
    }
};

// Min heap comparator
struct compare {
    bool operator()(Node* l, Node* r) {
        return l->freq > r->freq;
    }
};

// store codes
map<char, string> huffmanCode;

// generate codes
void generateCode(Node* root, string str) {
    if (!root) return;

    // leaf node
    if (!root->left && !root->right) {
        huffmanCode[root->ch] = str;
    }

    generateCode(root->left, str + "0");
    generateCode(root->right, str + "1");
}

// build tree
Node* buildHuffmanTree(map<char, int> freq) {
    priority_queue<Node*, vector<Node*>, compare> pq;

    for (auto p : freq) {
        pq.push(new Node(p.first, p.second));
    }

    while (pq.size() > 1) {
        Node* left = pq.top(); pq.pop();
        Node* right = pq.top(); pq.pop();

        Node* newNode = new Node('$', left->freq + right->freq);
        newNode->left = left;
        newNode->right = right;

        pq.push(newNode);
    }

    return pq.top();
}

int main() {

    // 1. Read file
    ifstream in("input.txt");

    if (!in) {
        cout << "Error opening file!" << endl;
        return 1;
    }

    string data((istreambuf_iterator<char>(in)), istreambuf_iterator<char>());
    in.close();

    cout << "File Content:\n" << data << endl;

    // 2. Frequency table
    map<char, int> freq;

    for (char ch : data) {
        freq[ch]++;
    }

    cout << "\nFrequency Table:\n";
    for (auto p : freq) {
        cout << p.first << " : " << p.second << endl;
    }

    // 3. Build tree
    Node* root = buildHuffmanTree(freq);

    // 4. Generate codes
    generateCode(root, "");

    cout << "\nHuffman Codes:\n";
    for (auto p : huffmanCode) {
        cout << p.first << " : " << p.second << endl;
    }

    // 5. Encode data
    string encoded = "";
    for (char ch : data) {
        encoded += huffmanCode[ch];
    }

    cout << "\nEncoded Data:\n" << encoded << endl;

    // 6. REAL COMPRESSION (BINARY WRITE)
    ofstream out("compressed.bin", ios::binary);

    unsigned char byte = 0;
    int count = 0;

    for (char bit : encoded) {
        byte = byte << 1;

        if (bit == '1')
            byte = byte | 1;

        count++;

        if (count == 8) {
            out.write((char*)&byte, 1);
            count = 0;
            byte = 0;
        }
    }

    // remaining bits
    if (count > 0) {
        byte = byte << (8 - count);
        out.write((char*)&byte, 1);
    }

    out.close();

    cout << "\nCompressed file created: compressed.bin\n";

    return 0;
}
