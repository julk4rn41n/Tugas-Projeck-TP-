#ifndef AVL_TREE_DYNAMIC_H
#define AVL_TREE_DYNAMIC_H

#include <iostream>

using namespace std;

template <typename T>
class AVLTreeDynamic {
private:
    struct AVLNode {
        T data;
        int tinggi;
        int indeksKiri;
        int indeksKanan;
        bool terhapus;
        
        AVLNode() : tinggi(1), indeksKiri(-1), indeksKanan(-1), terhapus(false) {}
        AVLNode(const T& val) : data(val), tinggi(1), 
                               indeksKiri(-1), indeksKanan(-1), terhapus(false) {}
    };
    
    AVLNode* nodes;
    int kapasitas;
    int jumlahNode;
    int rootIndex;
    int* freeIndices;
    int freeCount;
    int freeCapacity;
    
    void resizeArray() {
        int newCapacity = kapasitas * 2;
        AVLNode* newNodes = new AVLNode[newCapacity];
        for (int i = 0; i < jumlahNode; i++) {
            newNodes[i] = nodes[i];
        }
        delete[] nodes;
        nodes = newNodes;
        kapasitas = newCapacity;
    }
    
    void resizeFreeList() {
        int newCapacity = freeCapacity * 2;
        int* newFree = new int[newCapacity];
        for (int i = 0; i < freeCount; i++) {
            newFree[i] = freeIndices[i];
        }
        delete[] freeIndices;
        freeIndices = newFree;
        freeCapacity = newCapacity;
    }
    
    int getFreeIndex() {
        if (freeCount > 0) {
            return freeIndices[--freeCount];
        }
        if (jumlahNode >= kapasitas) {
            resizeArray();
        }
        return jumlahNode++;
    }
    
    void addToFreeList(int index) {
        if (freeCount >= freeCapacity) {
            resizeFreeList();
        }
        freeIndices[freeCount++] = index;
        nodes[index].terhapus = true;
    }
    
    int getTinggi(int index) {
        return (index == -1) ? 0 : nodes[index].tinggi;
    }
    
    int getBalance(int index) {
        if (index == -1) return 0;
        return getTinggi(nodes[index].indeksKiri) - getTinggi(nodes[index].indeksKanan);
    }
    
    int max(int a, int b) {
        return (a > b) ? a : b;
    }
    
    int rotasiKanan(int y) {
        int x = nodes[y].indeksKiri;
        int T2 = nodes[x].indeksKanan;
        
        nodes[x].indeksKanan = y;
        nodes[y].indeksKiri = T2;
        
        nodes[y].tinggi = 1 + max(getTinggi(nodes[y].indeksKiri), 
                                  getTinggi(nodes[y].indeksKanan));
        nodes[x].tinggi = 1 + max(getTinggi(nodes[x].indeksKiri), 
                                  getTinggi(nodes[x].indeksKanan));
        
        return x;
    }
    
    int rotasiKiri(int x) {
        int y = nodes[x].indeksKanan;
        int T2 = nodes[y].indeksKiri;
        
        nodes[y].indeksKiri = x;
        nodes[x].indeksKanan = T2;
        
        nodes[x].tinggi = 1 + max(getTinggi(nodes[x].indeksKiri), 
                                  getTinggi(nodes[x].indeksKanan));
        nodes[y].tinggi = 1 + max(getTinggi(nodes[y].indeksKiri), 
                                  getTinggi(nodes[y].indeksKanan));
        
        return y;
    }
    
    int sisipNode(int index, const T& data) {
        if (index == -1) {
            int newIndex = getFreeIndex();
            nodes[newIndex] = AVLNode(data);
            return newIndex;
        }
        
        if (data < nodes[index].data) {
            nodes[index].indeksKiri = sisipNode(nodes[index].indeksKiri, data);
        } else if (nodes[index].data < data) {
            nodes[index].indeksKanan = sisipNode(nodes[index].indeksKanan, data);
        } else {
            return index;
        }
        
        nodes[index].tinggi = 1 + max(getTinggi(nodes[index].indeksKiri), 
                                      getTinggi(nodes[index].indeksKanan));
        
        int balance = getBalance(index);
        
        if (balance > 1 && data < nodes[nodes[index].indeksKiri].data) {
            return rotasiKanan(index);
        }
        
        if (balance < -1 && nodes[nodes[index].indeksKanan].data < data) {
            return rotasiKiri(index);
        }
        
        if (balance > 1 && nodes[nodes[index].indeksKiri].data < data) {
            nodes[index].indeksKiri = rotasiKiri(nodes[index].indeksKiri);
            return rotasiKanan(index);
        }
        
        if (balance < -1 && data < nodes[nodes[index].indeksKanan].data) {
            nodes[index].indeksKanan = rotasiKanan(nodes[index].indeksKanan);
            return rotasiKiri(index);
        }
        
        return index;
    }
    
    int cariMin(int index) {
        int current = index;
        while (current != -1 && nodes[current].indeksKiri != -1) {
            current = nodes[current].indeksKiri;
        }
        return current;
    }
    
