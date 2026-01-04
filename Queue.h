#ifndef QUEUE_H
#define QUEUE_H

#include <iostream>
#include <fstream>
#include <iomanip>
#include <string>

using namespace std;

struct Notifikasi {
    string id;
    string penerima;
    string pengirim;
    string pesan;
    string tanggal;
    string waktu;
    string status;
};

#ifndef NOTIFIKASI_STRUCT_DEFINED
#define NOTIFIKASI_STRUCT_DEFINED
#endif

class NotificationQueue {
private:
    Notifikasi* data;
    int front;
    int rear;
    int count;
    int capacity;
    const string filename = "notifikasi.txt";

    void resize() {
        int newCapacity = capacity * 2;
        Notifikasi* newData = new Notifikasi[newCapacity];
        
        if (!isEmpty()) {
            int index = front;
            for (int i = 0; i < count; i++) {
                newData[i] = data[index];
                index = (index + 1) % capacity;
            }
        }
        
        delete[] data;
        data = newData;
        front = 0;
        rear = count - 1;
        capacity = newCapacity;
    }

public:
    NotificationQueue(int initialCapacity = 10) {
        capacity = initialCapacity;
        data = new Notifikasi[capacity];
        front = -1;
        rear = -1;
        count = 0;
    }

    ~NotificationQueue() {
        if (data) {
            delete[] data;
        }
    }

    bool isEmpty() {
        return count == 0;
    }

    bool isFull() {
        return count == capacity;
    }

    void enqueue(const Notifikasi& notif) {
        if (isFull()) {
            resize();
        }
        
        if (isEmpty()) {
            front = 0;
            rear = 0;
        } else {
            rear = (rear + 1) % capacity;
        }
        
        data[rear] = notif;
        count++;
    }

    Notifikasi dequeue() {
        if (isEmpty()) {
            Notifikasi empty;
            empty.id = "EMPTY";
            return empty;
        }
        
        Notifikasi notif = data[front];
        if (front == rear) {
            front = -1;
            rear = -1;
        } else {
            front = (front + 1) % capacity;
        }
        count--;
        return notif;
    }

    Notifikasi peek() {
        if (isEmpty()) {
            Notifikasi empty;
            empty.id = "EMPTY";
            return empty;
        }
        return data[front];
    }

    int size() {
        return count;
    }

    void clear() {
        front = -1;
        rear = -1;
        count = 0;
    }

    void saveToFile() {
        ofstream file(filename, ios::trunc);
        if (!file) return;
        
        if (isEmpty()) {
            file.close();
            return;
        }
        
        int index = front;
        for (int i = 0; i < count; i++) {
            Notifikasi notif = data[index];
            file << notif.id << "|" 
                 << notif.penerima << "|"
                 << notif.pengirim << "|"
                 << notif.pesan << "|"
                 << notif.tanggal << "|"
                 << notif.waktu << "|"
                 << notif.status << "\n";
            index = (index + 1) % capacity;
        }
        
        file.close();
    }

    void loadFromFile() {
        ifstream file(filename);
        if (!file) return;
        
        string line;
        while (getline(file, line)) {
            if (line.empty()) continue;
            
            Notifikasi notif;
            int pos1 = 0, pos2 = 0;
            
            pos2 = line.find('|', pos1);
            notif.id = line.substr(pos1, pos2 - pos1);
            
            pos1 = pos2 + 1;
            pos2 = line.find('|', pos1);
            notif.penerima = line.substr(pos1, pos2 - pos1);
            
            pos1 = pos2 + 1;
            pos2 = line.find('|', pos1);
            notif.pengirim = line.substr(pos1, pos2 - pos1);
            
            pos1 = pos2 + 1;
            pos2 = line.find('|', pos1);
            notif.pesan = line.substr(pos1, pos2 - pos1);
            
            pos1 = pos2 + 1;
            pos2 = line.find('|', pos1);
            notif.tanggal = line.substr(pos1, pos2 - pos1);
            
            pos1 = pos2 + 1;
            pos2 = line.find('|', pos1);
            notif.waktu = line.substr(pos1, pos2 - pos1);
            
            pos1 = pos2 + 1;
            notif.status = line.substr(pos1);
            
            enqueue(notif);
        }
        
        file.close();
    }

    void display() {
        cout << "\n+================================================================================+\n";
        cout << "|                              ISI QUEUE NOTIFIKASI                             |\n";
        cout << "+================================================================================+\n";
        
        if (isEmpty()) {
            cout << "|                          Queue kosong                                        |\n";
        } else {
            cout << "| No | Penerima           | Pengirim           | Pesan                         |\n";
            cout << "+----+--------------------+--------------------+-------------------------------+\n";
            
            int index = front;
            for (int i = 0; i < count; i++) {
                Notifikasi notif = data[index];
                string pesan = notif.pesan;
                if (pesan.length() > 20) {
                    pesan = pesan.substr(0, 17) + "...";
                }

                cout << left << "| " << setw(2) << right << (i + 1) << " "
                     << "| " << setw(18) << left << notif.penerima
                     << "| " << setw(18) << left << notif.pengirim
                     << "| " << setw(29) << left << pesan << "|\n";
                
                index = (index + 1) % capacity;
            }
        }
        cout << "+================================================================================+\n";
        cout << "Total notifikasi: " << count << "\n";
    }
};

#endif