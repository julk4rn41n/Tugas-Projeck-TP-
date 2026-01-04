#ifndef STACK_H
#define STACK_H

#include <iostream>
#include <fstream>

using namespace std;

struct DataHapus {
    string username;
    string password;
    string role;
    long long int nim;
    string nama;
};

class UndoStack {
private:
    DataHapus* stack;
    int top;
    int capacity;
    const string filename = "undo.txt";

    void resize() {
        int newCapacity = capacity * 2;
        DataHapus* newStack = new DataHapus[newCapacity];
        
        for (int i = 0; i <= top; i++) {
            newStack[i] = stack[i];
        }
        
        delete[] stack;
        stack = newStack;
        capacity = newCapacity;
    }

    long long int safe_stoll(const string& str) {
        if (str.empty()) return 0;
        try {
            return stoll(str);
        } catch (...) {
            return 0;
        }
    }

public:
    UndoStack(int initialCapacity = 10) {
        capacity = initialCapacity;
        stack = new DataHapus[capacity];
        top = -1;
        loadFromFile();
    }

    ~UndoStack() {
        if (stack) {
            delete[] stack;
        }
    }

    bool isEmpty() {
        return top == -1;
    }

    bool isFull() {
        return top == capacity - 1;
    }

    void push(const DataHapus& data) {
        if (isFull()) {
            resize();
        }
        stack[++top] = data;
        saveToFile();
    }

    DataHapus pop() {
        if (isEmpty()) {
            DataHapus empty = {"", "", "", 0, ""};
            return empty;
        }
        
        DataHapus data = stack[top--];
        
        // Update file dengan menghapus data teratas
        ifstream file(filename);
        string* lines = new string[200];
        int count = 0;
        
        while (getline(file, lines[count])) {
            count++;
        }
        file.close();
        
        if (count > 0) count--;
        
        ofstream out(filename);
        for (int i = 0; i < count; i++) {
            out << lines[i] << "\n";
        }
        out.close();
        
        delete[] lines;
        
        return data;
    }

    DataHapus peek() {
        if (isEmpty()) {
            DataHapus empty = {"", "", "", 0, ""};
            return empty;
        }
        return stack[top];
    }

    int size() {
        return top + 1;
    }

    void clear() {
        top = -1;
        ofstream file(filename, ios::trunc);
        file.close();
    }

    void saveToFile() {
        ofstream file(filename, ios::app);
        if (file.is_open()) {
            DataHapus data = stack[top]; // Simpan hanya yang terbaru
            file << data.username << "|"
                 << data.password << "|"
                 << data.role << "|"
                 << data.nim << "|"
                 << data.nama << "\n";
            file.close();
        }
    }

    void loadFromFile() {
        ifstream file(filename);
        if (!file.is_open()) return;
        
        string line;
        while (getline(file, line)) {
            if (line.empty()) continue;
            
            DataHapus data;
            int pos1 = 0, pos2 = 0;
            
            pos2 = line.find('|', pos1);
            data.username = line.substr(pos1, pos2 - pos1);
            
            pos1 = pos2 + 1;
            pos2 = line.find('|', pos1);
            data.password = line.substr(pos1, pos2 - pos1);
            
            pos1 = pos2 + 1;
            pos2 = line.find('|', pos1);
            data.role = line.substr(pos1, pos2 - pos1);
            
            pos1 = pos2 + 1;
            pos2 = line.find('|', pos1);
            data.nim = safe_stoll(line.substr(pos1, pos2 - pos1));
            
            pos1 = pos2 + 1;
            data.nama = line.substr(pos1);
            
            push(data);
        }
        file.close();
    }

    void display() {
        cout << "\n+==================================================+\n";
        cout << "|              ISI STACK (UNDO)                   |\n";
        cout << "+==================================================+\n";
        
        if (isEmpty()) {
            cout << "|              Stack kosong                        |\n";
        } else {
            for (int i = top; i >= 0; i--) {
                cout << "| [" << (top - i + 1) << "] " << stack[i].username 
                     << " - " << stack[i].nama << " (" << stack[i].role << ")\n";
            }
        }
        cout << "+==================================================+\n";
    }
};

#endif