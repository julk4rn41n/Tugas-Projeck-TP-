#include <iostream>
#include "avl_tree.h"
#include "huffman_tree.h"

using namespace std;

void test_avl() {
    cout << "=== AVL Tree Demo ===\n";
    AVLTreeDynamic<int> tree;
    int values[] = {50, 20, 70, 10, 30, 60, 80};
    for (int v : values) tree.sisipkan(v);

    int n = tree.ukuran();
    int* sorted = tree.dapatkanSemuaTerurut();
    cout << "Inorder (sorted): ";
    for (int i = 0; i < n; ++i) cout << sorted[i] << " ";
    cout << "\n\nTree structure:\n";
    tree.cetakPohon();

    cout << "\nDeleting 20...\n";
    tree.hapus(20);
    delete[] sorted;

    n = tree.ukuran();
    sorted = tree.dapatkanSemuaTerurut();
    cout << "Inorder after delete: ";
    for (int i = 0; i < n; ++i) cout << sorted[i] << " ";
    cout << "\n";
    delete[] sorted;
}

void test_huffman() {
    cout << "\n=== Huffman Demo ===\n";
    HuffmanTreeDynamic ht;
    string sample = "this is an example for huffman encoding";
    string compressed = ht.kompres(sample);
    cout << "Original: " << sample << "\n";
    cout << "Compressed bits: " << compressed << "\n\nCodes:\n";
    ht.tampilkanKode();
    string decompressed = ht.dekompres(compressed);
    cout << "\nDecompressed: " << decompressed << "\n";
}

int main() {
    test_avl();
    test_huffman();
    return 0;
}