    int hapusNode(int index, const T& data) {
        if (index == -1) return index;
        
        if (data < nodes[index].data) {
            nodes[index].indeksKiri = hapusNode(nodes[index].indeksKiri, data);
        } else if (nodes[index].data < data) {
            nodes[index].indeksKanan = hapusNode(nodes[index].indeksKanan, data);
        } else {
            if (nodes[index].indeksKiri == -1 || nodes[index].indeksKanan == -1) {
                int temp = (nodes[index].indeksKiri != -1) ? 
                          nodes[index].indeksKiri : nodes[index].indeksKanan;
                
                if (temp == -1) {
                    addToFreeList(index);
                    return -1;
                } else {
                    nodes[index] = nodes[temp];
                    addToFreeList(temp);
                }
            } else {
                int temp = cariMin(nodes[index].indeksKanan);
                nodes[index].data = nodes[temp].data;
                nodes[index].indeksKanan = hapusNode(nodes[index].indeksKanan, nodes[temp].data);
            }
        }
        
        if (index == -1) return index;
        
        nodes[index].tinggi = 1 + max(getTinggi(nodes[index].indeksKiri), 
                                      getTinggi(nodes[index].indeksKanan));
        
        int balance = getBalance(index);
        
        if (balance > 1 && getBalance(nodes[index].indeksKiri) >= 0) {
            return rotasiKanan(index);
        }
        
        if (balance > 1 && getBalance(nodes[index].indeksKiri) < 0) {
            nodes[index].indeksKiri = rotasiKiri(nodes[index].indeksKiri);
            return rotasiKanan(index);
        }
        
        if (balance < -1 && getBalance(nodes[index].indeksKanan) <= 0) {
            return rotasiKiri(index);
        }
        
        if (balance < -1 && getBalance(nodes[index].indeksKanan) > 0) {
            nodes[index].indeksKanan = rotasiKanan(nodes[index].indeksKanan);
            return rotasiKiri(index);
        }
        
        return index;
    }
    
    bool cariNode(int index, const T& data) {
        if (index == -1 || nodes[index].terhapus) return false;
        
        if (nodes[index].data == data) return true;
        
        if (data < nodes[index].data) {
            return cariNode(nodes[index].indeksKiri, data);
        }
        
        return cariNode(nodes[index].indeksKanan, data);
    }
    
    void inorder(int index, T* result, int& count) {
        if (index != -1 && !nodes[index].terhapus) {
            inorder(nodes[index].indeksKiri, result, count);
            result[count++] = nodes[index].data;
            inorder(nodes[index].indeksKanan, result, count);
        }
    }
    
    int hitungNode(int index) {
        if (index == -1 || nodes[index].terhapus) return 0;
        return 1 + hitungNode(nodes[index].indeksKiri) + 
               hitungNode(nodes[index].indeksKanan);
    }

public:
    AVLTreeDynamic() : nodes(nullptr), kapasitas(10), jumlahNode(0), 
                      rootIndex(-1), freeIndices(nullptr), 
                      freeCount(0), freeCapacity(10) {
        nodes = new AVLNode[kapasitas];
        freeIndices = new int[freeCapacity];
    }
    
    ~AVLTreeDynamic() {
        if (nodes) delete[] nodes;
        if (freeIndices) delete[] freeIndices;
    }
    
    void sisipkan(const T& data) {
        rootIndex = sisipNode(rootIndex, data);
    }
    
    void hapus(const T& data) {
        rootIndex = hapusNode(rootIndex, data);
    }
    
    bool cari(const T& data) {
        return cariNode(rootIndex, data);
    }
    
    void inorder(T* result, int& count) {
        count = 0;
        inorder(rootIndex, result, count);
    }
    
    int ukuran() {
        return hitungNode(rootIndex);
    }
    
    bool kosong() {
        return ukuran() == 0;
    }
    
    T* dapatkanSemuaTerurut() {
        int size = ukuran();
        T* result = new T[size];
        int count = 0;
        inorder(rootIndex, result, count);
        return result;
    }
    
    void cetakPohon(int index = -2, string indent = "", bool last = true) {
        if (index == -2) index = rootIndex;
        if (index == -1 || nodes[index].terhapus) return;
        
        cout << indent;
        if (last) {
            cout << "└── ";
            indent += "    ";
        } else {
            cout << "├── ";
            indent += "│   ";
        }
        
        cout << nodes[index].data << " [h=" << nodes[index].tinggi 
             << ", b=" << getBalance(index) << "]" << endl;
        
        bool hasLeft = (nodes[index].indeksKiri != -1 && !nodes[nodes[index].indeksKiri].terhapus);
        bool hasRight = (nodes[index].indeksKanan != -1 && !nodes[nodes[index].indeksKanan].terhapus);
        
        if (hasLeft) {
            cetakPohon(nodes[index].indeksKiri, indent, !hasRight);
        }
        if (hasRight) {
            cetakPohon(nodes[index].indeksKanan, indent, true);
        }
    }
};

#endif