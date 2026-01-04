//Hanya demo huffman tree saja

#ifndef HUFFMAN_TREE_DYNAMIC_H
#define HUFFMAN_TREE_DYNAMIC_H

#include <iostream>

using namespace std;

class HuffmanTreeDynamic {
private:
    struct HuffmanNode {
        char karakter;
        int frekuensi;
        int indeksKiri;
        int indeksKanan;
        int indeksParent;
        string kode;
        
        HuffmanNode() : karakter('\0'), frekuensi(0), 
                       indeksKiri(-1), indeksKanan(-1), 
                       indeksParent(-1), kode("") {}
        
        HuffmanNode(char ch, int freq) : karakter(ch), frekuensi(freq),
                                        indeksKiri(-1), indeksKanan(-1),
                                        indeksParent(-1), kode("") {}
    };
    
    HuffmanNode* nodes;
    int kapasitas;
    int jumlahNode;
    int rootIndex;
    
    void resizeArray() {
        int newCapacity = kapasitas * 2;
        HuffmanNode* newNodes = new HuffmanNode[newCapacity];
        for (int i = 0; i < jumlahNode; i++) {
            newNodes[i] = nodes[i];
        }
        delete[] nodes;
        nodes = newNodes;
        kapasitas = newCapacity;
    }
    
    int addNode(const HuffmanNode& node) {
        if (jumlahNode >= kapasitas) {
            resizeArray();
        }
        nodes[jumlahNode] = node;
        return jumlahNode++;
    }
    
    void hitungFrekuensi(const string& teks, int* freq) {
        for (int i = 0; i < 256; i++) freq[i] = 0;
        for (size_t i = 0; i < teks.length(); i++) {
            freq[(unsigned char)teks[i]]++;
        }
    }
    
    void buatKode(int nodeIndex, string kode) {
        if (nodeIndex == -1) return;
        
        nodes[nodeIndex].kode = kode;
        buatKode(nodes[nodeIndex].indeksKiri, kode + "0");
        buatKode(nodes[nodeIndex].indeksKanan, kode + "1");
    }

public:
    HuffmanTreeDynamic() : nodes(nullptr), kapasitas(10), 
                          jumlahNode(0), rootIndex(-1) {
        nodes = new HuffmanNode[kapasitas];
    }
    
    ~HuffmanTreeDynamic() {
        if (nodes) delete[] nodes;
    }
    
    string kompres(const string& teks) {
        int freq[256];
        hitungFrekuensi(teks, freq);
        
        for (int i = 0; i < 256; i++) {
            if (freq[i] > 0) {
                HuffmanNode node((char)i, freq[i]);
                addNode(node);
            }
        }
        
        int* minHeap = new int[jumlahNode];
        for (int i = 0; i < jumlahNode; i++) {
            minHeap[i] = i;
        }
        
        int heapSize = jumlahNode;
        while (heapSize > 1) {
            for (int i = 0; i < heapSize - 1; i++) {
                for (int j = 0; j < heapSize - i - 1; j++) {
                    if (nodes[minHeap[j]].frekuensi > nodes[minHeap[j + 1]].frekuensi) {
                        int temp = minHeap[j];
                        minHeap[j] = minHeap[j + 1];
                        minHeap[j + 1] = temp;
                    }
                }
            }
            
            HuffmanNode parent;
            parent.indeksKiri = minHeap[0];
            parent.indeksKanan = minHeap[1];
            parent.frekuensi = nodes[minHeap[0]].frekuensi + nodes[minHeap[1]].frekuensi;
            
            int parentIndex = addNode(parent);
            nodes[parentIndex].indeksKiri = minHeap[0];
            nodes[parentIndex].indeksKanan = minHeap[1];
            nodes[minHeap[0]].indeksParent = parentIndex;
            nodes[minHeap[1]].indeksParent = parentIndex;
            
            minHeap[0] = parentIndex;
            for (int i = 2; i < heapSize; i++) {
                minHeap[i - 1] = minHeap[i];
            }
            heapSize--;
        }
        
        rootIndex = minHeap[0];
        delete[] minHeap;
        
        buatKode(rootIndex, "");
        
        string hasil;
        for (size_t i = 0; i < teks.length(); i++) {
            for (int j = 0; j < jumlahNode; j++) {
                if (nodes[j].karakter == teks[i] && nodes[j].indeksKiri == -1) {
                    hasil += nodes[j].kode;
                    break;
                }
            }
        }
        
        return hasil;
    }
    
    string dekompres(const string& teksTerkompresi) {
        string hasil;
        int current = rootIndex;
        
        for (size_t i = 0; i < teksTerkompresi.length(); i++) {
            if (teksTerkompresi[i] == '0') {
                current = nodes[current].indeksKiri;
            } else {
                current = nodes[current].indeksKanan;
            }
            
            if (nodes[current].indeksKiri == -1) {
                hasil += nodes[current].karakter;
                current = rootIndex;
            }
        }
        
        return hasil;
    }
    
    void tampilkanKode() {
        for (int i = 0; i < jumlahNode; i++) {
            if (nodes[i].indeksKiri == -1) {
                cout << "'" << nodes[i].karakter << "': " << nodes[i].kode << endl;
            }
        }
    }
};


#endif
