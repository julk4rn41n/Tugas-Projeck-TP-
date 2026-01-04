#include <iostream>
#include <iomanip>
#include <fstream>

using namespace std;
#include "Stack.h"
#include "Queue.h"
#include "avl_tree.h"
#include "huffman_tree.h"

// Kemudian deklarasikan variabel global:
extern UndoStack undoStack;
extern NotificationQueue notificationQueue;
UndoStack undoStack;
NotificationQueue notificationQueue;

// =====================================================================
// FUNGSI HELPER UNTUK KONVERSI AMAN (DITAMBAHKAN DI ATAS SEMUA STRUCT)
// =====================================================================
long long int safe_stoll(const string& str) {
    if (str.empty()) {
        return 0;
    }
    
    // Cek apakah string valid untuk konversi
    bool valid = true;
    bool has_minus = false;
    
    // Cek karakter pertama
    size_t start = 0;
    if (str[0] == '-') {
        has_minus = true;
        start = 1;
    }
    
    // Cek semua karakter adalah digit
    for (size_t i = start; i < str.length(); i++) {
        if (str[i] < '0' || str[i] > '9') {
            valid = false;
            break;
        }
    }
    
    if (!valid || (start == 1 && str.length() == 1)) {
        return 0;
    }
    
    // Konversi manual
    long long int result = 0;
    for (size_t i = start; i < str.length(); i++) {
        result = result * 10 + (str[i] - '0');
    }
    
    if (has_minus) {
        result = -result;
    }
    
    return result;
}

// =====================================================================
// STRUCT AKUN GLOBAL (UNTUK SEMUA CLASS) - menggunakan array dinamis
// =====================================================================
struct Akun {
    string username;
    string password;
    string nama;
    long long int nim;        // Menggunakan long long int untuk NIM
    string prodi;      
    string pertanyaan;
    string jawaban;    
    string role;       
};

// Array dinamis untuk menyimpan akun
Akun* daftarAkun = nullptr;
int jumlahAkun = 0;
int kapasitasAkun = 0;

// =====================================================================
// STRUCT QUEUE NOTIFIKASI GLOBAL - DIPERBAIKI
// =====================================================================
#ifndef NOTIFIKASI_STRUCT_DEFINED
struct Notifikasi {
    string id;
    string penerima; // "ALL" untuk semua, "MAHASISWA", "DOSEN", atau username/NIM tertentu
    string pengirim; // "ADMIN" atau username pengirim
    string pesan;
    string tanggal;
    string waktu;
    string status; // "UNREAD" atau "READ"
};
#endif

struct QueueNotifikasi {
    Notifikasi* data; // Array dinamis untuk queue
    int front;
    int rear;
    int count;
    int kapasitas;
};

// Queue notifikasi global
QueueNotifikasi queueNotifikasi;

// =====================================================================
// STRUCT GLOBAL UNTUK MASTER BUKU - menggunakan array dinamis
// =====================================================================
struct BukuMaster {
    string kode;
    string judul;
    int stok;
};

BukuMaster* daftarBuku = nullptr;
int jumlahBuku = 0;
int kapasitasBuku = 0;

// =====================================================================
// STRUCT CHAT/INBOX GLOBAL - menggunakan array dinamis
// =====================================================================
struct Pesan {
    string id;
    string pengirim;
    string penerima;
    string subjek;
    string isi;
    string waktu;
    string status; // "UNREAD" atau "READ"
};

struct ChatSession {
    string user1;
    string user2;
    Pesan* pesan; // Array dinamis
    int jumlahPesan;
    int kapasitasPesan;
};

ChatSession* sesiChat = nullptr;
int jumlahSesiChat = 0;
int kapasitasSesiChat = 0;

// =====================================================================
// STRUCT BUKU PINJAM - menggunakan array dinamis
// =====================================================================
struct BukuPinjam {
    long long int nim;  // Menggunakan long long int
    string kode;
    string judul;
    string tglPinjamStr;
};

BukuPinjam* daftarPinjaman = nullptr;
int jumlahPinjaman = 0;
int kapasitasPinjaman = 0;

// =====================================================================
// STRUCT MATA KULIAH - menggunakan array dinamis (ditambahkan dari 03_01_2026.cpp)
// =====================================================================
struct Matkul {
    string kode;
    string nama;
    int sks;
    string semester;
    string prodi;
};

Matkul* daftarMatkul = nullptr;
int jumlahMatkul = 0;
int kapasitasMatkul = 0;

// =====================================================================
// STRUCT JADWAL KULIAH - menggunakan array dinamis (ditambahkan dari 03_01_2026.cpp)
// =====================================================================
struct JadwalKuliah {
    string kodeMatkul;
    string namaMatkul;
    string hari;
    string jamMulai;
    string jamSelesai;
    string ruangan;
    string semester;
    string kelas;
    string dosen;
};

JadwalKuliah* daftarJadwal = nullptr;
int jumlahJadwal = 0;
int kapasitasJadwal = 0;

// =====================================================================
// STRUCT KRS MAHASISWA - menggunakan array dinamis (ditambahkan dari 03_01_2026.cpp)
// =====================================================================
struct KRSMahasiswa {
    long long int nim;  // Menggunakan long long int
    string kodeMatkul;
    string namaMatkul;
    int sks;
};

KRSMahasiswa* daftarKRS = nullptr;
int jumlahKRS = 0;
int kapasitasKRS = 0;

// =====================================================================
// STRUCT NILAI MAHASISWA - menggunakan array dinamis
// =====================================================================
struct NilaiMahasiswa {
    long long int nim;  // Menggunakan long long int
    string kodeMatkul;
    string namaMatkul;
    int sks;
    string nilai;
};

NilaiMahasiswa* daftarNilai = nullptr;
int jumlahNilai = 0;
int kapasitasNilai = 0;

// =====================================================================
// STRUCT PRESENSI - menggunakan array dinamis
// =====================================================================
struct Presensi {
    long long int nim;  // Menggunakan long long int
    string kodeMatkul;
    string namaMatkul;
    int hadir;
    int sakit;
    int izin;
    int alpha;
};

Presensi* daftarPresensi = nullptr;
int jumlahPresensi = 0;
int kapasitasPresensi = 0;

// =====================================================================
// STRUCT PENGAJUAN - menggunakan array dinamis
// =====================================================================
struct Pengajuan {
    long long int nim;  // Menggunakan long long int
    string jenis;
    string tanggal;
    string status;
};

Pengajuan* daftarPengajuan = nullptr;
int jumlahPengajuan = 0;
int kapasitasPengajuan = 0;

// =====================================================================
// STRUCT PEMBAYARAN - menggunakan array dinamis
// =====================================================================
struct Pembayaran {
    long long int nim;  // Menggunakan long long int
    string tanggal;
    long long int nominal;  // Menggunakan long long int
    string keterangan;
};

Pembayaran* daftarPembayaran = nullptr;
int jumlahPembayaran = 0;
int kapasitasPembayaran = 0;

// =====================================================================
// FUNGSI BANTU UNTUK ARRAY DINAMIS
// =====================================================================
void tambahAkun(const Akun& akun) {
    if (jumlahAkun >= kapasitasAkun) {
        kapasitasAkun = kapasitasAkun == 0 ? 10 : kapasitasAkun * 2;
        Akun* temp = new Akun[kapasitasAkun];
        for (int i = 0; i < jumlahAkun; i++) {
            temp[i] = daftarAkun[i];
        }
        delete[] daftarAkun;
        daftarAkun = temp;
    }
    daftarAkun[jumlahAkun++] = akun;
}

void hapusAkun(int index) {
    if (index < 0 || index >= jumlahAkun) return;
    for (int i = index; i < jumlahAkun - 1; i++) {
        daftarAkun[i] = daftarAkun[i + 1];
    }
    jumlahAkun--;
}

Akun* cariAkunByUsername(const string& username) {
    for (int i = 0; i < jumlahAkun; i++) {
        if (daftarAkun[i].username == username) {
            return &daftarAkun[i];
        }
    }
    return nullptr;
}

Akun* cariAkunByNIM(long long int nim) {
    for (int i = 0; i < jumlahAkun; i++) {
        if (daftarAkun[i].nim == nim) {
            return &daftarAkun[i];
        }
    }
    return nullptr;
}

void tambahBuku(const BukuMaster& buku) {
    if (jumlahBuku >= kapasitasBuku) {
        kapasitasBuku = kapasitasBuku == 0 ? 10 : kapasitasBuku * 2;
        BukuMaster* temp = new BukuMaster[kapasitasBuku];
        for (int i = 0; i < jumlahBuku; i++) {
            temp[i] = daftarBuku[i];
        }
        delete[] daftarBuku;
        daftarBuku = temp;
    }
    daftarBuku[jumlahBuku++] = buku;
}

void hapusBuku(int index) {
    if (index < 0 || index >= jumlahBuku) return;
    for (int i = index; i < jumlahBuku - 1; i++) {
        daftarBuku[i] = daftarBuku[i + 1];
    }
    jumlahBuku--;
}

BukuMaster* cariBukuByKode(const string& kode) {
    for (int i = 0; i < jumlahBuku; i++) {
        if (daftarBuku[i].kode == kode) {
            return &daftarBuku[i];
        }
    }
    return nullptr;
}

void tambahJadwal(const JadwalKuliah& jadwal) {
    if (jumlahJadwal >= kapasitasJadwal) {
        kapasitasJadwal = kapasitasJadwal == 0 ? 10 : kapasitasJadwal * 2;
        JadwalKuliah* temp = new JadwalKuliah[kapasitasJadwal];
        for (int i = 0; i < jumlahJadwal; i++) {
            temp[i] = daftarJadwal[i];
        }
        delete[] daftarJadwal;
        daftarJadwal = temp;
    }
    daftarJadwal[jumlahJadwal++] = jadwal;
}

void hapusJadwal(int index) {
    if (index < 0 || index >= jumlahJadwal) return;
    for (int i = index; i < jumlahJadwal - 1; i++) {
        daftarJadwal[i] = daftarJadwal[i + 1];
    }
    jumlahJadwal--;
}

void tambahMatkulGlobal(const Matkul& matkul) {
    if (jumlahMatkul >= kapasitasMatkul) {
        kapasitasMatkul = kapasitasMatkul == 0 ? 10 : kapasitasMatkul * 2;
        Matkul* temp = new Matkul[kapasitasMatkul];
        for (int i = 0; i < jumlahMatkul; i++) {
            temp[i] = daftarMatkul[i];
        }
        delete[] daftarMatkul;
        daftarMatkul = temp;
    }
    daftarMatkul[jumlahMatkul++] = matkul;
}

void hapusMatkulGlobal(int index) {
    if (index < 0 || index >= jumlahMatkul) return;
    for (int i = index; i < jumlahMatkul - 1; i++) {
        daftarMatkul[i] = daftarMatkul[i + 1];
    }
    jumlahMatkul--;
}

Matkul* cariMatkulByKode(const string& kode) {
    for (int i = 0; i < jumlahMatkul; i++) {
        if (daftarMatkul[i].kode == kode) {
            return &daftarMatkul[i];
        }
    }
    return nullptr;
}

ChatSession* cariAtauBuatSesiChat(const string& user1, const string& user2) {
    for (int i = 0; i < jumlahSesiChat; i++) {
        if ((sesiChat[i].user1 == user1 && sesiChat[i].user2 == user2) ||
            (sesiChat[i].user1 == user2 && sesiChat[i].user2 == user1)) {
            return &sesiChat[i];
        }
    }
    
    if (jumlahSesiChat >= kapasitasSesiChat) {
        kapasitasSesiChat = kapasitasSesiChat == 0 ? 5 : kapasitasSesiChat * 2;
        ChatSession* temp = new ChatSession[kapasitasSesiChat];
        for (int i = 0; i < jumlahSesiChat; i++) {
            temp[i] = sesiChat[i];
        }
        delete[] sesiChat;
        sesiChat = temp;
    }
    
    sesiChat[jumlahSesiChat].user1 = user1;
    sesiChat[jumlahSesiChat].user2 = user2;
    sesiChat[jumlahSesiChat].pesan = nullptr;
    sesiChat[jumlahSesiChat].jumlahPesan = 0;
    sesiChat[jumlahSesiChat].kapasitasPesan = 0;
    
    return &sesiChat[jumlahSesiChat++];
}

void tambahPesanKeSesi(ChatSession* sesi, const Pesan& pesan) {
    if (sesi->jumlahPesan >= sesi->kapasitasPesan) {
        sesi->kapasitasPesan = sesi->kapasitasPesan == 0 ? 10 : sesi->kapasitasPesan * 2;
        Pesan* temp = new Pesan[sesi->kapasitasPesan];
        for (int i = 0; i < sesi->jumlahPesan; i++) {
            temp[i] = sesi->pesan[i];
        }
        delete[] sesi->pesan;
        sesi->pesan = temp;
    }
    sesi->pesan[sesi->jumlahPesan++] = pesan;
}

void tambahPinjaman(const BukuPinjam& pinjaman) {
    if (jumlahPinjaman >= kapasitasPinjaman) {
        kapasitasPinjaman = kapasitasPinjaman == 0 ? 10 : kapasitasPinjaman * 2;
        BukuPinjam* temp = new BukuPinjam[kapasitasPinjaman];
        for (int i = 0; i < jumlahPinjaman; i++) {
            temp[i] = daftarPinjaman[i];
        }
        delete[] daftarPinjaman;
        daftarPinjaman = temp;
    }
    daftarPinjaman[jumlahPinjaman++] = pinjaman;
}

void hapusPinjamanByIndex(int index) {
    if (index < 0 || index >= jumlahPinjaman) return;
    for (int i = index; i < jumlahPinjaman - 1; i++) {
        daftarPinjaman[i] = daftarPinjaman[i + 1];
    }
    jumlahPinjaman--;
}

int cariIndexPinjaman(long long int nim, const string& kode) {
    for (int i = 0; i < jumlahPinjaman; i++) {
        if (daftarPinjaman[i].nim == nim && daftarPinjaman[i].kode == kode) {
            return i;
        }
    }
    return -1;
}

void tambahKRS(const KRSMahasiswa& krs) {
    if (jumlahKRS >= kapasitasKRS) {
        kapasitasKRS = kapasitasKRS == 0 ? 10 : kapasitasKRS * 2;
        KRSMahasiswa* temp = new KRSMahasiswa[kapasitasKRS];
        for (int i = 0; i < jumlahKRS; i++) {
            temp[i] = daftarKRS[i];
        }
        delete[] daftarKRS;
        daftarKRS = temp;
    }
    daftarKRS[jumlahKRS++] = krs;
}

void hapusKRSByIndex(int index) {
    if (index < 0 || index >= jumlahKRS) return;
    for (int i = index; i < jumlahKRS - 1; i++) {
        daftarKRS[i] = daftarKRS[i + 1];
    }
    jumlahKRS--;
}

int cariIndexKRS(long long int nim, const string& kodeMatkul) {
    for (int i = 0; i < jumlahKRS; i++) {
        if (daftarKRS[i].nim == nim && daftarKRS[i].kodeMatkul == kodeMatkul) {
            return i;
        }
    }
    return -1;
}

// =====================================================================
// FUNGSI BANTU UNTUK MENYIMPAN KRS PER MAHASISWA (DITAMBAHKAN)
// =====================================================================
void simpanKRSMahasiswaKeFile(long long int nim) {
    string namaFile = "KRS_" + to_string(nim) + ".txt";
    ofstream file(namaFile.c_str());
    
    if (!file) {
        cout << "Gagal membuat file KRS untuk NIM " << nim << endl;
        return;
    }
    
    // Cari data mahasiswa
    Akun* mahasiswa = cariAkunByNIM(nim);
    string namaMahasiswa = (mahasiswa) ? mahasiswa->nama : "Mahasiswa";
    string prodiMahasiswa = (mahasiswa) ? mahasiswa->prodi : "-";
    
    // Header file
    file << "+=================================================================+\n";
    file << "|                       KARTU RENCANA STUDI                       |\n";
    file << "+=================================================================+\n\n";
    file << "Nama Mahasiswa  : " << namaMahasiswa << "\n";
    file << "NIM             : " << nim << "\n";
    file << "Program Studi   : " << prodiMahasiswa << "\n";
    file << "Semester        : Semester 5 (2024/2025)\n";
    file << "Tanggal Cetak   : 01-12-2025 10:30:00\n\n";
    
    file << "+============+=========================================+====+\n";
    file << "| Kode MK    | Nama Mata Kuliah                        |SKS |\n";
    file << "+============+=========================================+====+\n";

    int totalSKS = 0;
    bool adaKRS = false;
    
    for (int i = 0; i < jumlahKRS; i++) {
        if (daftarKRS[i].nim == nim) {
            adaKRS = true;
            totalSKS += daftarKRS[i].sks;
            file << "| " << setw(10) << left << daftarKRS[i].kodeMatkul
                 << " | " << setw(41) << left << daftarKRS[i].namaMatkul
                 << "| " << setw(3) << right << daftarKRS[i].sks << " |\n";
            file << "+------------+-----------------------------------------+----+\n";
        }
    }
    
    if (!adaKRS) {
        file << "|               Belum ada mata kuliah dalam KRS               |\n";
        file << "+------------+-----------------------------------------+----+\n";
    }
    
    file << "| TOTAL SKS  | " << setw(41) << left << " "
         << "| " << setw(3) << right << totalSKS << " |\n";
    file << "+============+=========================================+====+\n\n";
    
    file << "Catatan:\n";
    file << "1. KRS ini berlaku untuk semester berjalan\n";
    file << "2. Maksimal SKS yang dapat diambil: 24 SKS\n";
    file << "3. KRS harus disetujui oleh dosen wali\n";
    
    file.close();
}

// =====================================================================
// FUNGSI BANTU UNTUK MEMUAT KRS DARI FILE PER MAHASISWA
// =====================================================================
void muatKRSMahasiswaDariFile(long long int nim) {
    string namaFile = "KRS_" + to_string(nim) + ".txt";
    ifstream file(namaFile.c_str());
    
    if (!file) {
        // File tidak ada, tidak perlu dimuat
        return;
    }
    
    // Hapus KRS lama untuk NIM ini
    for (int i = 0; i < jumlahKRS; i++) {
        if (daftarKRS[i].nim == nim) {
            hapusKRSByIndex(i);
            i--; // Kembali ke index sebelumnya karena array bergeser
        }
    }
    
    // Lewati header file (kita tidak perlu baca dari file txt karena sudah ada di array)
    file.close();
}

// =====================================================================
// FUNGSI BANTU UNTUK MENAMPILKAN KHS SESUAI KRS MAHASISWA (DITAMBAHKAN)
// =====================================================================
void tampilkanKHSSesuaiKRS(long long int nim) {
    system("cls");
    
    // Cari data mahasiswa
    Akun* mahasiswa = cariAkunByNIM(nim);
    if (!mahasiswa) {
        cout << "Data mahasiswa tidak ditemukan!\n";
        cout << "\nTekan ENTER untuk kembali...";
        cin.get();
        return;
    }
    
    cout << "\n+================================================================================+\n";
    cout << "|                     KARTU HASIL STUDI (KHS) - " << setw(28) << left << mahasiswa->nama << " |\n";
    cout << "|                                NIM: " << setw(40) << left << nim << " |\n";
    cout << "|                            Program Studi: " << setw(36) << left << mahasiswa->prodi << " |\n";
    cout << "+================================================================================+\n";
    
    // Cari mata kuliah yang diambil di KRS
    bool adaKRS = false;
    bool adaNilai = false;
    int totalSKSKRS = 0;
    int totalSKSKHS = 0;
    double totalBobot = 0.0;
    
    cout << "+============+=========================================+====+============+===========+\n";
    cout << "| Kode MK    | Nama Mata Kuliah                        |SKS | Status     | Nilai     |\n";
    cout << "+============+=========================================+====+============+===========+\n";
    
    // Pertama, tampilkan semua mata kuliah yang ada di KRS
    for (int i = 0; i < jumlahKRS; i++) {
        if (daftarKRS[i].nim == nim) {
            adaKRS = true;
            totalSKSKRS += daftarKRS[i].sks;
            
            // Cek apakah ada nilai untuk mata kuliah ini
            bool nilaiDitemukan = false;
            string nilai = "-";
            string status = "Belum Nilai";
            
            for (int j = 0; j < jumlahNilai; j++) {
                if (daftarNilai[j].nim == nim && daftarNilai[j].kodeMatkul == daftarKRS[i].kodeMatkul) {
                    nilaiDitemukan = true;
                    nilai = daftarNilai[j].nilai;
                    status = "Sudah Nilai";
                    adaNilai = true;
                    totalSKSKHS += daftarKRS[i].sks;
                    
                    // Hitung bobot nilai
                    if (nilai == "A") totalBobot += 4.0 * daftarKRS[i].sks;
                    else if (nilai == "A-") totalBobot += 3.7 * daftarKRS[i].sks;
                    else if (nilai == "B+") totalBobot += 3.3 * daftarKRS[i].sks;
                    else if (nilai == "B") totalBobot += 3.0 * daftarKRS[i].sks;
                    else if (nilai == "B-") totalBobot += 2.7 * daftarKRS[i].sks;
                    else if (nilai == "C+") totalBobot += 2.3 * daftarKRS[i].sks;
                    else if (nilai == "C") totalBobot += 2.0 * daftarKRS[i].sks;
                    else if (nilai == "D") totalBobot += 1.0 * daftarKRS[i].sks;
                    else if (nilai == "E") totalBobot += 0.0 * daftarKRS[i].sks;
                    break;
                }
            }
            
            cout << "| " << setw(10) << left << daftarKRS[i].kodeMatkul
                 << " | " << setw(41) << left << daftarKRS[i].namaMatkul
                 << "| " << setw(3) << right << daftarKRS[i].sks
                 << " | " << setw(10) << left << status
                 << " | " << setw(9) << left << nilai << " |\n";
            cout << "+------------+-----------------------------------------+----+------------+-----------+\n";
        }
    }
    
    if (!adaKRS) {
        cout << "|               Belum ada mata kuliah dalam KRS                                    |\n";
        cout << "+------------+-----------------------------------------+----+------------+-----------+\n";
    }
    
    // Tampilkan total SKS dan IPK jika ada nilai
    cout << "| TOTAL SKS  | " << setw(41) << left << " "
         << "| " << setw(3) << right << totalSKSKRS
         << " | " << setw(10) << left << " "
         << " | " << setw(9) << left << " " << " |\n";
    cout << "+============+=========================================+====+============+===========+\n\n";
    
    if (adaNilai && totalSKSKHS > 0) {
        double ipk = totalBobot / totalSKSKHS;
        cout << "Total SKS yang sudah dinilai : " << totalSKSKHS << " SKS\n";
        cout << "IPK Sementara               : " << fixed << setprecision(2) << ipk << "\n";
    } else if (adaKRS) {
        cout << "Belum ada nilai untuk mata kuliah yang diambil.\n";
    }
    
    cout << "\nTekan ENTER untuk kembali...";
    cin.get();
}

// =====================================================================
// FUNGSI VALIDASI
// =====================================================================
bool cekNilaiSudahAda(long long int nim, const string& kodeMatkul) {
    for (int i = 0; i < jumlahNilai; i++) {
        if (daftarNilai[i].nim == nim && daftarNilai[i].kodeMatkul == kodeMatkul) {
            return true;
        }
    }
    return false;
}

bool cekMahasiswaAmbilKRS(long long int nim, const string& kodeMatkul) {
    for (int i = 0; i < jumlahKRS; i++) {
        if (daftarKRS[i].nim == nim && daftarKRS[i].kodeMatkul == kodeMatkul) {
            return true;
        }
    }
    return false;
}

int getIndexNilai(long long int nim, const string& kodeMatkul) {
    for (int i = 0; i < jumlahNilai; i++) {
        if (daftarNilai[i].nim == nim && daftarNilai[i].kodeMatkul == kodeMatkul) {
            return i;
        }
    }
    return -1;
}

bool cekKonflikJadwal(const string& hari, const string& jamMulai, const string& jamSelesai, 
                      const string& ruangan, const string& semester, const string& kelas = "") {
    auto waktuKeMenit = [](const string& waktu) {
        int jam = stoi(waktu.substr(0, 2));
        int menit = stoi(waktu.substr(3, 2));
        return jam * 60 + menit;
    };
    
    int mulai1 = waktuKeMenit(jamMulai);
    int selesai1 = waktuKeMenit(jamSelesai);
    
    for (int i = 0; i < jumlahJadwal; i++) {
        if (daftarJadwal[i].hari == hari && daftarJadwal[i].semester == semester) {
            if (!kelas.empty() && daftarJadwal[i].kelas == kelas) {
                int mulai2 = waktuKeMenit(daftarJadwal[i].jamMulai);
                int selesai2 = waktuKeMenit(daftarJadwal[i].jamSelesai);
                
                if ((mulai1 >= mulai2 && mulai1 < selesai2) ||
                    (selesai1 > mulai2 && selesai1 <= selesai2) ||
                    (mulai1 <= mulai2 && selesai1 >= selesai2)) {
                    return true;
                }
            }
            
            if (daftarJadwal[i].ruangan == ruangan) {
                int mulai2 = waktuKeMenit(daftarJadwal[i].jamMulai);
                int selesai2 = waktuKeMenit(daftarJadwal[i].jamSelesai);
                
                if ((mulai1 >= mulai2 && mulai1 < selesai2) ||
                    (selesai1 > mulai2 && selesai1 <= selesai2) ||
                    (mulai1 <= mulai2 && selesai1 >= selesai2)) {
                    return true;
                }
            }
        }
    }
    return false;
}

// =====================================================================
// FUNGSI BANTU UNTUK QUEUE NOTIFIKASI
// =====================================================================
void initQueueNotifikasi() {
    queueNotifikasi.kapasitas = 10;
    queueNotifikasi.data = new Notifikasi[queueNotifikasi.kapasitas];
    queueNotifikasi.front = -1;
    queueNotifikasi.rear = -1;
    queueNotifikasi.count = 0;
}
bool isQueueEmpty() {
    return queueNotifikasi.count == 0;
}
bool isQueueFull() {
    return queueNotifikasi.count == queueNotifikasi.kapasitas;
}
void resizeQueue() {
    int newCapacity = queueNotifikasi.kapasitas * 2;
    Notifikasi* newData = new Notifikasi[newCapacity];
    
    if (!isQueueEmpty()) {
        int index = queueNotifikasi.front;
        for (int i = 0; i < queueNotifikasi.count; i++) {
            newData[i] = queueNotifikasi.data[index];
            index = (index + 1) % queueNotifikasi.kapasitas;
        }
    }
    
    delete[] queueNotifikasi.data;
    queueNotifikasi.data = newData;
    queueNotifikasi.front = 0;
    queueNotifikasi.rear = queueNotifikasi.count - 1;
    queueNotifikasi.kapasitas = newCapacity;
}
void enqueueNotifikasi(const Notifikasi& notif) {
    if (isQueueFull()) {
        resizeQueue();
    }
    
    if (isQueueEmpty()) {
        queueNotifikasi.front = 0;
        queueNotifikasi.rear = 0;
    } else {
        queueNotifikasi.rear = (queueNotifikasi.rear + 1) % queueNotifikasi.kapasitas;
    }
    
    queueNotifikasi.data[queueNotifikasi.rear] = notif;
    queueNotifikasi.count++;
}
Notifikasi dequeueNotifikasi() {
    if (isQueueEmpty()) {
        Notifikasi empty;
        empty.id = "EMPTY";
        return empty;
    }
    
    Notifikasi notif = queueNotifikasi.data[queueNotifikasi.front];
    if (queueNotifikasi.front == queueNotifikasi.rear) {
        queueNotifikasi.front = -1;
        queueNotifikasi.rear = -1;
    } else {
        queueNotifikasi.front = (queueNotifikasi.front + 1) % queueNotifikasi.kapasitas;
    }
    queueNotifikasi.count--;
    return notif;
}
Notifikasi peekNotifikasi() {
    if (isQueueEmpty()) {
        Notifikasi empty;
        empty.id = "EMPTY";
        return empty;
    }
    return queueNotifikasi.data[queueNotifikasi.front];
}

// =====================================================================
// FUNGSI BANTU UNTUK NOTIFIKASI
// =====================================================================
string generateNotifikasiID() {
    static int counter = 0;
    return "NOTIF_" + to_string(++counter) + "_" + to_string(rand() % 1000);
}

string getCurrentDateTime() {
    return "01-12-2025 10:30:00";
}

bool isPenerimaValid(const string& penerima, const string& kategori) {
    if (kategori == "SEMUA") {
        return true;
    } else if (kategori == "MAHASISWA") {
        for (int i = 0; i < jumlahAkun; i++) {
            if (daftarAkun[i].role == "mahasiswa") {
                return true;
            }
        }
        return false;
    } else if (kategori == "DOSEN") {
        for (int i = 0; i < jumlahAkun; i++) {
            if (daftarAkun[i].role == "dosen") {
                return true;
            }
        }
        return false;
    } else {
        // Cek username/NIM tertentu
        for (int i = 0; i < jumlahAkun; i++) {
            if (daftarAkun[i].username == penerima || 
                to_string(daftarAkun[i].nim) == penerima) {
                return true;
            }
        }
        return false;
    }
}

// =====================================================================
// FUNGSI SIMPAN DAN MUAT NOTIFIKASI DARI FILE
// =====================================================================
void simpanNotifikasiKeFile() {
    ofstream file("notifikasi.txt");
    if (!file) {
        cout << "Gagal membuka file notifikasi untuk disimpan!\n";
        return;
    }
    
    if (isQueueEmpty()) {
        file.close();
        return;
    }
    
    // Simpan semua notifikasi dalam queue
    int index = queueNotifikasi.front;
    for (int i = 0; i < queueNotifikasi.count; i++) {
        Notifikasi notif = queueNotifikasi.data[index];
        file << notif.id << "|" 
             << notif.penerima << "|"
             << notif.pengirim << "|"
             << notif.pesan << "|"
             << notif.tanggal << "|"
             << notif.waktu << "|"
             << notif.status << "\n";
        index = (index + 1) % queueNotifikasi.kapasitas;
    }
    
    file.close();
}

void muatNotifikasiDariFile() {
    ifstream file("notifikasi.txt");
    if (!file) {
        // File tidak ada, tidak ada notifikasi yang perlu dimuat
        return;
    }
    
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
        
        enqueueNotifikasi(notif);
    }
    
    file.close();
}

// =====================================================================
// FUNGSI UNTUK MENGIRIM NOTIFIKASI (TANPA VECTOR)
// =====================================================================
void kirimNotifikasiKePenerima(const string& penerima, const string& kategori, 
                               const string& pesan, const string& pengirim = "ADMIN") {
    
    if (!isPenerimaValid(penerima, kategori)) {
        cout << "Penerima tidak valid atau tidak ditemukan!\n";
        return;
    }
    // Buat array dinamis untuk daftar penerima
    string* daftarPenerima = nullptr;
    int jumlahPenerima = 0;
    int kapasitasPenerima = 0;
    
    // Fungsi untuk menambah penerima ke array dinamis
    auto tambahPenerima = [&](const string& target) {
        if (jumlahPenerima >= kapasitasPenerima) {
            kapasitasPenerima = (kapasitasPenerima == 0) ? 10 : kapasitasPenerima * 2;
            string* temp = new string[kapasitasPenerima];
            for (int i = 0; i < jumlahPenerima; i++) {
                temp[i] = daftarPenerima[i];
            }
            if (daftarPenerima) delete[] daftarPenerima;
            daftarPenerima = temp;
        }
        daftarPenerima[jumlahPenerima++] = target;
    };
    
    if (kategori == "SEMUA") {
        for (int i = 0; i < jumlahAkun; i++) {
            tambahPenerima(daftarAkun[i].username);
        }
    } else if (kategori == "MAHASISWA") {
        for (int i = 0; i < jumlahAkun; i++) {
            if (daftarAkun[i].role == "mahasiswa") {
                tambahPenerima(daftarAkun[i].username);
            }
        }
    } else if (kategori == "DOSEN") {
        // Kirim ke semua dosen
        for (int i = 0; i < jumlahAkun; i++) {
            if (daftarAkun[i].role == "dosen") {
                tambahPenerima(daftarAkun[i].username);
            }
        }
    } else {
        // Kirim ke penerima tertentu
        tambahPenerima(penerima);
    }
    
    // Kirim notifikasi ke setiap penerima
    for (int i = 0; i < jumlahPenerima; i++) {
        const string& target = daftarPenerima[i];
        Notifikasi notifBaru;
        notifBaru.id = generateNotifikasiID();
        notifBaru.penerima = target;
        notifBaru.pengirim = pengirim;
        notifBaru.pesan = pesan;
        string datetime = getCurrentDateTime();
        size_t spacePos = datetime.find(' ');
        notifBaru.tanggal = datetime.substr(0, spacePos);
        notifBaru.waktu = datetime.substr(spacePos + 1);
        notifBaru.status = "UNREAD";
        
        enqueueNotifikasi(notifBaru);
    }
    // Hapus array dinamis
    if (daftarPenerima) {
        delete[] daftarPenerima;
    }
    
    simpanNotifikasiKeFile();
    
    cout << "Notifikasi berhasil dikirim ke " << jumlahPenerima << " penerima!\n";
}

int hitungNotifikasiBelumDibaca(const string& username) {
    if (isQueueEmpty()) return 0;
    
    int count = 0;
    int index = queueNotifikasi.front;
    for (int i = 0; i < queueNotifikasi.count; i++) {
        Notifikasi notif = queueNotifikasi.data[index];
        if (notif.penerima == username || notif.penerima == "ALL" || 
            (notif.penerima == "MAHASISWA" && cariAkunByUsername(username)->role == "mahasiswa") ||
            (notif.penerima == "DOSEN" && cariAkunByUsername(username)->role == "dosen")) {
            if (notif.status == "UNREAD") {
                count++;
            }
        }
        index = (index + 1) % queueNotifikasi.kapasitas;
    }
    
    return count;
}

// =====================================================================
// FUNGSI UNTUK MELIHAT NOTIFIKASI (TANPA VECTOR)
// =====================================================================
void tampilkanNotifikasiUntukUser(const string& username) {
    system("cls");
    
    Akun* user = cariAkunByUsername(username);
    if (!user) {
        cout << "User tidak ditemukan!\n";
        cout << "\nTekan ENTER untuk kembali...";
        cin.get();
        return;
    }
    
    string role = user->role;
    int notifBelumDibaca = hitungNotifikasiBelumDibaca(username);
    
    cout << "\n+================================================================================+\n";
    cout << "|                           NOTIFIKASI SAYA                                      |\n";
    cout << "+================================================================================+\n";
    
    if (notifBelumDibaca > 0) {
        cout << "|  Anda memiliki " << setw(2) << notifBelumDibaca << " notifikasi belum dibaca" 
             << setw(36) << " |\n";
        cout << "+--------------------------------------------------------------------------------+\n";
    }
    
    if (isQueueEmpty()) {
        cout << "|                              Tidak ada notifikasi                               |\n";
        cout << "+--------------------------------------------------------------------------------+\n";
        cout << "\nTekan ENTER untuk kembali...";
        cin.get();
        return;
    }
    
    // Buat array dinamis untuk notifikasi user
    Notifikasi* notifikasiUser = nullptr;
    int jumlahNotifikasiUser = 0;
    int kapasitasNotifikasiUser = 0;
    
    // Fungsi untuk menambah notifikasi ke array dinamis
    auto tambahNotifikasi = [&](const Notifikasi& notif) {
        if (jumlahNotifikasiUser >= kapasitasNotifikasiUser) {
            kapasitasNotifikasiUser = (kapasitasNotifikasiUser == 0) ? 10 : kapasitasNotifikasiUser * 2;
            Notifikasi* temp = new Notifikasi[kapasitasNotifikasiUser];
            for (int i = 0; i < jumlahNotifikasiUser; i++) {
                temp[i] = notifikasiUser[i];
            }
            if (notifikasiUser) delete[] notifikasiUser;
            notifikasiUser = temp;
        }
        notifikasiUser[jumlahNotifikasiUser++] = notif;
    };
    
    int index = queueNotifikasi.front;
    
    // Kumpulkan notifikasi untuk user ini
    for (int i = 0; i < queueNotifikasi.count; i++) {
        Notifikasi notif = queueNotifikasi.data[index];
        
        bool untukUser = false;
        if (notif.penerima == username) {
            untukUser = true;
        } else if (notif.penerima == "ALL") {
            untukUser = true;
        } else if (notif.penerima == "MAHASISWA" && role == "mahasiswa") {
            untukUser = true;
        } else if (notif.penerima == "DOSEN" && role == "dosen") {
            untukUser = true;
        }
        
        if (untukUser) {
            tambahNotifikasi(notif);
        }
        
        index = (index + 1) % queueNotifikasi.kapasitas;
    }
    
    if (jumlahNotifikasiUser == 0) {
        cout << "|                              Tidak ada notifikasi                               |\n";
    } else {
        // Tampilkan dari yang terbaru (LIFO untuk tampilan)
        cout << "+----+---------------------+-----------------------------------------------------+\n";
        cout << "| No | Pengirim            | Pesan                                               |\n";
        cout << "+----+---------------------+-----------------------------------------------------+\n";
        
        for (int i = jumlahNotifikasiUser - 1; i >= 0; i--) {
            Notifikasi notif = notifikasiUser[i];
            
            string statusIcon = (notif.status == "UNREAD") ? " [BARU]" : "";
            string pesan = notif.pesan;
            if (pesan.length() > 50) {
                pesan = pesan.substr(0, 47) + "...";
            }
            
            cout << "| " << setw(2) << right << (jumlahNotifikasiUser - i) << " "
                 << "| " << setw(19) << left << notif.pengirim
                 << "| " << setw(51) << left << (pesan + statusIcon) << "|\n";
            
            // Tandai sebagai sudah dibaca
            if (notif.status == "UNREAD") {
                // Update status di queue
                int queueIndex = queueNotifikasi.front;
                for (int j = 0; j < queueNotifikasi.count; j++) {
                    if (queueNotifikasi.data[queueIndex].id == notif.id) {
                        queueNotifikasi.data[queueIndex].status = "READ";
                        break;
                    }
                    queueIndex = (queueIndex + 1) % queueNotifikasi.kapasitas;
                }
            }
        }
        cout << "+----+---------------------+-----------------------------------------------------+\n";
    }
    
    // Hapus array dinamis
    if (notifikasiUser) {
        delete[] notifikasiUser;
    }
    
    simpanNotifikasiKeFile(); // Simpan perubahan status
    
    cout << "\nTekan ENTER untuk kembali...";
    cin.get();
}

// =====================================================================
// CLASS CHAT SYSTEM
// =====================================================================
class ChatSystem {
private:
    string currentUser;

    string generateMessageID() {
        static int counter = 0;
        return "MSG_" + to_string(++counter) + "_" + to_string(rand() % 1000);
    }

    string getCurrentTime() {
        return "01-12-2025 10:30";
    }

    void simpanPesanKeFile(const Pesan& pesan) {
        string filename = "chat_" + pesan.pengirim + "_" + pesan.penerima + ".txt";
        ofstream file(filename.c_str(), ios::app);
        if (file) {
            file << pesan.id << "|" << pesan.pengirim << "|" << pesan.penerima << "|"
                 << pesan.subjek << "|" << pesan.isi << "|" << pesan.waktu << "|" 
                 << pesan.status << "\n";
            file.close();
        }
    }

    void muatPesanDariFile(const string& user1, const string& user2) {
        string filename1 = "chat_" + user1 + "_" + user2 + ".txt";
        string filename2 = "chat_" + user2 + "_" + user1 + ".txt";
        
        ChatSession* session = cariAtauBuatSesiChat(user1, user2);
        
        if (session->pesan) {
            delete[] session->pesan;
        }
        session->pesan = nullptr;
        session->jumlahPesan = 0;
        session->kapasitasPesan = 0;
        
        ifstream file1(filename1.c_str());
        if (file1) {
            string line;
            while (getline(file1, line)) {
                Pesan pesan;
                int pos1 = 0, pos2 = 0;
                
                pos2 = line.find('|', pos1);
                pesan.id = line.substr(pos1, pos2 - pos1);
                
                pos1 = pos2 + 1;
                pos2 = line.find('|', pos1);
                pesan.pengirim = line.substr(pos1, pos2 - pos1);
                
                pos1 = pos2 + 1;
                pos2 = line.find('|', pos1);
                pesan.penerima = line.substr(pos1, pos2 - pos1);
                
                pos1 = pos2 + 1;
                pos2 = line.find('|', pos1);
                pesan.subjek = line.substr(pos1, pos2 - pos1);
                
                pos1 = pos2 + 1;
                pos2 = line.find('|', pos1);
                pesan.isi = line.substr(pos1, pos2 - pos1);
                
                pos1 = pos2 + 1;
                pos2 = line.find('|', pos1);
                pesan.waktu = line.substr(pos1, pos2 - pos1);
                
                pos1 = pos2 + 1;
                pesan.status = line.substr(pos1);
                
                tambahPesanKeSesi(session, pesan);
            }
            file1.close();
        }
        
        ifstream file2(filename2.c_str());
        if (file2) {
            string line;
            while (getline(file2, line)) {
                Pesan pesan;
                int pos1 = 0, pos2 = 0;
                
                pos2 = line.find('|', pos1);
                pesan.id = line.substr(pos1, pos2 - pos1);
                
                pos1 = pos2 + 1;
                pos2 = line.find('|', pos1);
                pesan.pengirim = line.substr(pos1, pos2 - pos1);
                
                pos1 = pos2 + 1;
                pos2 = line.find('|', pos1);
                pesan.penerima = line.substr(pos1, pos2 - pos1);
                
                pos1 = pos2 + 1;
                pos2 = line.find('|', pos1);
                pesan.subjek = line.substr(pos1, pos2 - pos1);
                
                pos1 = pos2 + 1;
                pos2 = line.find('|', pos1);
                pesan.isi = line.substr(pos1, pos2 - pos1);
                
                pos1 = pos2 + 1;
                pos2 = line.find('|', pos1);
                pesan.waktu = line.substr(pos1, pos2 - pos1);
                
                pos1 = pos2 + 1;
                pesan.status = line.substr(pos1);
                
                tambahPesanKeSesi(session, pesan);
            }
            file2.close();
        }
    }

    int hitungPesanBelumDibaca(const string& user) {
        int count = 0;
        for (int i = 0; i < jumlahSesiChat; i++) {
            if (sesiChat[i].user1 == user || sesiChat[i].user2 == user) {
                for (int j = 0; j < sesiChat[i].jumlahPesan; j++) {
                    if (sesiChat[i].pesan[j].penerima == user && sesiChat[i].pesan[j].status == "UNREAD") {
                        count++;
                    }
                }
            }
        }
        return count;
    }

public:
    ChatSystem() {
        currentUser = "";
    }
    
    void setCurrentUser(const string& user) {
        currentUser = user;
    }

    void kirimPesan(const string& penerima, const string& subjek, const string& isi) {
        ChatSession* session = cariAtauBuatSesiChat(currentUser, penerima);
        
        Pesan pesan;
        pesan.id = generateMessageID();
        pesan.pengirim = currentUser;
        pesan.penerima = penerima;
        pesan.subjek = subjek;
        pesan.isi = isi;
        pesan.waktu = getCurrentTime();
        pesan.status = "UNREAD";
        
        tambahPesanKeSesi(session, pesan);
        simpanPesanKeFile(pesan);
        cout << "\nPesan berhasil dikirim ke " << penerima << "!\n";
    }

    void lihatInbox() {
        system("cls");
        int pesanBelumDibaca = hitungPesanBelumDibaca(currentUser);
        
        cout << "+================================================================================+\n";
        cout << "|                                INBOX PRIBADI                                   |\n";
        cout << "+================================================================================+\n";
        
        if (pesanBelumDibaca > 0) {
            cout << "|  Anda memiliki " << setw(2) << pesanBelumDibaca << " pesan belum dibaca" 
                 << setw(43) << " |\n";
            cout << "+--------------------------------------------------------------------------------+\n";
        }
        
        // Buat array dinamis untuk user list
        string* userList = new string[jumlahAkun];
        Pesan* lastMessages = new Pesan[jumlahAkun];
        int* unreadCounts = new int[jumlahAkun];
        int userCount = 0;
        
        for (int i = 0; i < jumlahSesiChat; i++) {
            if (sesiChat[i].user1 == currentUser || sesiChat[i].user2 == currentUser) {
                string otherUser = (sesiChat[i].user1 == currentUser) ? sesiChat[i].user2 : sesiChat[i].user1;
                
                if (sesiChat[i].jumlahPesan > 0) {
                    Pesan lastMsg = sesiChat[i].pesan[sesiChat[i].jumlahPesan - 1];
                    int unread = 0;
                    
                    for (int j = 0; j < sesiChat[i].jumlahPesan; j++) {
                        if (sesiChat[i].pesan[j].penerima == currentUser && sesiChat[i].pesan[j].status == "UNREAD") {
                            unread++;
                        }
                    }
                    
                    userList[userCount] = otherUser;
                    lastMessages[userCount] = lastMsg;
                    unreadCounts[userCount] = unread;
                    userCount++;
                }
            }
        }
        
        if (userCount == 0) {
            cout << "|                               Tidak ada pesan                                |\n";
        } else {
            cout << "+----+---------------------+---------------------------+------------+----------+\n";
            cout << "| No | Pengirim            | Subjek                    | Waktu      | Status   |\n";
            cout << "+----+---------------------+---------------------------+------------+----------+\n";
            
            for (int i = 0; i < userCount; i++) {
                string status = (unreadCounts[i] > 0) ? " BARU" : " DIBACA";
                string subjek = lastMessages[i].subjek;
                if (subjek.length() > 25) {
                    subjek = subjek.substr(0, 22) + "...";
                }
                
                cout << "| " << setw(2) << right << (i + 1) << " "
                     << "| " << setw(19) << left << userList[i]
                     << "| " << setw(25) << left << subjek
                     << "| " << setw(10) << left << lastMessages[i].waktu
                     << "| " << setw(8) << left << status << " |\n";
            }
            cout << "+----+---------------------+---------------------------+------------+----------+\n";
        }
        
        // Hapus array dinamis
        delete[] userList;
        delete[] lastMessages;
        delete[] unreadCounts;
    }

    void bacaPesan(const string& pengirim) {
        system("cls");
        muatPesanDariFile(currentUser, pengirim);
        ChatSession* session = cariAtauBuatSesiChat(currentUser, pengirim);
        
        if (session->jumlahPesan == 0) {
            cout << "\nTidak ada pesan dengan " << pengirim << ".\n";
            cout << "Tekan ENTER untuk kembali...";
            cin.get();
            return;
        }
        
        cout << "+================================================================================+\n";
        cout << "|                           PERCAKAPAN DENGAN " << setw(31) << left << pengirim << " |\n";
        cout << "+================================================================================+\n";
        
        for (int i = 0; i < session->jumlahPesan; i++) {
            Pesan* p = &session->pesan[i];
            string nama = (p->pengirim == currentUser) ? "Anda" : p->pengirim;
            
            cout << "\n[" << p->waktu << "] >> " << nama << ":\n";
            cout << "Subjek: " << p->subjek << "\n";
            cout << "Pesan : " << p->isi << "\n";
            cout << "--------------------------------------------------------------------------------\n";
            
            if (p->penerima == currentUser && p->status == "UNREAD") {
                p->status = "READ";
                ofstream file(("chat_" + p->pengirim + "_" + p->penerima + ".txt").c_str());
                if (file) {
                    for (int j = 0; j < session->jumlahPesan; j++) {
                        file << session->pesan[j].id << "|" << session->pesan[j].pengirim << "|" 
                             << session->pesan[j].penerima << "|" << session->pesan[j].subjek << "|"
                             << session->pesan[j].isi << "|" << session->pesan[j].waktu << "|"
                             << session->pesan[j].status << "\n";
                    }
                    file.close();
                }
            }
        }
        
        cout << "\nTekan ENTER untuk kembali...";
        cin.get();
    }

    void menuChat() {
        int pilih;
        do {
            system("cls");
            int pesanBelumDibaca = hitungPesanBelumDibaca(currentUser);
            
            cout << "+================================================================================+\n";
            cout << "|                                 SISTEM CHAT                                    |\n";
            cout << "+================================================================================+\n";
            if (pesanBelumDibaca > 0) {
                cout << "|NOTIFIKASI: " << setw(2) << pesanBelumDibaca << " pesan belum dibaca" 
                     << setw(2) << " |\n";
            }
            cout << "+--------------------------------------------------------------------------------+\n";
            cout << "| 1. Lihat Inbox                                                                 |\n";
            cout << "| 2. Kirim Pesan Baru                                                            |\n";
            cout << "| 3. Baca Percakapan                                                             |\n";
            cout << "| 4. Cari Pengguna                                                               |\n";
            cout << "| 5. Lihat Notifikasi                                                            |\n";
            cout << "| 0. Kembali                                                                     |\n";
            cout << "+--------------------------------------------------------------------------------+\n";
            cout << "Pilih :: ";
            
            int input;
            cin >> input;
            pilih = input;
            cin.ignore(1000, '\n');
            
            switch (pilih) {
                case 1:
                    lihatInbox();
                    cout << "\nTekan ENTER untuk kembali...";
                    cin.get();
                    break;
                case 2:
                    kirimPesanMenu();
                    break;
                case 3:
                    bacaPercakapanMenu();
                    break;
                case 4:
                    cariPenggunaMenu();
                    break;
                case 5:
                    tampilkanNotifikasiUntukUser(currentUser);
                    break;
                case 0:
                    return;
                default:
                    cout << "Pilihan tidak valid!\n";
                    cout << "Tekan ENTER untuk melanjutkan...";
                    cin.get();
            }
        } while (pilih != 0);
    }

private:
    void kirimPesanMenu() {
        system("cls");
        cout << "+================================================================================+\n";
        cout << "|                               KIRIM PESAN BARU                                 |\n";
        cout << "+================================================================================+\n";
        
        cout << "\nDaftar Pengguna:\n";
        cout << "+------------------+----------------------+-----------------+\n";
        cout << "| Username         | Nama                 | Role           |\n";
        cout << "+------------------+----------------------+-----------------+\n";
        
        int count = 0;
        for (int i = 0; i < jumlahAkun; i++) {
            if (daftarAkun[i].username != currentUser) {
                cout << "| " << setw(16) << left << daftarAkun[i].username
                     << "| " << setw(20) << left << daftarAkun[i].nama
                     << "| " << setw(15) << left << daftarAkun[i].role << "|\n";
                count++;
            }
        }
        
        if (count == 0) {
            cout << "|                   Tidak ada pengguna lain                      |\n";
        }
        cout << "+------------------+----------------------+-----------------+\n";
        
        string penerima, subjek, isi;
        cout << "\nMasukkan username penerima: ";
        getline(cin, penerima);
        
        bool penerimaAda = false;
        for (int i = 0; i < jumlahAkun; i++) {
            if (daftarAkun[i].username == penerima && daftarAkun[i].username != currentUser) {
                penerimaAda = true;
                break;
            }
        }
        
        if (!penerimaAda) {
            cout << " Username penerima tidak ditemukan!\n";
            cout << "Tekan ENTER untuk kembali...";
            cin.get();
            return;
        }
        
        cout << "Masukkan subjek pesan: ";
        getline(cin, subjek);
        cout << "Masukkan isi pesan: ";
        getline(cin, isi);
        
        if (subjek.empty() || isi.empty()) {
            cout << " Subjek dan isi pesan tidak boleh kosong!\n";
        } else {
            kirimPesan(penerima, subjek, isi);
        }
        
        cout << "Tekan ENTER untuk kembali...";
        cin.get();
    }

    void bacaPercakapanMenu() {
        system("cls");
        cout << "+================================================================================+\n";
        cout << "|                             BACA PERCAKAPAN                                    |\n";
        cout << "+================================================================================+\n";
        
        string pengirim;
        cout << "Masukkan username pengirim yang ingin dilihat percakapannya: ";
        getline(cin, pengirim);
        
        bacaPesan(pengirim);
    }

    void cariPenggunaMenu() {
        system("cls");
        cout << "+================================================================================+\n";
        cout << "|                               CARI PENGGUNA                                    |\n";
        cout << "+================================================================================+\n";
        
        string keyword;
        cout << "Masukkan nama atau username yang dicari: ";
        getline(cin, keyword);
        
        cout << "\nHasil Pencarian:\n";
        cout << "+------------------+----------------------+-----------------+\n";
        cout << "| Username         | Nama                 | Role           |\n";
        cout << "+------------------+----------------------+-----------------+\n";
        
        bool ditemukan = false;
        for (int i = 0; i < jumlahAkun; i++) {
            if (daftarAkun[i].username != currentUser && 
                (daftarAkun[i].username.find(keyword) != string::npos || 
                 daftarAkun[i].nama.find(keyword) != string::npos)) {
                cout << "| " << setw(16) << left << daftarAkun[i].username
                     << "| " << setw(20) << left << daftarAkun[i].nama
                     << "| " << setw(15) << left << daftarAkun[i].role << "|\n";
                ditemukan = true;
            }
        }
        
        if (!ditemukan) {
            cout << "|                   Tidak ada hasil ditemukan                   |\n";
        }
        cout << "+------------------+----------------------+-----------------+\n";
        
        cout << "\nTekan ENTER untuk kembali...";
        cin.get();
    }
};

// =====================================================================
// CLASS PERPUSTAKAAN
// =====================================================================
class Perpustakaan {
private:
    long long int mahasiswaNIM;
    
    string getCurrentDate() {
        return "01-12-2025";
    }

public:
    Perpustakaan() {
        mahasiswaNIM = 0;
    }
    
    void setMahasiswaNIM(long long int nim) {
        mahasiswaNIM = nim;
    }

    bool pinjamBuku(const string& kode) {
        BukuMaster* buku = cariBukuByKode(kode);
        if (!buku || buku->stok <= 0) {
            return false;
        }
        
        for (int i = 0; i < jumlahPinjaman; i++) {
            if (daftarPinjaman[i].nim == mahasiswaNIM && daftarPinjaman[i].kode == kode) {
                return false;
            }
        }
        
        buku->stok--;
        
        BukuPinjam pinjaman;
        pinjaman.nim = mahasiswaNIM;
        pinjaman.kode = kode;
        pinjaman.judul = buku->judul;
        pinjaman.tglPinjamStr = getCurrentDate();
        
        tambahPinjaman(pinjaman);
        
        simpanPinjamanKeFile();
        simpanBukuKeFile();
        
        return true;
    }

    void simpanBukuKeFile() {
        ofstream file("buku.txt");
        for (int i = 0; i < jumlahBuku; i++) {
            file << daftarBuku[i].kode << "|" << daftarBuku[i].judul << "|" << daftarBuku[i].stok << "\n";
        }
        file.close();
    }

    void simpanPinjamanKeFile() {
        ofstream file("pinjaman.txt");
        for (int i = 0; i < jumlahPinjaman; i++) {
            file << daftarPinjaman[i].nim << "|" << daftarPinjaman[i].kode << "|" 
                 << daftarPinjaman[i].judul << "|" << daftarPinjaman[i].tglPinjamStr << "\n";
        }
        file.close();
    }

    bool kembalikanBuku(const string& kode) {
        int index = -1;
        for (int i = 0; i < jumlahPinjaman; i++) {
            if (daftarPinjaman[i].nim == mahasiswaNIM && daftarPinjaman[i].kode == kode) {
                index = i;
                break;
            }
        }
        
        if (index == -1) {
            return false;
        }
        
        BukuMaster* buku = cariBukuByKode(kode);
        if (buku) {
            buku->stok++;
        }
        
        hapusPinjamanByIndex(index);
        
        simpanBukuKeFile();
        simpanPinjamanKeFile();
        
        int denda = hitungDenda("01-12-2025");
        if (denda > 0) {
            cout << "\nDENDA: Rp " << denda << endl;
        }
        
        return true;
    }

    int hitungDenda(const string& tglPinjamStr) {
        const int BATAS_HARI = 7;
        const int DENDA_PER_HARI = 1000;
        
        int selisihHari = 5;
        
        if (selisihHari > BATAS_HARI) {
            int hariTelat = selisihHari - BATAS_HARI;
            return hariTelat * DENDA_PER_HARI;
        }
        return 0;
    }

    void tampilkanDaftarBukuTersedia() {
        if (jumlahBuku == 0) {
            cout << "\nBelum ada buku yang ditambahkan oleh Admin.\n";
            return;
        }
        
        cout << "\n+=======================================================+\n";
        cout << "|                 DAFTAR BUKU TERSEDIA                  |\n";
        cout << "+-----------+-------------------------------+-----------+\n";
        cout << "| Kode Buku | Judul Buku                    | Stok      |\n";
        cout << "+-----------+-------------------------------+-----------+\n";
        
        for (int i = 0; i < jumlahBuku; i++) {
            cout << "| " << setw(10) << left << daftarBuku[i].kode
                 << "| " << setw(30) << left << daftarBuku[i].judul
                 << "| " << setw(9) << left << daftarBuku[i].stok << "|\n";
        }
        cout << "+-----------+-------------------------------+-----------+\n";
    }

    void cariBuku(const string& kriteria) {
        if (jumlahBuku == 0) {
            cout << "\nBelum ada buku di database.\n";
            return;
        }

        cout << "\n+=======================================================+\n";
        cout << "|                    HASIL PENCARIAN BUKU               |\n";
        cout << "+-----------+-------------------------------+-----------+\n";
        cout << "| Kode Buku | Judul Buku                    | Stok      |\n";
        cout << "+-----------+-------------------------------+-----------+\n";
        
        bool ketemu = false;
        for (int i = 0; i < jumlahBuku; i++) {
            if (daftarBuku[i].kode == kriteria || daftarBuku[i].judul.find(kriteria) != string::npos) {
                cout << "| " << setw(10) << left << daftarBuku[i].kode
                     << "| " << setw(30) << left << daftarBuku[i].judul
                     << "| " << setw(9) << left << daftarBuku[i].stok << "|\n";
                ketemu = true;
            }
        }

        if (!ketemu) {
            cout << "|            Buku tidak ditemukan di database.          |\n";
        }
        cout << "+-----------+-------------------------------+-----------+\n";
    }

    void tampilkanBukuDipinjam() {
        cout << "\n+================================================================================+\n";
        cout << "|                           DAFTAR BUKU DIPINJAM                                  |\n";
        cout << "+-----------+-------------------------------+---------------+----------------+\n";
        cout << "| Kode Buku | Judul Buku                    | Tgl Pinjam    | Denda          |\n";
        cout << "+-----------+-------------------------------+---------------+----------------+\n";

        bool adaPinjaman = false;
        for (int i = 0; i < jumlahPinjaman; i++) {
            if (daftarPinjaman[i].nim == mahasiswaNIM) {
                int denda = hitungDenda(daftarPinjaman[i].tglPinjamStr);
                string dendaStr = "Rp " + to_string(denda);
                
                cout << "| " << setw(10) << left << daftarPinjaman[i].kode
                     << "| " << setw(30) << left << daftarPinjaman[i].judul
                     << "| " << setw(13) << left << daftarPinjaman[i].tglPinjamStr
                     << "| " << setw(14) << left << dendaStr << "|\n";
                adaPinjaman = true;
            }
        }

        if (!adaPinjaman) {
            cout << "|                     Belum ada buku yang dipinjam                          |\n";
        }
        cout << "+-----------+-------------------------------+---------------+----------------+\n";
    }

    int hitungTotalDenda() {
        int totalDenda = 0;
        for (int i = 0; i < jumlahPinjaman; i++) {
            if (daftarPinjaman[i].nim == mahasiswaNIM) {
                totalDenda += hitungDenda(daftarPinjaman[i].tglPinjamStr);
            }
        }
        return totalDenda;
    }

    bool adaPinjaman() {
        for (int i = 0; i < jumlahPinjaman; i++) {
            if (daftarPinjaman[i].nim == mahasiswaNIM) {
                return true;
            }
        }
        return false;
    }

    void lihatKoleksiBukuAdmin() {
        if (jumlahBuku == 0) {
            cout << "|          Belum ada buku di database. Silakan tambah.       |\n";
        } else {
            for (int i = 0; i < jumlahBuku; i++) {
                cout << "| " << setw(10) << left << daftarBuku[i].kode
                     << "| " << setw(30) << left << daftarBuku[i].judul
                     << "| " << setw(9) << left << daftarBuku[i].stok << "|\n";
            }
        }
    }

    bool tambahBukuBaruAdmin(const string& kode, const string& judul, int stok) {
        if (cariBukuByKode(kode) != nullptr) {
            return false;
        }
        
        BukuMaster buku;
        buku.kode = kode;
        buku.judul = judul;
        buku.stok = stok;
        
        tambahBuku(buku);
        simpanBukuKeFile();
        return true;
    }

    bool hapusBukuAdmin(const string& kodeHapus) {
        for (int i = 0; i < jumlahBuku; i++) {
            if (daftarBuku[i].kode == kodeHapus) {
                hapusBuku(i);
                simpanBukuKeFile();
                return true;
            }
        }
        return false;
    }
};

// =====================================================================
// CLASS PERKULIAHAN
// =====================================================================
class Perkuliahan {
private:
    long long int spp, BR, BBM, BA, BTK, BBV, DPPT;
    long long int totalBayar;
    
    long long int mahasiswaNIM;
    string mahasiswaNama;
    string mahasiswaProdi;
    
    const int MAX_SKS = 24;

    double konversiNilai(string n) {
        if (n == "A") return 4.0;
        if (n == "A-") return 3.7;
        if (n == "B+") return 3.3;
        if (n == "B") return 3.0;
        if (n == "B-") return 2.7;
        if (n == "C+") return 2.3;
        if (n == "C") return 2.0;
        if (n == "D") return 1.0;
        if (n == "E") return 0.0;
        return 0.0; 
    }

public:
    Perkuliahan() { 
        spp = 2750000; 
        BR = 300000; 
        BBM = 1500000; 
        BA = 75000;
        BTK = 275000; 
        BBV = 4400000; 
        DPPT = 5250000;
        totalBayar = 0;
        mahasiswaNIM = 0;
        mahasiswaNama = "";
        mahasiswaProdi = "";
    }
    
    void setMahasiswaInfo(const string& nama, long long int nim, const string& prodi){
        mahasiswaNama = nama;
        mahasiswaNIM = nim;
        mahasiswaProdi = prodi;
    }

    void tampilkanInfoMahasiswa(){
        if (!mahasiswaNama.empty() || mahasiswaNIM != 0 || !mahasiswaProdi.empty()){
            cout<<"\nNama           : " << (mahasiswaNama.empty() ? "-" : mahasiswaNama)
                <<"\nNIM            : " << (mahasiswaNIM == 0 ? "-" : to_string(mahasiswaNIM))
                <<"\nProgram Studi  : " << (mahasiswaProdi.empty() ? "-" : mahasiswaProdi)
                <<"\n\n";
        }
    }

    void menuPerkuliahan(){
        int pilih;
        do{
            cout << "\n+-------------> MENU PERKULIAHAN <-------------+\n";
            cout << "1. Jadwal Kuliah\n";
            cout << "2. KHS (Kartu Hasil Studi)\n";
            cout << "3. KRS (Kartu Rencana Studi)\n";
            cout << "4. Transkip Nilai (IPK)\n";
            cout << "5. Info Pembayaran\n";
            cout << "6. Virtual Pembayaran\n";
            cout << "7. Tugas akhir\n";
            cout << "8. Presensi Kuliah\n";
            cout << "0. Kembali\n";
            cout << endl;
            cout<<"Pilih :: ";
            
            int input;
            cin >> input;
            pilih = input;
            cin.ignore(1000, '\n');
            
            switch (pilih){
                case 1:
                    tampilkanJadwal();
                    break;
                case 2:
                    tampilkanKHS(); 
                    break;
                case 3:
                    menuKRS(); 
                    break;
                case 4:
                    tampilkanTranskip(); 
                    break;
                case 5:
                    infoPembayaran(); 
                    break;
                case 6:
                    pembayaran(); 
                    break;
                case 7:
                    Tugas_Akhir(); 
                    break;
                case 8:
                    menuPresensi();
                    break;
                case 0:
                    cout << "Kembali ke portal utama...\n"; 
                    break; 
                default: 
                    cout << "Pilihan tidak valid!\n";
                    break; 
            }
        } while (pilih != 0);
    }
    
    void tampilkanTabelMatkul(){
        system("cls"); 
        tampilkanInfoMahasiswa();
        
        if (jumlahMatkul == 0) {
            inisialisasiTabelMatkul();
        }
        
        cout<<"+----------------------------------------------------------------------+\n";
        cout<<"|\t\tDAFTAR MATA KULIAH TERSEDIA\t\t\t   |\n";
        cout<<"+----------------------------------------------------------------------+\n";
        cout<<"|No"<< left<< setw(11) <<"| Kode"
            <<setw(31) <<"| Nama Mata Kuliah"
            <<setw(6)  <<"|SKS"
            <<setw(9)  <<"|Semester" << "|"<< endl;
        cout<<"|--+----------+------------------------------+-----+---------|\n";
        
        for (int i = 0; i < jumlahMatkul; i++) {
            cout<<"|"<< left << setw(2) << (i+1) <<"|"<< setw(10)<< daftarMatkul[i].kode<<"|"
                << setw(30)<< daftarMatkul[i].nama<<"|"
                << setw(5) << daftarMatkul[i].sks<<"|"
                << setw(9) << daftarMatkul[i].semester<<"|"<< endl;
            cout<<"|--+----------+------------------------------+-----+---------|\n";
        }
    }
    
    void inisialisasiTabelMatkul() {
        if (jumlahMatkul > 0) return;
        
        string kode[] = {"IF101", "IF102", "IF103", "IF104", "IF105", "IF106", "IF107", "IF108", "IF109"};
        string nama[] = {"Algoritma dan Pemrograman", "Struktur Data", "Basis Data", 
                        "Pemrograman Berorientasi Objek", "Jaringan Komputer", "Sistem Operasi",
                        "Pemrograman Web", "Kecerdasan Buatan", "Rekayasa Perangkat Lunak"};
        int sks[] = {3, 3, 3, 3, 2, 3, 3, 3, 3};
        string semester[] = {"1", "1", "2", "2", "3", "3", "4", "4", "5"};
        string prodi[] = {"Informatika", "Informatika", "Informatika", "Informatika", "Informatika",
                         "Informatika", "Informatika", "Informatika", "Informatika"};
        
        for (int i = 0; i < 9; i++) {
            Matkul m;
            m.kode = kode[i];
            m.nama = nama[i];
            m.sks = sks[i];
            m.semester = semester[i];
            m.prodi = prodi[i];
            
            tambahMatkulGlobal(m);
        }
    }
    
    void tambahMatkul(){
        system("cls"); 
        string kode;
        tampilkanTabelMatkul(); 
        cout<<"\nMasukkan kode mata kuliah yang ingin ditambahkan: ";
        getline(cin, kode);
        
        Matkul* matkulDipilih = nullptr;
        for (int i = 0; i < jumlahMatkul; i++) {
            if (daftarMatkul[i].kode == kode) {
                matkulDipilih = &daftarMatkul[i];
                break;
            }
        }

        if (!matkulDipilih) {
            cout<<"Kode tidak ditemukan di daftar mata kuliah.\n";
            return;
        }
        
        int sksSaatIni = 0;
        for (int i = 0; i < jumlahKRS; i++) {
            if (daftarKRS[i].nim == mahasiswaNIM) {
                sksSaatIni += daftarKRS[i].sks;
            }
        }
        
        int sksBaru = matkulDipilih->sks;
        
        cout << "\nTotal SKS saat ini: " << sksSaatIni << endl;
        cout << "SKS Mata Kuliah baru: " << sksBaru << endl;
        
        if (sksSaatIni + sksBaru > MAX_SKS) {
            cout << "\nGAGAL: Total SKS akan melebihi batas maksimum (" << MAX_SKS << " SKS).\n";
            cout << "Total SKS Anda jika ditambah matkul ini: " << (sksSaatIni + sksBaru) << " SKS.\n";
            return;
        }

        for (int i = 0; i < jumlahKRS; i++) {
            if (daftarKRS[i].nim == mahasiswaNIM && daftarKRS[i].kodeMatkul == kode) {
                cout<<"Mata kuliah ini sudah diambil sebelumnya!\n";
                return;
            }
        }

        KRSMahasiswa baru;
        baru.nim = mahasiswaNIM;
        baru.kodeMatkul = matkulDipilih->kode;
        baru.namaMatkul = matkulDipilih->nama;
        baru.sks = matkulDipilih->sks;
        
        tambahKRS(baru);
        cout<<"Mata kuliah "<< matkulDipilih->nama<<" berhasil ditambahkan ke KRS.\n";
        cout << "Total SKS baru Anda: " << (sksSaatIni + sksBaru) << " SKS.\n";
        
        simpanKRSToFile();
        // SIMPAN KRS KE FILE PER MAHASISWA (TAMBAHAN)
        simpanKRSMahasiswaKeFile(mahasiswaNIM);
    }
    
    void hapusMatkul(){
        system("cls"); 
        string kode;
        
        int countKRS = 0;
        for (int i = 0; i < jumlahKRS; i++) {
            if (daftarKRS[i].nim == mahasiswaNIM) {
                countKRS++;
            }
        }
        
        if(countKRS == 0) {
            cout << "\nKRS masih kosong, tidak ada yang bisa dihapus.\n";
            return;
        }
        
        tampilkanKRSInternal(); 
        cout<<"\nMasukkan kode mata kuliah yang ingin dihapus: ";
        getline(cin, kode);
        
        int index = -1;
        for (int i = 0; i < jumlahKRS; i++) {
            if (daftarKRS[i].nim == mahasiswaNIM && daftarKRS[i].kodeMatkul == kode) {
                index = i;
                break;
            }
        }
        
        if (index == -1) {
            cout<<"Kode mata kuliah tidak ditemukan di KRS.\n";
            return;
        }
        
        cout<<"Mata kuliah "<< daftarKRS[index].namaMatkul<<" dihapus dari KRS.\n";
        hapusKRSByIndex(index);
        
        int totalSKS = 0;
        for (int i = 0; i < jumlahKRS; i++) {
            if (daftarKRS[i].nim == mahasiswaNIM) {
                totalSKS += daftarKRS[i].sks;
            }
        }
        
        simpanKRSToFile();
        // SIMPAN KRS KE FILE PER MAHASISWA (TAMBAHAN)
        simpanKRSMahasiswaKeFile(mahasiswaNIM);
        cout << "Total SKS sekarang: " << totalSKS << " SKS.\n";
    }

    void simpanKRSToFile() {
        ofstream file("krs.txt");
        for (int i = 0; i < jumlahKRS; i++) {
            file << daftarKRS[i].nim << "|" << daftarKRS[i].kodeMatkul << "|" 
                 << daftarKRS[i].namaMatkul << "|" << daftarKRS[i].sks << "\n";
        }
        file.close();
    }

    void tampilkanKRSInternal() {
        int count = 0;
        for (int i = 0; i < jumlahKRS; i++) {
            if (daftarKRS[i].nim == mahasiswaNIM) {
                count++;
            }
        }
        
        if (count == 0) {
            cout << "\nBelum ada mata kuliah yang diambil.\n";
            return;
        }
        
        cout << "\n+================================================================+\n";
        cout << "|                     DAFTAR KRS SAAT INI                        |\n";
        cout << "+====+============+=========================================+====+\n";
        cout << "| No | Kode       | Nama Mata Kuliah                        |SKS |\n";
        cout << "+====+============+=========================================+====+\n";
        
        int nomor = 1;
        int total = 0;
        for (int i = 0; i < jumlahKRS; i++) {
            if (daftarKRS[i].nim == mahasiswaNIM) {
                cout << "| " << setw(2) << right << nomor << " "
                     << "| " << setw(10) << left << daftarKRS[i].kodeMatkul
                     << "| " << setw(41) << left << daftarKRS[i].namaMatkul
                     << "| " << setw(3) << right << daftarKRS[i].sks << "|\n";
                cout << "+----+------------+-----------------------------------------+----+\n";
                total += daftarKRS[i].sks;
                nomor++;
            }
        }
        cout<< "| " << setw(58) << left << "TOTAL SKS"
            << "| " << setw(3) << right << total << "|\n";
        cout<< "+================================================================+\n";
    }

    void tampilkanKRS() {
        system("cls"); 
        tampilkanInfoMahasiswa();
        
        int count = 0;
        for (int i = 0; i < jumlahKRS; i++) {
            if (daftarKRS[i].nim == mahasiswaNIM) {
                count++;
            }
        }

        if (count == 0) {
            cout << "\nBelum ada mata kuliah yang diambil.\n";
            cout << "\nTekan ENTER untuk kembali...";
            cin.get(); 
            return;
        }
        tampilkanKRSInternal(); 
        cout << "\nTekan ENTER untuk kembali...";
        cin.get();
    }

    void cetakKRSKeFile() {
        system("cls"); 
        int count = 0;
        for (int i = 0; i < jumlahKRS; i++) {
            if (daftarKRS[i].nim == mahasiswaNIM) {
                count++;
            }
        }
        
        if (count == 0) {
            cout << "\nKRS kosong, tidak ada yang bisa dicetak.\n";
            cout << "\nTekan ENTER untuk kembali...";
            cin.get();
            return;
        }

        string namaFile = "Cetak_KRS_" + to_string(mahasiswaNIM) + ".txt";
        ofstream file(namaFile.c_str());
        
        cout << "Mencetak KRS ke file " << namaFile << "...\n";

        file << "+=================================================================+\n";
        file << "|                       KARTU RENCANA STUDI                       |\n";
        file << "+=================================================================+\n\n";
        file << "Nama           : " << mahasiswaNama << "\n";
        file << "NIM            : " << mahasiswaNIM << "\n";
        file << "Program Studi  : " << mahasiswaProdi << "\n\n";
        
        file << "+====+============+=========================================+====+\n";
        file << "| No | Kode       | Nama Mata Kuliah                        |SKS |\n";
        file << "+====+============+=========================================+====+\n";

        int nomor = 1;
        int total = 0;
        for (int i = 0; i < jumlahKRS; i++) {
            if (daftarKRS[i].nim == mahasiswaNIM) {
                file << "| " << setw(2) << right << nomor << " "
                     << "| " << setw(10) << left << daftarKRS[i].kodeMatkul
                     << "| " << setw(41) << left << daftarKRS[i].namaMatkul
                     << "| " << setw(3) << right << daftarKRS[i].sks << "|\n";
                file << "+----+------------+-----------------------------------------+----+\n";
                total += daftarKRS[i].sks;
                nomor++;
            }
        }
        file << "| " << setw(58) << left << "TOTAL SKS"
             << "| " << setw(3) << right << total << "|\n";
        file << "+=================================================================+\n\n";
        
        file << "Dicetak pada: 01-12-2025 10:30:00\n";
        
        file.close();

        cout << "\nKRS berhasil dicetak ke file " << namaFile << "!\n";
        cout << "\nTekan ENTER untuk kembali...";
        cin.get();
    }
    
    void menuKRS(){
        int pilih;
        do {
            cout << "\n+-------------> MENU KRS <-------------+\n";
            cout << "1. Tambah Mata Kuliah\n";
            cout << "2. Hapus Mata Kuliah\n";
            cout << "3. Lihat KRS\n";
            cout << "4. Cetak KRS ke File (.txt)\n";
            cout << "5. Kembali\n";
            cout << "+--------------------------------------+\n";
            cout << endl;
            
            cout<<"Pilih :: ";
            int input;
            cin >> input;
            pilih = input;
            cin.ignore(1000, '\n');

            switch(pilih){
                case 1: 
                    tambahMatkul(); 
                    break;
                case 2: 
                    hapusMatkul(); 
                    break;
                case 3: 
                    tampilkanKRS(); 
                    break;
                case 4: 
                    cetakKRSKeFile(); 
                    break;
                case 5: 
                    break; 
                default: cout << "Pilihan tidak valid!\n"; break; 
            }
        } while (pilih != 5);
    }
    
    void tampilkanJadwal(){
        system("cls"); 
        tampilkanInfoMahasiswa();
        
        int count = 0;
        for (int i = 0; i < jumlahKRS; i++) {
            if (daftarKRS[i].nim == mahasiswaNIM) {
                count++;
            }
        }
        
        if(count == 0){
            cout << "\nBelum ada mata kuliah di KRS untuk ditampilkan jadwalnya.\n";
            cout << "\nTekan ENTER untuk kembali...";
            cin.get(); 
            return;
        }
        
        const char* hari[5] = {"Senin", "Selasa", "Rabu", "Kamis", "Jumat"};
        cout<<"+===========================================================+\n";
        cout<<"|\t\t\tJADWAL KULIAH\t\t\t    |\n";
        cout<<"+===========================================================+\n";
        cout<< left<< setw(10)<< "|Kode"
            << setw(33)<<"|Mata Kuliah"
            << setw(10)<<"|Hari"
            << setw(10)<<"|Jam      |" << endl;
        cout<<"+---------+--------------------------------+---------+---------+\n"; 
        
        for (int i = 0; i < jumlahKRS; i++) {
            if (daftarKRS[i].nim == mahasiswaNIM) {
                string h = hari[i % 5];
                int jam = 7 + (i % 10); 
                
                string jamStr;
                if (jam < 10) jamStr = "0" + to_string(jam);
                else jamStr = to_string(jam);
                
                string jamSelesaiStr;
                int jamSelesai = jam + daftarKRS[i].sks;
                if (jamSelesai < 10) jamSelesaiStr = "0" + to_string(jamSelesai);
                else jamSelesaiStr = to_string(jamSelesai);
                
                cout<<"|"<< left<< setw(9)<< daftarKRS[i].kodeMatkul<<"|"
                    << setw(32)<< daftarKRS[i].namaMatkul<<"|"
                    << setw(9)<< h <<"|"
                    << setw(9)<< jamStr + ":00-" + jamSelesaiStr + ":00" <<"|"<< endl;
                cout<< "+---------+--------------------------------+---------+---------+\n";
            }
        }
        cout << "\nTekan ENTER untuk kembali...";
        cin.get();
    }

    void tampilkanJadwalKuliahMahasiswa(long long int nim) {
        system("cls");
        cout << "\n+================================================================================+\n";
        cout << "|                         JADWAL KULIAH MAHASISWA                             |\n";
        cout << "+===========+===============================================+========+===========+\n";
        cout << "| Hari      | Mata Kuliah                                  | Jam    | Ruangan   |\n";
        cout << "+===========+===============================================+========+===========+\n";
        
        bool adaJadwal = false;
        for (int i = 0; i < jumlahKRS; i++) {
            if (daftarKRS[i].nim == nim) {
                for (int j = 0; j < jumlahJadwal; j++) {
                    if (daftarJadwal[j].kodeMatkul == daftarKRS[i].kodeMatkul) {
                        adaJadwal = true;
                        string jam = daftarJadwal[j].jamMulai + "-" + daftarJadwal[j].jamSelesai;
                        cout << "| " << setw(9) << left << daftarJadwal[j].hari
                             << "| " << setw(45) << left << daftarKRS[i].namaMatkul
                             << "| " << setw(6) << left << jam
                             << " | " << setw(7) << left << daftarJadwal[j].ruangan << " |\n";
                        cout << "+-----------+-----------------------------------------------+--------+-----------+\n";
                    }
                }
            }
        }
        
        if (!adaJadwal) {
            cout << "|               Belum ada jadwal kuliah untuk semester ini               |\n";
        }
        
        cout << "\nTekan ENTER untuk kembali...";
        cin.get();
    }

    void tampilkanKHS(){
        system("cls"); 
        tampilkanInfoMahasiswa();
        
        // GUNAKAN FUNGSI BARU UNTUK MENAMPILKAN KHS SESUAI KRS
        tampilkanKHSSesuaiKRS(mahasiswaNIM);
    }
    
    void tampilkanTranskip(){
        system("cls"); 
        tampilkanInfoMahasiswa();
        
        cout<<"\n+===================== TRANSKIP NILAI ======================+\n";
        cout<< left << setw(10)<<"| Kode"
            << setw(33)<<"| Mata Kuliah"
            << setw(6) <<"| SKS"
            << setw(8) <<"| Nilai" << "|\n";
        cout<<"+---------+--------------------------------+-----+-------+\n";
        
        bool adaData = false;
        int totalSKS = 0;
        double totalBobot = 0;
        
        for (int i = 0; i < jumlahNilai; i++) {
            if (daftarNilai[i].nim == mahasiswaNIM) {
                adaData = true;
                double bobot = konversiNilai(daftarNilai[i].nilai);
                
                totalSKS += daftarNilai[i].sks;
                totalBobot += bobot * daftarNilai[i].sks;
                
                cout<< "|"<< left<< setw(9) << daftarNilai[i].kodeMatkul << "|"
                    << setw(32) << daftarNilai[i].namaMatkul << "|"
                    << setw(5)  << to_string(daftarNilai[i].sks) << "|"
                    << setw(7) << daftarNilai[i].nilai << "|\n";
            }
        }
        
        if (!adaData) {
            cout << "|              Belum ada data nilai                            |\n";
        }
        cout<<"+---------+--------------------------------+-----+-------+\n";
        
        if(totalSKS > 0){
            double ipk = totalBobot / totalSKS;
            cout << "| Total SKS : " << setw(44) << left << totalSKS << "|\n";
            cout << "| IPK       : " << setw(44) << left << fixed << setprecision(2) << ipk << "|\n";
            cout << "+=========================================================+\n";
        }
        
        cout << "\nTekan ENTER untuk kembali...";
        cin.get();
    }

    bool tampilkanKRSUntukDosen(long long int nimMahasiswa) {
        int count = 0;
        for (int i = 0; i < jumlahKRS; i++) {
            if (daftarKRS[i].nim == nimMahasiswa) {
                count++;
            }
        }
        
        if (count == 0) {
            cout << "\nMahasiswa ini belum mengambil KRS.\n";
            return false;
        }
        
        string namaMahasiswa = "";
        for (int i = 0; i < jumlahAkun; i++) {
            if (daftarAkun[i].nim == nimMahasiswa) {
                namaMahasiswa = daftarAkun[i].nama;
                break;
            }
        }
        
        cout << "\nKRS Mahasiswa: " << namaMahasiswa << " (" << nimMahasiswa << ")\n";
        cout << "+====+============+=========================================+====+\n";
        cout << "| No | Kode       | Nama Mata Kuliah                        |SKS |\n";
        cout << "+====+============+=========================================+====+\n";
        
        int nomor = 1;
        for (int i = 0; i < jumlahKRS; i++) {
            if (daftarKRS[i].nim == nimMahasiswa) {
                cout<<"| "<< setw(2) << right<< nomor << " "
                    <<"| "<< setw(10) << left<< daftarKRS[i].kodeMatkul
                    <<"| "<< setw(41) << left<< daftarKRS[i].namaMatkul
                    <<"| "<< setw(3) << right<< daftarKRS[i].sks << "|\n";
                nomor++;
            }
        }
        cout << "+----+------------+-----------------------------------------+----+\n";
        return true;
    }

    void simpanNilai(long long int nimMahasiswa, string kodeInput, string nilaiInput) {
        string namaMatkul = "";
        int sksMatkul = 0;
        bool matkulValid = false;
        
        for (int i = 0; i < jumlahKRS; i++) {
            if (daftarKRS[i].nim == nimMahasiswa && daftarKRS[i].kodeMatkul == kodeInput) {
                namaMatkul = daftarKRS[i].namaMatkul;
                sksMatkul = daftarKRS[i].sks;
                matkulValid = true;
                break;
            }
        }

        if (!matkulValid) {
            cout << "Error: Mahasiswa tidak mengambil matkul ini.\n";
            return;
        }

        int index = -1;
        for (int i = 0; i < jumlahNilai; i++) {
            if (daftarNilai[i].nim == nimMahasiswa && daftarNilai[i].kodeMatkul == kodeInput) {
                index = i;
                break;
            }
        }

        if (index != -1) {
            daftarNilai[index].nilai = nilaiInput;
            cout << "\nNilai untuk " << namaMatkul << " berhasil diupdate!\n";
        } else {
            NilaiMahasiswa nilaiBaru;
            nilaiBaru.nim = nimMahasiswa;
            nilaiBaru.kodeMatkul = kodeInput;
            nilaiBaru.namaMatkul = namaMatkul;
            nilaiBaru.sks = sksMatkul;
            nilaiBaru.nilai = nilaiInput;
            
            if (jumlahNilai >= kapasitasNilai) {
                kapasitasNilai = kapasitasNilai == 0 ? 10 : kapasitasNilai * 2;
                NilaiMahasiswa* temp = new NilaiMahasiswa[kapasitasNilai];
                for (int i = 0; i < jumlahNilai; i++) {
                    temp[i] = daftarNilai[i];
                }
                delete[] daftarNilai;
                daftarNilai = temp;
            }
            daftarNilai[jumlahNilai++] = nilaiBaru;
            cout << "\nNilai untuk " << namaMatkul << " berhasil disimpan!\n";
        }

        simpanNilaiToFile();
    }

    void simpanNilaiToFile() {
        ofstream file("nilai.txt");
        for (int i = 0; i < jumlahNilai; i++) {
            file << daftarNilai[i].nim << "|" << daftarNilai[i].kodeMatkul << "|" 
                 << daftarNilai[i].namaMatkul << "|" << daftarNilai[i].sks << "|"
                 << daftarNilai[i].nilai << "\n";
        }
        file.close();
    }
    
    void Tugas_Akhir(){
        int pilih;
        system("cls");
        do{
            cout<<"1. Tugas Akhir\n";
            cout<<"2. Thesis\n";
            cout<<"3. Keluar\n";
            cout<<"Pilih:: "; 
            int input;
            cin >> input;
            pilih = input;
            cin.ignore(1000, '\n');
            switch(pilih){
            case 1:
                system("cls");
                cout<<"+----+------+------------+-------+-------+-----------+-----------+--------+----------+\n";
                cout<< left<<"| No "<< setw(5)<<"| Aksi "<< setw(5)<<"| Tgl.Daftar "
                    << setw(5)<<"| Topik "<< setw(5)<<"| Judul "<< setw(5)<<"| Deskripsi "
                    << setw(5)<<"| Pembimbing "<< setw(5)<<"| Status "<< setw(5)<<"| Informasi |"<< endl;
                cout<<"|------------------------------------------------------------------------------------|\n";
                cout<<"|\t\t\t\t -- Belum ada data --\t\t\t\t     |\n";
                cout<<"+------------------------------------------------------------------------------------+\n";
                break;
            case 2:
                system("cls");
                cout<<"+----+------+------------+-------+-------+-----------+-----------+--------+----------+\n";
                cout<< left<<"| No "<< setw(5)<<"| Aksi "<< setw(5)<<"| Tgl.Daftar "
                    << setw(5)<<"| Topik "<< setw(5)<<"| Judul "<< setw(5)<<"| Deskripsi "
                    << setw(5)<<"| Pembimbing "<< setw(5)<<"| Status "<< setw(5)<<"| Informasi |"<< endl;
                cout<<"|------------------------------------------------------------------------------------|\n";
                cout<<"|\t\t\t\t -- Belum ada data --\t\t\t\t     |\n";
                cout<<"+------------------------------------------------------------------------------------+\n";
                break;
            case 3:
                break;

            default:
                system("cls");
                cout<<"Pilihan tidak Valid(1-3)\n";
                break;
            }
        }while(pilih != 3);
    }
    
    void infoPembayaran(){
        system("cls");
        long long int Total = spp + BR + BBM + BA + BTK + BBV + DPPT;
        long long int kurang = Total - totalBayar;

        ifstream file("pembayaran.txt");
        if(file){
            string line;
            if (getline(file, line)) {
                totalBayar = safe_stoll(line);
            }
        }
        file.close();
        
        kurang = Total - totalBayar;
        cout << "=============================================\n";
        cout << "|        INFO PEMBAYARAN MAHASISWA          |\n";
        cout << "=============================================\n";
        cout << "| Biaya SPP pokok           | "<< spp << endl;
        cout << "| Biaya Registrasi          | "<< BR << endl;
        cout << "| Biaya Bina Mahasiswa      | "<< BBM << endl;
        cout << "| Biaya Asuransi           | "<< BA << endl;
        cout << "| Biaya Tabungan KKN        | "<< BTK << endl;
        cout << "| Beban Biaya Variabel      | "<< BBV << endl;
        cout << "| Biaya DPPT                | "<< DPPT << endl;
        cout << "---------------------------------------------\n";
        cout << "| TOTAL HARUS DIBAYAR       | "<< Total << endl;
        cout << "| TOTAL TERBAYAR            | "<< totalBayar << endl;
        cout << "| KEKURANGAN                | "<< kurang << endl;
        cout << "=============================================\n\n";
    }

    void pembayaran(){
        system("cls");
        long long int Total = spp + BR + BBM + BA + BTK + BBV + DPPT;
        long long int kurang = Total - totalBayar;
        cout << "Jumlah yang harus dibayar : " << Total << endl;
        cout << "Jumlah yang sudah dibayar : " << totalBayar << endl;
        cout << "Sisa kekurangan           : " << kurang << endl;
        cout << "\nMasukkan nominal pembayaran: ";
        long long int bayar;
        cin >> bayar;
        totalBayar += bayar;
        ofstream file("pembayaran.txt");
        file << totalBayar;
        file.close();
        cout << "\nPembayaran berhasil dicatat!\n";
    }
    
    void menuPresensi() {
        int pilih;
        do {
            system("cls");
            cout << "\n+-------------> MENU PRESENSI <-------------+\n";
            cout << "1. Lihat Presensi\n";
            cout << "2. Input Presensi\n";
            cout << "3. Kembali\n";
            cout << "+-------------------------------------------+\n";
            cout << "Pilih :: ";
            
            int input;
            cin >> input;
            pilih = input;
            cin.ignore(1000, '\n');
            
            switch (pilih) {
                case 1:
                    lihatPresensi();
                    break;
                case 2:
                    inputPresensi();
                    break;
                case 3:
                    return;
                default:
                    cout << "Pilihan tidak valid!\n";
                    break;
            }
        } while (pilih != 3);
    }

    void lihatPresensi() {
        system("cls");
        tampilkanInfoMahasiswa();
        
        cout << "+==================================================================+\n";
        cout << "|                        PRESENSI MAHASISWA                        |\n";
        cout << "+----------+------------------------+------+------+------+--------+\n";
        cout << "| Kode     | Mata Kuliah            |Hadir |Sakit |Izin  | Alpha  |\n";
        cout << "+----------+------------------------+------+------+------+--------+\n";
        
        bool adaData = false;
        for (int i = 0; i < jumlahPresensi; i++) {
            if (daftarPresensi[i].nim == mahasiswaNIM) {
                adaData = true;
                cout << "| " << setw(8) << left << daftarPresensi[i].kodeMatkul
                     << "| " << setw(22) << left << daftarPresensi[i].namaMatkul
                     << "| " << setw(4) << right << daftarPresensi[i].hadir
                     << "  | " << setw(4) << right << daftarPresensi[i].sakit
                     << " | " << setw(4) << right << daftarPresensi[i].izin
                     << "  | " << setw(6) << right << daftarPresensi[i].alpha << " |\n";
            }
        }
        
        if (!adaData) {
            cout << "|                  Belum ada data presensi                         |\n";
        }
        
        cout << "+----------+------------------------+------+------+------+--------+\n";
        cout << "\nTekan ENTER untuk kembali...";
        cin.get();
    }

    void inputPresensi() {
        system("cls");
        tampilkanInfoMahasiswa();
        
        bool adaKRS = false;
        for (int i = 0; i < jumlahKRS; i++) {
            if (daftarKRS[i].nim == mahasiswaNIM) {
                adaKRS = true;
                break;
            }
        }
        
        if (!adaKRS) {
            cout << "Belum ada mata kuliah di KRS. Silakan tambah mata kuliah terlebih dahulu.\n";
            cout << "\nTekan ENTER untuk kembali...";
            cin.get();
            return;
        }
        
        cout << "+==================================================================+\n";
        cout << "|                         INPUT PRESENSI                           |\n";
        cout << "+----------+------------------------+------+------+------+--------+\n";
        cout << "| Kode     | Mata Kuliah            |Hadir |Sakit |Izin  | Alpha  |\n";
        cout << "+----------+------------------------+------+------+------+--------+\n";
        
        for (int i = 0; i < jumlahPresensi; i++) {
            if (daftarPresensi[i].nim == mahasiswaNIM) {
                cout << "| " << setw(8) << left << daftarPresensi[i].kodeMatkul
                     << "| " << setw(22) << left << daftarPresensi[i].namaMatkul
                     << "| " << setw(4) << right << daftarPresensi[i].hadir
                     << "  | " << setw(4) << right << daftarPresensi[i].sakit
                     << " | " << setw(4) << right << daftarPresensi[i].izin
                     << "  | " << setw(6) << right << daftarPresensi[i].alpha << " |\n";
            }
        }
        cout << "+----------+------------------------+------+------+------+--------+\n";
        
        string kodeMatkul;
        cout << "\nMasukkan kode mata kuliah: ";
        getline(cin, kodeMatkul);
        
        bool matkulValid = false;
        for (int i = 0; i < jumlahKRS; i++) {
            if (daftarKRS[i].nim == mahasiswaNIM && daftarKRS[i].kodeMatkul == kodeMatkul) {
                matkulValid = true;
                break;
            }
        }
        
        if (!matkulValid) {
            cout << "Kode mata kuliah tidak ditemukan di KRS Anda!\n";
            cout << "\nTekan ENTER untuk kembali...";
            cin.get();
            return;
        }
        
        int pilihan;
        cout << "\nPilih jenis presensi:\n";
        cout << "1. Hadir\n";
        cout << "2. Sakit\n";
        cout << "3. Izin\n";
        cout << "4. Alpha\n";
        cout << "Pilih (1-4): ";
        cin >> pilihan;
        cin.ignore(1000, '\n');
        
        int index = -1;
        for (int i = 0; i < jumlahPresensi; i++) {
            if (daftarPresensi[i].nim == mahasiswaNIM && daftarPresensi[i].kodeMatkul == kodeMatkul) {
                index = i;
                break;
            }
        }
        
        if (index == -1) {
            Presensi presensiBaru;
            presensiBaru.nim = mahasiswaNIM;
            presensiBaru.kodeMatkul = kodeMatkul;
            
            for (int i = 0; i < jumlahKRS; i++) {
                if (daftarKRS[i].nim == mahasiswaNIM && daftarKRS[i].kodeMatkul == kodeMatkul) {
                    presensiBaru.namaMatkul = daftarKRS[i].namaMatkul;
                    break;
                }
            }
            
            presensiBaru.hadir = 0;
            presensiBaru.sakit = 0;
            presensiBaru.izin = 0;
            presensiBaru.alpha = 0;
            
            if (jumlahPresensi >= kapasitasPresensi) {
                kapasitasPresensi = kapasitasPresensi == 0 ? 10 : kapasitasPresensi * 2;
                Presensi* temp = new Presensi[kapasitasPresensi];
                for (int i = 0; i < jumlahPresensi; i++) {
                    temp[i] = daftarPresensi[i];
                }
                delete[] daftarPresensi;
                daftarPresensi = temp;
            }
            index = jumlahPresensi;
            daftarPresensi[jumlahPresensi++] = presensiBaru;
        }
        
        switch (pilihan) {
            case 1:
                daftarPresensi[index].hadir++;
                cout << "Presensi HADIR berhasil dicatat.\n";
                break;
            case 2:
                daftarPresensi[index].sakit++;
                cout << "Presensi SAKIT berhasil dicatat.\n";
                break;
            case 3:
                daftarPresensi[index].izin++;
                cout << "Presensi IZIN berhasil dicatat.\n";
                break;
            case 4:
                daftarPresensi[index].alpha++;
                cout << "Presensi ALPHA berhasil dicatat.\n";
                break;
            default:
                cout << "Pilihan tidak valid!\n";
                cout << "\nTekan ENTER untuk kembali...";
                cin.get();
                return;
        }
        
        simpanPresensiToFile();
        cout << "\nTekan ENTER untuk kembali...";
        cin.get();
    }

    void simpanPresensiToFile() {
        ofstream file("presensi.txt");
        for (int i = 0; i < jumlahPresensi; i++) {
            file << daftarPresensi[i].nim << "|" << daftarPresensi[i].kodeMatkul << "|" 
                 << daftarPresensi[i].namaMatkul << "|" << daftarPresensi[i].hadir << "|"
                 << daftarPresensi[i].sakit << "|" << daftarPresensi[i].izin << "|"
                 << daftarPresensi[i].alpha << "\n";
        }
        file.close();
    }
    
    void wifi(){
        system("cls");
        int rePassword, pilih;
        char Y;
        cout << " _________________________________________________\n";
        cout << "|              INFO WIFI UNIVERSITAS              |\n";
        cout << "|_________________________________________________|\n";
        cout << "|User Wifi :                                      |\n";
        cout << "|"<< mahasiswaNIM<<"                                       |\n";
        cout << "|Tanggal Expired:                                 |\n";
        cout << "|31 Desember 2026                                 |\n";
        cout << "|Password Wifi :                                  |\n";
        cout << "|*********                                        |\n";
        cout << "Apakahah anda ingin reset password wifi?(Y/T): ";
        cin >> Y;
        if(Y == 'Y' || Y == 'y'){    
            cout << "|Re-Password:                                     |\n";
            cout << "|"; cin >> rePassword;
        } else if(Y == 'T' || Y == 't'){
            return;
        }
        ofstream file("wifi.txt");
        if(file.is_open()){
            file <<"User Wifi : ";
            file << mahasiswaNIM << endl;
            file <<"Password Wifi :";
            file << rePassword;
            file.close();
            cout << "Penulisan ke file berhasil.\n";
        } else{
            cout << "Gagal membuka file untuk penulisan.\n";
        }
    }
    
    void kelulusan(){
        system("cls");
        int pilihan;
        do{
            cout << " _______________________________________________\n";
            cout << "|           INFO KELULUSAN MAHASISWA            |\n";
            cout << "|_______________________________________________|\n";
            cout << "|1. Wisudah                                     |\n";
            cout << "|2. Upload Tugas Akhir                          |\n";
            cout << "|0. Keluar                                      |\n";
            cout << "|pilih:: "; 
            int input;
            cin >> input;
            pilihan = input;
            switch(pilihan){
                case 1:
                    system("cls");
                    cout << " _____________________________________________________________\n";
                    cout << "|                INFORMASI PENDAFTARAN WISUDA                 |\n";
                    cout << "|_____________________________________________________________|\n";
                    cout << "| Jadwal Pendaftaran Akrif     |        Keterangan            |\n";
                    cout << "|______________________________|______________________________|\n";
                    cout << "| Periode Wisuda      |:  |    | Anda belum Terdaftar Wisudah |\n";
                    cout << "|_____________________|   |____|______________________________|\n";
                    cout << "| Mulai Pendaftaran   |:  |    |                              |\n";
                    cout << "|_____________________|   |____| Ukuran Toga:                 |\n";
                    cout << "| Selesai Pendaftaran |:  |    | Foto Anda Belum Di Upload    |\n";
                    cout << "|_____________________|___|____|______________________________|\n";
                    cout << "\n_________________________________________________________________\n";
                    cout << "|Data Pendadaran anda tidak ada, silahkan konfirmasi ke Prodi(TU)|\n";
                    cout << "|________________________________________________________________|\n";
                    break;
                case 2:
                    system("cls");
                    cout << " ________________________________________\n";
                    cout << "|        Upload File Skripsi             |\n";
                    cout << "|________________________________________|\n";
                    cout << "|      Status Anda Belum LULUS           |\n";
                    cout << "|________________________________________|\n";
                    break;
                case 0:
                    break;
                default:
                    cout << "Pilihan tidak valid!\n";
                    break;
            }
        }while (pilihan != 0);   
    }

    void sertifikasi(){
        system("cls");
        cout << "_______________________________________________\n";
        cout << "|         DAFTAR SERTIFIKASI LPS UAD          |\n";
        cout << "|_____________________________________________|\n";
        cout << "| i Informasi                                 |\n";
        cout << "| Belum ada jadwal yang ditambahkan            |\n";
        cout << "|_____________________________________________|\n";
    }

    void lain_lain(){
        int pilih;
        do{
            system("cls");
            cout << "+-------------> MENU LAIN-LAIN <-------------+\n";
            cout << "1. Info Wifi\n";
            cout << "2. Kelulusan\n";
            cout << "3. Sertifikasi\n";
            cout << "0. Kembali\n";
            cout << "+--------------------------------------------+\n";
            cout << "Pilih :: ";
            int input;
            cin >> input;
            pilih = input;
            switch(pilih){
                case 1:
                    wifi();
                    break;
                case 2:
                    kelulusan();
                    break;
                case 3:
                    sertifikasi();
                    break;
                case 0:
                    break;
                default:
                    cout << "Pilihan tidak valid!\n";
                    break;
            }
        } while (pilih != 0);
    }

    void aksesLihatRiwayatPembayaran() { 
        system("cls");
        cout << "+====================================================================+\n";
        cout << "|                      RIWAYAT PEMBAYARAN                            |\n";
        cout << "+-------------------+------------------+----------------------------+\n";
        cout << "| Tanggal           | Nominal          | Keterangan                 |\n";
        cout << "+-------------------+------------------+----------------------------+\n";
        
        bool adaData = false;
        for (int i = 0; i < jumlahPembayaran; i++) {
            if (daftarPembayaran[i].nim == mahasiswaNIM) {
                adaData = true;
                cout << "| " << setw(17) << left << daftarPembayaran[i].tanggal
                     << "| Rp " << setw(13) << left << daftarPembayaran[i].nominal
                     << "| " << setw(26) << left << daftarPembayaran[i].keterangan << "|\n";
            }
        }
        
        if (!adaData) {
            cout << "|              Belum ada riwayat pembayaran                          |\n";
        }
        cout << "+-------------------+------------------+----------------------------+\n";
        cout << "\nTekan ENTER untuk kembali...";
        cin.get();
    }

    void aksesKalenderAkademik() { 
        system("cls");
        cout << "+==================================================================+\n";
        cout << "|                      KALENDER AKADEMIK 2025/2026                 |\n";
        cout << "+==================================================================+\n";
        cout << "\nSemester Ganjil 2025/2026:\n";
        cout << "- Registrasi             : 28 Juli 2025\n";
        cout << "- Perkuliahan Dimulai    : 22 September 2025\n";
        cout << "- UTS                    : 10 - 22 November 2025\n";
        cout << "- UAS                    : 19 - 31 Januari 2026\n";
        cout << "- Libur Semester         : 02 Februari - 30 Maret 2026\n";
        cout << "\nSemester Genap 2026/2027:\n";
        cout << "- Registrasi             : 26 Januari 2026\n";
        cout << "- Perkuliahan Dimulai    : 30 Maret 2026\n";
        cout << "- UTS                    : 18 - 30 Mei 2026\n";
        cout << "- UAS                    : 27 Juli - 08 Agustus 2026\n";
        cout << "- Libur Semester         : 10 Agustus - ... 2026\n";
        cout << "\n+------------------------------------------------------------------+\n";
        cout << "| CATATAN PENTING:                                                 |\n";
        cout << "| - Batas akhir KRS: 2 minggu setelah perkuliahan dimulai         |\n";
        cout << "| - Pembayaran SPP: Paling lambat H-7 sebelum UTS                 |\n";
        cout << "+------------------------------------------------------------------+\n";
        cout << "\nTekan ENTER untuk kembali...";
        cin.get();
    }
};

// =====================================================================
// CLASS KEMAHASISWAAN
// =====================================================================
class Kemahasiswaan {
private:
    long long int mahasiswaNIM;
    string mahasiswaNama;
    Perpustakaan perpustakaan;
    Perkuliahan perkuliahan;

    string tanggalHariIni() {
        return "01-12-2025";
    }

    void centerText(const string &text, int width = 80) {
        int len = text.length();
        int space = (width - len) / 2;
        if (space < 0) space = 0;
        cout << string(space, ' ') << text << endl;
    }

public:
    Kemahasiswaan() {
        mahasiswaNIM = 0;
        mahasiswaNama = "";
    }
    
    void setMahasiswaInfo(const string& nama, long long int nim) {
        mahasiswaNama = nama;
        mahasiswaNIM = nim;
        perpustakaan.setMahasiswaNIM(nim);
        perkuliahan.setMahasiswaInfo(nama, nim, "");
    }

    void ajukanSuratAktif() {
        system("cls"); 
        
        Pengajuan pengajuanBaru;
        pengajuanBaru.nim = mahasiswaNIM;
        pengajuanBaru.jenis = "Surat Aktif"; 
        pengajuanBaru.tanggal = tanggalHariIni();
        pengajuanBaru.status = "Diproses";
        
        if (jumlahPengajuan >= kapasitasPengajuan) {
            kapasitasPengajuan = kapasitasPengajuan == 0 ? 10 : kapasitasPengajuan * 2;
            Pengajuan* temp = new Pengajuan[kapasitasPengajuan];
            for (int i = 0; i < jumlahPengajuan; i++) {
                temp[i] = daftarPengajuan[i];
            }
            delete[] daftarPengajuan;
            daftarPengajuan = temp;
        }
        daftarPengajuan[jumlahPengajuan++] = pengajuanBaru;
        
        simpanPengajuanToFile();
        
        // Kirim notifikasi ke admin
        string pesan = "Pengajuan Surat Aktif dari mahasiswa " + mahasiswaNama + " (NIM: " + to_string(mahasiswaNIM) + ")";
        
        // Cari admin untuk kirim notifikasi
        for (int i = 0; i < jumlahAkun; i++) {
            if (daftarAkun[i].role == "admin") {
                kirimNotifikasiKePenerima(daftarAkun[i].username, "TERSENDIRI", pesan, "SYSTEM");
                break;
            }
        }
        
        cout << "\n Pengajuan Surat Aktif Kuliah berhasil dikirim (" << pengajuanBaru.tanggal << ")\n";
        cout << "Notifikasi telah dikirim ke admin.\n";
    }

    void simpanPengajuanToFile() {
        ofstream file("pengajuan.txt");
        for (int i = 0; i < jumlahPengajuan; i++) {
            file << daftarPengajuan[i].nim << "|" << daftarPengajuan[i].jenis << "|" 
                 << daftarPengajuan[i].tanggal << "|" << daftarPengajuan[i].status << "\n";
        }
        file.close();
    }

    void ajukanCuti() {
        system("cls"); 
        
        Pengajuan pengajuanBaru;
        pengajuanBaru.nim = mahasiswaNIM;
        pengajuanBaru.jenis = "Cuti Akademik";
        pengajuanBaru.tanggal = tanggalHariIni();
        pengajuanBaru.status = "Menunggu Persetujuan";
        
        if (jumlahPengajuan >= kapasitasPengajuan) {
            kapasitasPengajuan = kapasitasPengajuan == 0 ? 10 : kapasitasPengajuan * 2;
            Pengajuan* temp = new Pengajuan[kapasitasPengajuan];
            for (int i = 0; i < jumlahPengajuan; i++) {
                temp[i] = daftarPengajuan[i];
            }
            delete[] daftarPengajuan;
            daftarPengajuan = temp;
        }
        daftarPengajuan[jumlahPengajuan++] = pengajuanBaru;
        
        simpanPengajuanToFile();
        
        // Kirim notifikasi ke admin
        string pesan = "Pengajuan Cuti Akademik dari mahasiswa " + mahasiswaNama + " (NIM: " + to_string(mahasiswaNIM) + ")";
        
        // Cari admin untuk kirim notifikasi
        for (int i = 0; i < jumlahAkun; i++) {
            if (daftarAkun[i].role == "admin") {
                kirimNotifikasiKePenerima(daftarAkun[i].username, "TERSENDIRI", pesan, "SYSTEM");
                break;
            }
        }
        
        cout << "\n Pengajuan Cuti Akademik berhasil dikirim (" << pengajuanBaru.tanggal << ")\n";
        cout << "Notifikasi telah dikirim ke admin.\n";
    }

    void lihatRiwayat() {
        system("cls"); 
        
        int count = 0;
        for (int i = 0; i < jumlahPengajuan; i++) {
            if (daftarPengajuan[i].nim == mahasiswaNIM) {
                count++;
            }
        }
        
        if (count == 0) {
            cout << "\nBelum ada pengajuan kemahasiswaan.\n";
            cout << "\nTekan ENTER untuk kembali...";
            cin.get();
            return;
        }

        cout << "\n+============================================================+\n";
        cout << "|               RIWAYAT PENGAJUAN MAHASISWA                |\n";
        cout << "+===============+==============+===========================+\n";
        cout << "| Jenis         | Tanggal      | Status                    |\n";
        cout << "+===============+==============+===========================+\n";

        for (int i = 0; i < jumlahPengajuan; i++) {
            if (daftarPengajuan[i].nim == mahasiswaNIM) {
                cout << "| " << setw(13) << left << daftarPengajuan[i].jenis
                     << "| " << setw(12) << left << daftarPengajuan[i].tanggal
                     << "| " << setw(25) << left << daftarPengajuan[i].status << " |\n";
                cout << "+---------------+--------------+---------------------------+\n";
            }
        }
        cout << "\nTekan ENTER untuk kembali...";
        cin.get();
    }
    
    void sertifP2k(const string& namaMahasiswa) {
        system("cls"); 
        const int width = 80;
        string nomor = "R.5/867/D.6/IX/2024";
        string kegiatan = "Program Pengenalan Kampus (P2K)";
        string universitas = "Universitas Ahmad Dahlan";
        string tahunAkademik = "Tahun Akademik 2024/2025";
        string tanggal = "9 - 14 & 17 September 2024";
        string tema = "Kolaborasi Dahlan Muda, Merawat Alam Semesta";
        string tanggalHijriyah = "6 - 11 & 14 Rabiulawal 1446 H";
        string tanggalMasehi = "9-14 & 17 September 2024 M";
        string tempat = "Yogyakarta";
        string pejabat = "Dr. Gatot Sugiharto, S.H., M.H.";
        string jabatan = "Wakil Rektor Bidang Kemahasiswaan dan Alumni";
        string nip = "NIPM 19790710 200409 111 0885095";

        cout << "\n" << string(width, '=') << endl;
        centerText("UNIVERSITAS AHMAD DAHLAN", width);
        cout << string(width, '=') << "\n\n";

        centerText("SERTIFIKAT", width);
        cout << endl;
        centerText("Nomor: " + nomor, width);
        cout << "\n";

        centerText("Diberikan kepada:", width);
        cout << "\n";
        centerText("   " + (namaMahasiswa.empty() ? "[NAMA MAHASISWA]" : namaMahasiswa) + "   ", width);
        cout << "\n";
        centerText("Atas partisipasinya sebagai:", width);
        centerText("PESERTA", width);
        cout << "\n";

        centerText("dalam " + kegiatan + " bagi Mahasiswa Baru", width);
        centerText(universitas + " " + tahunAkademik, width);
        centerText("pada tanggal " + tanggal, width);
        centerText("dengan tema:", width);
        centerText("\"" + tema + "\"", width);
        cout << "\n" << string(width, '-') << "\n";

        centerText(tempat + ", " + tanggalHijriyah, width);
        centerText(tanggalMasehi, width);
        cout << "\n";

        centerText(jabatan, width);
        cout << "\n";
        centerText("(ttd)", width);
        cout << "\n";
        centerText(pejabat, width);
        centerText(nip, width);

        cout << "\n" << string(width, '=') << endl;
        centerText("\"Kolaborasi Dahlan Muda, Merawat Alam Semesta\"", width);
        cout << string(width, '=') << endl;

        cout << "\nTekan ENTER untuk kembali...";
        cin.get(); 
    }

    void menuKemahasiswaan() {
        int pilih;
        do {
            cout << "\n+-------------> MENU KEMAHASISWAAN <-------------+\n";
            cout << "1. Ajukan Surat Aktif Kuliah\n";
            cout << "2. Ajukan Cuti Akademik\n";
            cout << "3. Lihat Riwayat Pengajuan\n";
            cout << "4. Sertifikat P2K\n";
            cout << "5. Perpustakaan" << endl;
            cout << "6. Baca Notifikasi\n";
            cout << "7. Riwayat Pembayaran\n";
            cout << "8. Presensi\n";
            cout << "9. Kalender Akademik\n";
            cout << "10. Kembali\n";
            cout << "+-----------------------------------------------+\n";
            cout << "Pilih :: ";
            
            int input;
            cin >> input;
            pilih = input;
            cin.ignore(1000, '\n');

            switch (pilih) {
                case 1: 
                    ajukanSuratAktif();
                    break;
                case 2: 
                    ajukanCuti(); 
                    break;
                case 3: 
                    lihatRiwayat(); 
                    break;
                case 4: 
                    sertifP2k(mahasiswaNama); 
                    break;
                case 5: 
                    menuPerpustakaan(); 
                    break;
                case 6: 
                    bacaNotifikasi();
                    break;
                case 7: 
                    perkuliahan.aksesLihatRiwayatPembayaran(); 
                    break;
                case 8: 
                    perkuliahan.menuPresensi(); 
                    break;
                case 9: 
                    perkuliahan.aksesKalenderAkademik(); 
                    break;
                case 10:
                    cout << "Kembali ke portal utama...\n"; 
                    break;
                default: 
                    cout << "Pilihan tidak valid!\n"; 
                    break; 
            }
        } while (pilih != 10);
    }
    
    void pinjamBuku() {
        system("cls"); 
        perpustakaan.tampilkanDaftarBukuTersedia();
        
        string kode;
        cout << "\nMasukkan kode buku yang ingin dipinjam: ";
        getline(cin, kode);
        
        if (perpustakaan.pinjamBuku(kode)) {
            cout << "Buku berhasil dipinjam!\n";
        } else {
            cout << "Gagal meminjam buku. Mungkin stok habis atau sudah dipinjam.\n";
        }
        
        cout << "Tekan ENTER untuk kembali...";
        cin.get();
    }

    void kembalikanBuku() {
        system("cls"); 
        perpustakaan.tampilkanBukuDipinjam(); 
        if (!perpustakaan.adaPinjaman()) {
            cout << "\nTekan ENTER untuk kembali...";
            cin.get();
            return; 
        }

        string kode;
        cout << "\nMasukkan kode buku yang ingin dikembalikan: ";
        getline(cin, kode);

        if (perpustakaan.kembalikanBuku(kode)) {
            cout << "Buku berhasil dikembalikan!\n";
        } else {
            cout << "Buku dengan kode tersebut tidak ditemukan di daftar pinjaman Anda.\n";
        }
        
        cout << "Tekan ENTER untuk kembali...";
        cin.get();
    }

    void cariBuku() {
        system("cls"); 
        cout << "\nCari berdasarkan Kode (misal: BK001) atau Judul (misal: Basis Data)";
        string cari;
        cout << "\nMasukkan judul atau kode buku yang dicari: ";
        getline(cin, cari);

        perpustakaan.cariBuku(cari);
        
        cout << "Tekan ENTER untuk kembali...";
        cin.get();
    }

    void menuPerpustakaan() {
        int pilih;
        do {
            cout << "\n+================= INFO DENDA =================+\n";
            int totalDenda = perpustakaan.hitungTotalDenda();
            if (totalDenda == 0) {
                cout << "|             Anda tidak memiliki denda.             |\n";
            } else {
                cout << "| TOTAL DENDA ANDA: Rp " << setw(23) << totalDenda << "|\n";
            }
            cout << "+==============================================+\n";

            cout << "\n+-------------> MENU PERPUSTAKAAN <-------------+\n";
            cout << "1. Pinjam Buku\n";
            cout << "2. Kembalikan Buku (Bayar Denda)\n";
            cout << "3. Cari Buku di Katalog\n";
            cout << "4. Lihat Daftar Buku Dipinjam\n";
            cout << "5. Kembali\n";
            cout << "+-----------------------------------------------+\n";
            cout << "Pilih :: ";
            
            int input;
            cin >> input;
            pilih = input;
            cin.ignore(1000, '\n');

            switch (pilih) {
                case 1: pinjamBuku(); break;
                case 2: kembalikanBuku(); break;
                case 3: cariBuku(); break;
                case 4: 
                    system("cls"); 
                    perpustakaan.tampilkanBukuDipinjam(); 
                    cout << "\nTekan ENTER untuk kembali..."; 
                    cin.get();
                    break;
                case 5: cout << "Kembali ke menu Kemahasiswaan...\n"; break; 
                default: cout << "Pilihan tidak valid!\n"; break; 
            }
        } while (pilih != 5);
    }

    void bacaNotifikasi() {
        // Cari username berdasarkan NIM
        string username = "";
        for (int i = 0; i < jumlahAkun; i++) {
            if (daftarAkun[i].nim == mahasiswaNIM) {
                username = daftarAkun[i].username;
                break;
            }
        }
        
        if (!username.empty()) {
            tampilkanNotifikasiUntukUser(username);
        } else {
            cout << "\nError: Username tidak ditemukan!\n";
            cout << "Tekan ENTER untuk kembali...";
            cin.get();
        }
    }
};

// =====================================================================
// CLASS DOSEN
// =====================================================================
class Dosen {
private:
    Perkuliahan perkuliahan;
    Akun* akunDosen;
    ChatSystem chatSystem;

    void tampilkanDaftarMahasiswa() {
        system("cls"); 
        cout << "+=========================================================+\n";
        cout << "|                     DAFTAR MAHASISWA                    |\n";
        cout << "+----------------+----------------------+-----------------+\n";
        cout << "| NIM            | Nama Mahasiswa       | Program Studi   |\n";
        cout << "+----------------+----------------------+-----------------+\n";
        
        int count = 0;
        for(int i = 0; i < jumlahAkun; i++) {
            if (daftarAkun[i].role == "mahasiswa") {
                cout << "| " << setw(14) << left << daftarAkun[i].nim
                     << "| " << setw(20) << left << daftarAkun[i].nama
                     << "| " << setw(15) << left << daftarAkun[i].prodi << "|\n";
                count++;
            }
        }
        
        if (count == 0) {
            cout << "|                Belum ada data mahasiswa.                |\n";
        }
        cout << "+----------------+----------------------+-----------------+\n";
    }

    void inputNilai() {
        system("cls"); 
        tampilkanDaftarMahasiswa();
        
        long long int nimInput;
        cout << "\nMasukkan NIM mahasiswa yang akan dinilai (atau '0' untuk batal): ";
        cin >> nimInput;
        cin.ignore(1000, '\n');

        if (nimInput == 0) return;

        Akun* mhs = cariAkunByNIM(nimInput);
        if (!mhs || mhs->role != "mahasiswa") {
            cout << "NIM tidak ditemukan atau bukan mahasiswa.\n";
            cout << "Tekan ENTER untuk kembali...";
            cin.get();
            return;
        }

        perkuliahan.setMahasiswaInfo(mhs->nama, mhs->nim, mhs->prodi);
        
        system("cls");
        bool adaKRS = perkuliahan.tampilkanKRSUntukDosen(nimInput);
        if (!adaKRS) {
            cout << "Mahasiswa ini belum mengambil KRS.\n";
            cout << "Tekan ENTER untuk kembali...";
            cin.get();
            return;
        }

        string kodeMatkul, nilaiMatkul;
        cout << "\nMasukkan Kode Matkul yang akan dinilai: ";
        getline(cin, kodeMatkul);
        
        if (!cekMahasiswaAmbilKRS(nimInput, kodeMatkul)) {
            cout << "Mahasiswa tidak mengambil mata kuliah ini dalam KRS!\n";
            cout << "Tidak bisa input nilai.\n";
            cout << "Tekan ENTER untuk kembali...";
            cin.get();
            return;
        }
        
        bool nilaiSudahAda = cekNilaiSudahAda(nimInput, kodeMatkul);
        if (nilaiSudahAda) {
            cout << "\nNilai untuk mata kuliah ini sudah ada!\n";
            cout << "1. Update nilai yang sudah ada\n";
            cout << "2. Batal\n";
            cout << "Pilih (1-2): ";
            
            int pilihan;
            cin >> pilihan;
            cin.ignore(1000, '\n');
            
            if (pilihan == 1) {
                updateNilai(nimInput, kodeMatkul);
                return;
            } else {
                return;
            }
        }
        
        cout << "Masukkan Nilai (A, A-, B+, B, B-, C+, C, D, E): ";
        getline(cin, nilaiMatkul);

        string nilaiValid[] = {"A", "A-", "B+", "B", "B-", "C+", "C", "D", "E"};
        bool nilaiValidFlag = false;
        for (int i = 0; i < 9; i++) {
            if (nilaiMatkul == nilaiValid[i]) {
                nilaiValidFlag = true;
                break;
            }
        }
        
        if (!nilaiValidFlag) {
            cout << "Nilai tidak valid! Harus A, A-, B+, B, B-, C+, C, D, atau E.\n";
            cout << "Tekan ENTER untuk kembali...";
            cin.get();
            return;
        }

        perkuliahan.simpanNilai(nimInput, kodeMatkul, nilaiMatkul);

        // Kirim notifikasi ke mahasiswa
        string pesan = "Nilai untuk mata kuliah " + kodeMatkul + " telah diinput: " + nilaiMatkul;
        Akun* mahasiswa = cariAkunByNIM(nimInput);
        if (mahasiswa) {
            kirimNotifikasiKePenerima(mahasiswa->username, "TERSENDIRI", pesan, akunDosen->username);
        }

        cout << "Nilai berhasil disimpan dan notifikasi telah dikirim ke mahasiswa.\n";
        cout << "Tekan ENTER untuk kembali...";
        cin.get();
    }

    void updateNilai(long long int nim, const string& kodeMatkul) {
        system("cls");
        
        Akun* mhs = cariAkunByNIM(nim);
        if (!mhs) {
            cout << "Mahasiswa tidak ditemukan!\n";
            cout << "Tekan ENTER untuk kembali...";
            cin.get();
            return;
        }
        
        int index = getIndexNilai(nim, kodeMatkul);
        if (index == -1) {
            cout << "Data nilai tidak ditemukan!\n";
            cout << "Tekan ENTER untuk kembali...";
            cin.get();
            return;
        }
        
        cout << "\n+==================================================+\n";
        cout << "|                UPDATE NILAI MAHASISWA           |\n";
        cout << "+==================================================+\n";
        cout << "Nama Mahasiswa: " << mhs->nama << endl;
        cout << "NIM: " << nim << endl;
        cout << "Mata Kuliah: " << daftarNilai[index].namaMatkul << endl;
        cout << "Nilai Lama: " << daftarNilai[index].nilai << endl;
        
        string nilaiBaru;
        cout << "\nMasukkan Nilai Baru (A, A-, B+, B, B-, C+, C, D, E): ";
        getline(cin, nilaiBaru);
        
        string nilaiValid[] = {"A", "A-", "B+", "B", "B-", "C+", "C", "D", "E"};
        bool nilaiValidFlag = false;
        for (int i = 0; i < 9; i++) {
            if (nilaiBaru == nilaiValid[i]) {
                nilaiValidFlag = true;
                break;
            }
        }
        
        if (!nilaiValidFlag) {
            cout << "Nilai tidak valid! Harus A, A-, B+, B, B-, C+, C, D, atau E.\n";
            cout << "Tekan ENTER untuk kembali...";
            cin.get();
            return;
        }
        
        daftarNilai[index].nilai = nilaiBaru;
        
        perkuliahan.simpanNilaiToFile();
        
        // Kirim notifikasi ke mahasiswa
        string pesan = "Nilai untuk mata kuliah " + kodeMatkul + " telah diupdate menjadi: " + nilaiBaru;
        kirimNotifikasiKePenerima(mhs->username, "TERSENDIRI", pesan, akunDosen->username);
        
        cout << "\nNilai berhasil diupdate dan notifikasi telah dikirim!\n";
        cout << "Nilai " << daftarNilai[index].namaMatkul << ": " << nilaiBaru << endl;
        cout << "Tekan ENTER untuk kembali...";
        cin.get();
    }

    void menuUpdateNilai() {
        system("cls");
        tampilkanDaftarMahasiswa();
        
        long long int nimInput;
        cout << "\nMasukkan NIM mahasiswa yang akan diupdate nilainya: ";
        cin >> nimInput;
        cin.ignore(1000, '\n');

        if (nimInput == 0) return;

        Akun* mhs = cariAkunByNIM(nimInput);
        if (!mhs || mhs->role != "mahasiswa") {
            cout << "NIM tidak ditemukan atau bukan mahasiswa.\n";
            cout << "Tekan ENTER untuk kembali...";
            cin.get();
            return;
        }
        
        system("cls");
        cout << "\n+==================================================+\n";
        cout << "|           UPDATE NILAI MAHASISWA                |\n";
        cout << "+==================================================+\n";
        cout << "Nama: " << mhs->nama << " | NIM: " << nimInput << endl;
        cout << "+-----------+-------------------------------------+-------+\n";
        cout << "| Kode      | Mata Kuliah                        | Nilai |\n";
        cout << "+-----------+-------------------------------------+-------+\n";
        
        bool adaNilai = false;
        for (int i = 0; i < jumlahNilai; i++) {
            if (daftarNilai[i].nim == nimInput) {
                adaNilai = true;
                cout << "| " << setw(9) << left << daftarNilai[i].kodeMatkul
                     << "| " << setw(35) << left << daftarNilai[i].namaMatkul
                     << "| " << setw(5) << left << daftarNilai[i].nilai << " |\n";
            }
        }
        
        if (!adaNilai) {
            cout << "|          Belum ada nilai untuk mahasiswa ini          |\n";
            cout << "Tekan ENTER untuk kembali...";
            cin.get();
            return;
        }
        
        cout << "+-----------+-------------------------------------+-------+\n";
        
        string kodeMatkul;
        cout << "\nMasukkan Kode Mata Kuliah yang akan diupdate: ";
        getline(cin, kodeMatkul);
        
        if (!cekNilaiSudahAda(nimInput, kodeMatkul)) {
            cout << "Nilai untuk mata kuliah ini belum ada!\n";
            cout << "Gunakan menu 'Input Nilai' untuk menambah nilai baru.\n";
            cout << "Tekan ENTER untuk kembali...";
            cin.get();
            return;
        }
        
        updateNilai(nimInput, kodeMatkul);
    }

    void editProfil() {
        system("cls"); 
        int pilih;
        string input;
        cout << "+==================================================+\n";
        cout << "|                     EDIT PROFIL                  |\n";
        cout << "+==================================================+\n";
        cout << "Data Anda Saat Ini (Role: " << akunDosen->role << ")\n";
        cout << "1. Nama          : " << akunDosen->nama << "\n";
        cout << "2. ID (NIP)      : " << akunDosen->nim << "\n";
        cout << "3. Fakultas/Dept : " << akunDosen->prodi << "\n";
        cout << "4. Password      : [RAHASIA]\n";
        cout << "5. Kembali\n";
        cout << "+--------------------------------------------------+\n";
        cout << "Apa yang ingin Anda ubah? (1-4): ";
        
        int inputPilih;
        cin >> inputPilih;
        pilih = inputPilih;
        cin.ignore(1000, '\n');

        switch(pilih) {
            case 1:
                cout << "Masukkan Nama baru: ";
                getline(cin, akunDosen->nama);
                cout << "Nama berhasil diubah!\n";
                break;
            case 2:
                cout << "Masukkan NIP baru: ";
                long long int newNIP;
                cin >> newNIP;
                akunDosen->nim = newNIP;
                cin.ignore(1000, '\n');
                cout << "NIP berhasil diubah!\n";
                break;
            case 3:
                cout << "Masukkan Fakultas/Dept baru: ";
                getline(cin, akunDosen->prodi);
                cout << "Fakultas/Dept berhasil diubah!\n";
                break;
            case 4:
                cout << "Masukkan Password baru: ";
                getline(cin, akunDosen->password);
                cout << "Password berhasil diubah!\n";
                break;
            case 5:
                return;
            default:
                cout << "Pilihan tidak valid.\n";
        }
        
        simpanAkunToFile();
        cout << "Tekan ENTER untuk kembali...";
        cin.get();
    }

    void simpanAkunToFile() {
        ofstream file("akun_dosen.txt", ios::trunc);
        for (int i = 0; i < jumlahAkun; i++) {
            if (daftarAkun[i].role == "dosen") {
                file << daftarAkun[i].username << "|"
                     << daftarAkun[i].password << "|"
                     << daftarAkun[i].nama << "|"
                     << daftarAkun[i].nim << "|"
                     << daftarAkun[i].prodi << "|"
                     << daftarAkun[i].pertanyaan << "|"
                     << daftarAkun[i].jawaban << "|"
                     << daftarAkun[i].role << "\n";
            }
        }
        file.close();
    }

public:
    Dosen(Akun* akun) {
        akunDosen = akun;
        chatSystem.setCurrentUser(akun->username);
    }

    void menuDosen() {
        int pilih;
        do {
            // Hitung notifikasi belum dibaca
            int notifBelumDibaca = hitungNotifikasiBelumDibaca(akunDosen->username);
            
            cout << "\n+==================================================+\n";
            cout << "           PORTAL DOSEN - " << akunDosen->nama << "           \n";
            if (notifBelumDibaca > 0) {
                cout << "|  NOTIFIKASI: " << notifBelumDibaca << " belum dibaca" << setw(26) << " |\n";
            }
            cout << "+==================================================+\n";
            cout << "1. Input Nilai Mahasiswa\n"; 
            cout << "2. Update Nilai Mahasiswa\n";
            cout << "3. Lihat Data Mahasiswa\n";
            cout << "4. Sistem Chat & Pesan\n";
            cout << "5. Baca Notifikasi\n";
            cout << "6. Edit Profil\n";
            cout << "7. Logout\n";
            cout << "+--------------------------------------------------+\n";
            cout << "Pilih :: ";

            int input;
            cin >> input;
            pilih = input;
            cin.ignore(1000, '\n');

            switch (pilih) {
            case 1:
                inputNilai();
                break;
            case 2:
                menuUpdateNilai();
                break;
            case 3:
                tampilkanDaftarMahasiswa(); 
                cout << "\nTekan ENTER untuk kembali...";
                cin.get();
                break;
            case 4:
                chatSystem.menuChat();
                break;
            case 5:
                tampilkanNotifikasiUntukUser(akunDosen->username);
                break;
            case 6:
                editProfil();
                break;
            case 7:
                cout << "\nAnda telah logout. Terima kasih!\n"; 
                break;
            default:
                cout << "Pilihan tidak valid!\n"; 
            }
        } while (pilih != 7);
    }
};

// =====================================================================
// CLASS ADMIN
// =====================================================================
class Admin {
private:
    struct DataHapus {
        string username;
        string password;
        string role;
        long long int nim;
        string nama;
    };

    DataHapus* undoStack;
    int topUndo;
    int kapasitasUndo;

    Perpustakaan perpustakaanAdmin;
    Akun* akunAdmin;
    
    ChatSystem chatSystem;

    bool cekUsername(string username) {
        for (int i = 0; i < jumlahAkun; i++) {
            if (daftarAkun[i].username == username) {
                return true;
            }
        }
        return false;
    }

    void statistikMahasiswa() {
        system("cls");
        cout << "+==================================================+\n";
        cout << "|            STATISTIK DATA MAHASISWA             |\n";
        cout << "+==================================================+\n";
        
        int totalMhs = 0, totalDosen = 0, totalAdmin = 0;
        
        for (int i = 0; i < jumlahAkun; i++) {
            if (daftarAkun[i].role == "mahasiswa") {
                totalMhs++;
            } else if (daftarAkun[i].role == "dosen") {
                totalDosen++;
            } else if (daftarAkun[i].role == "admin") {
                totalAdmin++;
            }
        }
        
        cout << "\nTotal Pengguna Terdaftar:\n";
        cout << "- Mahasiswa : " << totalMhs << " orang\n";
        cout << "- Dosen     : " << totalDosen << " orang\n";
        cout << "- Admin     : " << totalAdmin << " orang\n";
        
        cout << "\nTekan ENTER untuk kembali...";
        cin.get();
    }

    void buatAkunDosen() {
        system("cls"); 
        Akun baru;
        cout << "\n=== BUAT AKUN DOSEN BARU ===\n";
        
        do {
            cout << "Masukkan username: ";
            getline(cin, baru.username);
            if (cekUsername(baru.username)) {
                cout << "Username '" << baru.username << "' sudah terdaftar. Coba username lain.\n";
            }
        } while (cekUsername(baru.username));

        cout << "Masukkan password: ";
        getline(cin, baru.password);
        
        baru.role = "dosen";
        cout << "Masukkan Nama Lengkap Dosen (misal: Dr. Budi, M.Kom): ";
        getline(cin, baru.nama);
        cout << "Masukkan NIP/ID Dosen: ";
        long long int nip;
        cin >> nip;
        baru.nim = nip;
        cin.ignore(1000, '\n');
        cout << "Masukkan Fakultas (misal: FTI): ";
        getline(cin, baru.prodi);
        
        cout << "Masukkan pertanyaan keamanan (contoh: Siapa nama ibu kandung Anda?): ";
        getline(cin, baru.pertanyaan);
        cout << "Masukkan jawaban keamanan: ";
        getline(cin, baru.jawaban);

        tambahAkun(baru);
        
        // Simpan akun dosen ke file setelah membuat
        simpanSemuaAkunKeFile();
        
        // Kirim notifikasi selamat datang ke dosen baru
        string pesan = "Selamat bergabung di Portal UAD, " + baru.nama + "! Akun dosen Anda telah berhasil dibuat.";
        kirimNotifikasiKePenerima(baru.username, "TERSENDIRI", pesan, "SYSTEM");

        cout << "\nAkun DOSEN '" << baru.nama << "' berhasil dibuat!\n";
        cout << "Username: " << baru.username << " | NIP: " << baru.nim << "\n";
        cout << "Notifikasi selamat datang telah dikirim.\n";
        cout << "Tekan ENTER untuk kembali...";
        cin.get();
    }

    void buatAkunMahasiswa() {
        system("cls"); 
        Akun baru;
        cout << "\n=== BUAT AKUN MAHASISWA BARU ===\n";
        
        do {
            cout << "Masukkan username: ";
            getline(cin, baru.username);
            if (cekUsername(baru.username)) {
                cout << "Username '" << baru.username << "' sudah terdaftar. Coba username lain.\n";
            }
        } while (cekUsername(baru.username));

        cout << "Masukkan password: ";
        getline(cin, baru.password);
        
        baru.role = "mahasiswa";
        cout << "Masukkan Nama Lengkap Mahasiswa: ";
        getline(cin, baru.nama);
        cout << "Masukkan NIM: ";
        long long int nim;
        cin >> nim;
        baru.nim = nim;
        cin.ignore(1000, '\n');
        cout << "Masukkan Program Studi: ";
        getline(cin, baru.prodi);
        
        cout << "Masukkan pertanyaan keamanan (contoh: Siapa nama ibu kandung Anda?): ";
        getline(cin, baru.pertanyaan);
        cout << "Masukkan jawaban keamanan: ";
        getline(cin, baru.jawaban);

        tambahAkun(baru);
        
        // Simpan akun mahasiswa ke file setelah membuat
        simpanSemuaAkunKeFile();
        
        // Kirim notifikasi selamat datang ke mahasiswa baru
        string pesan = "Selamat bergabung di Portal UAD, " + baru.nama + "! Akun mahasiswa Anda telah berhasil dibuat.";
        kirimNotifikasiKePenerima(baru.username, "TERSENDIRI", pesan, "SYSTEM");

        cout << "\nAkun MAHASISWA '" << baru.nama << "' berhasil dibuat!\n";
        cout << "Username: " << baru.username << " | NIM: " << baru.nim << "\n";
        cout << "Notifikasi selamat datang telah dikirim.\n";
        cout << "Tekan ENTER untuk kembali...";
        cin.get();
    }

    void simpanSemuaAkunKeFile() {
        // Kosongkan file terlebih dahulu
        ofstream clear1("akun_mahasiswa.txt", ios::trunc);
        ofstream clear2("akun_dosen.txt", ios::trunc);
        ofstream clear3("akun_admin.txt", ios::trunc);
        clear1.close(); clear2.close(); clear3.close();

        // Tulis semua data akun ke file yang sesuai
        for (int i = 0; i < jumlahAkun; i++) {
            string filename;
            if (daftarAkun[i].role == "mahasiswa") {
                filename = "akun_mahasiswa.txt";
            } else if (daftarAkun[i].role == "dosen") {
                filename = "akun_dosen.txt";
            } else if (daftarAkun[i].role == "admin") {
                filename = "akun_admin.txt";
            } else {
                continue;
            }

            ofstream file(filename.c_str(), ios::app);
            if (file) {
                file << daftarAkun[i].username << "|"
                     << daftarAkun[i].password << "|"
                     << daftarAkun[i].nama << "|"
                     << daftarAkun[i].nim << "|"
                     << daftarAkun[i].prodi << "|"
                     << daftarAkun[i].pertanyaan << "|"
                     << daftarAkun[i].jawaban << "|"
                     << daftarAkun[i].role << "\n";
                file.close();
            }
        }
    }

    void menuBuatAkun() {
        int pilih;
        do {
            system("cls");
            cout << "\n+==================================================+\n";
            cout << "|               BUAT AKUN PENGGUNA BARU           |\n";
            cout << "+==================================================+\n";
            cout << "1. Buat Akun Dosen\n";
            cout << "2. Buat Akun Mahasiswa\n";
            cout << "3. Kembali ke Menu Admin\n";
            cout << "+--------------------------------------------------+\n";
            cout << "Pilih :: ";
            
            int input;
            cin >> input;
            pilih = input;
            cin.ignore(1000, '\n');

            switch(pilih) {
                case 1:
                    buatAkunDosen();
                    break;
                case 2:
                    buatAkunMahasiswa();
                    break;
                case 3:
                    return;
                default:
                    cout << "Pilihan tidak valid!\n";
                    cout << "Tekan ENTER untuk melanjutkan...";
                    cin.get();
            }
        } while (pilih != 3);
    }

    void lihatSemuaAkun() {
        system("cls"); 
        cout << "+========================================================================+\n";
        cout << "|                         DAFTAR SEMUA PENGGUNA                        |\n";
        cout << "+------------------+----------------------+----------------+-----------+\n";
        cout << "| Username         | Nama                 | ID (NIM/NIP)   | Role      |\n";
        cout << "+------------------+----------------------+----------------+-----------+\n";
        
        for (int i = 0; i < jumlahAkun; i++) {
            cout << "| " << setw(16) << left << daftarAkun[i].username
                 << "| " << setw(20) << left << daftarAkun[i].nama
                 << "| " << setw(14) << left << daftarAkun[i].nim
                 << "| " << setw(9) << left << daftarAkun[i].role << "|\n";
        }
        
        if (jumlahAkun == 0) {
            cout << "|                   Belum ada data pengguna terdaftar.                   |\n";
        }
        cout << "+------------------+----------------------+----------------+-----------+\n";
        
        cout << "\nTekan ENTER untuk kembali...";
        cin.get();
    }

    void simpanUndoKeFile(DataHapus d) {
        ofstream out("undo.txt", ios::app);
        out << d.username << "|"
            << d.password << "|"
            << d.role     << "|"
            << d.nim      << "|"
            << d.nama     << "\n";
        out.close();
    }

    void pushUndo(DataHapus d) {
        if (topUndo >= kapasitasUndo - 1) {
            kapasitasUndo = kapasitasUndo * 2;
            DataHapus* temp = new DataHapus[kapasitasUndo];
            for (int i = 0; i <= topUndo; i++) {
                temp[i] = undoStack[i];
            }
            delete[] undoStack;
            undoStack = temp;
        }
        undoStack[++topUndo] = d;
        simpanUndoKeFile(d);
    }

    DataHapus popUndo() {
        DataHapus kosong = {"","","",0,""};
        if (topUndo < 0) return kosong;

        DataHapus d = undoStack[topUndo--];

        ifstream file("undo.txt");
        string* lines = new string[200];
        int count = 0;

        while (getline(file, lines[count])) {
            count++;
        }
        file.close();

        if (count > 0) count--;

        ofstream out("undo.txt");
        for (int i = 0; i < count; i++) {
            out << lines[i] << "\n";
        }
        out.close();
        
        delete[] lines;

        return d;
    }

    void muatUndoDariFile() {
        ifstream file("undo.txt");
        if (!file.is_open()) {
            return;
        }
        
        // Baca semua data ke array sementara
        DataHapus* tempData = nullptr;
        int tempCount = 0;
        int tempCapacity = 0;
        
        string line;
        while (getline(file, line)) {
            if (line.empty()) continue;
            
            DataHapus d;
            int pos1 = 0, pos2 = 0;
            
            pos2 = line.find('|', pos1);
            d.username = line.substr(pos1, pos2 - pos1);
            
            pos1 = pos2 + 1;
            pos2 = line.find('|', pos1);
            d.password = line.substr(pos1, pos2 - pos1);
            
            pos1 = pos2 + 1;
            pos2 = line.find('|', pos1);
            d.role = line.substr(pos1, pos2 - pos1);
            
            pos1 = pos2 + 1;
            pos2 = line.find('|', pos1);
            d.nim = safe_stoll(line.substr(pos1, pos2 - pos1));
            
            pos1 = pos2 + 1;
            d.nama = line.substr(pos1);
            
            // Tambah ke array sementara
            if (tempCount >= tempCapacity) {
                tempCapacity = (tempCapacity == 0) ? 10 : tempCapacity * 2;
                DataHapus* newTemp = new DataHapus[tempCapacity];
                for (int i = 0; i < tempCount; i++) {
                    newTemp[i] = tempData[i];
                }
                if (tempData) delete[] tempData;
                tempData = newTemp;
            }
            tempData[tempCount++] = d;
        }
        file.close();
        
        // Pindahkan data dari array sementara ke undoStack
        if (tempData) {
            for (int i = 0; i < tempCount; i++) {
                pushUndo(tempData[i]);
            }
            delete[] tempData;
        }
    }

    void hapusAkunPengguna() {
        system("cls"); 
        cout << "+==================================================+\n";
        cout << "|                 HAPUS AKUN PENGGUNA             |\n";
        cout << "+==================================================+\n";
        
        string usernameHapus;
        cout << "Masukkan username yang akan dihapus (atau '0' untuk batal): ";
        getline(cin, usernameHapus);

        if (usernameHapus == "0") return;
        
        if (usernameHapus == akunAdmin->username) {
            cout << "\nTidak bisa menghapus akun Anda sendiri!\n";
            cin.get();
            return;
        }

        int index = -1;
        for (int i = 0; i < jumlahAkun; i++) {
            if (daftarAkun[i].username == usernameHapus) {
                index = i;
                break;
            }
        }

        if (index == -1) {
            cout << "\nAkun tidak ditemukan.\n";
            cin.get();
            return;
        }

        DataHapus d;
        d.username = daftarAkun[index].username;
        d.password = daftarAkun[index].password;
        d.role     = daftarAkun[index].role;
        d.nim      = daftarAkun[index].nim;
        d.nama     = daftarAkun[index].nama;
        
        pushUndo(d);

        hapusAkun(index);

        simpanSemuaAkunKeFile();
        
        // Kirim notifikasi ke user yang dihapus
        string pesan = "Akun Anda telah dinonaktifkan oleh admin. Silakan hubungi admin untuk informasi lebih lanjut.";
        kirimNotifikasiKePenerima(usernameHapus, "TERSENDIRI", pesan, akunAdmin->username);

        cout << "\nAkun '" << usernameHapus << "' berhasil dihapus.\n";
        cout << "Akun disimpan ke UNDO dan notifikasi telah dikirim.\n";

        cin.get();
    }

    void kembalikanAkun() {
        system("cls");
        cout << "+==================================================+\n";
        cout << "|                KEMBALIKAN AKUN (UNDO)            |\n";
        cout << "+==================================================+\n";

        if (topUndo < 0) {
            cout << "Tidak ada akun untuk dikembalikan!\n";
            cin.get();
            return;
        }

        DataHapus d = popUndo();

        Akun baru;
        baru.username = d.username;
        baru.password = d.password;
        baru.role     = d.role;
        baru.nim      = d.nim;
        baru.nama     = d.nama;
        baru.prodi = "Belum diisi";
        baru.pertanyaan = "Belum diisi";
        baru.jawaban = "Belum diisi";

        tambahAkun(baru);
        
        // Simpan ke file setelah mengembalikan akun
        simpanSemuaAkunKeFile();
        
        // Kirim notifikasi ke user yang dikembalikan
        string pesan = "Akun Anda telah diaktifkan kembali oleh admin. Selamat kembali ke Portal UAD!";
        kirimNotifikasiKePenerima(d.username, "TERSENDIRI", pesan, akunAdmin->username);

        cout << "\nAkun '" << d.username << "' telah dikembalikan!\n";
        cout << "Notifikasi telah dikirim ke user.\n";
        cin.get();
    }

    void riwayatAkunDihapus() {
        system("cls");
        cout << "+==================================================+\n";
        cout << "|              RIWAYAT AKUN DIHAPUS               |\n";
        cout << "+==================================================+\n\n";

        ifstream file("undo.txt");

        if (!file.is_open()) {
            cout << "Belum ada riwayat penghapusan akun.\n";
            cout << "Tekan ENTER...";
            cin.get();
            return;
        }

        string line;
        bool adaData = false;

        while (getline(file, line)) {
            adaData = true;

            int pos1 = 0, pos2 = 0;
            
            pos2 = line.find('|', pos1);
            string u = line.substr(pos1, pos2 - pos1);
            
            pos1 = pos2 + 1;
            pos2 = line.find('|', pos1);
            string p = line.substr(pos1, pos2 - pos1);
            
            pos1 = pos2 + 1;
            pos2 = line.find('|', pos1);
            string r = line.substr(pos1, pos2 - pos1);
            
            pos1 = pos2 + 1;
            pos2 = line.find('|', pos1);
            long long int n = safe_stoll(line.substr(pos1, pos2 - pos1));
            
            pos1 = pos2 + 1;
            string nm = line.substr(pos1);

            cout << "Username : " << u << endl;
            cout << "Password : " << p << endl;
            cout << "Role     : " << r << endl;
            cout << "NIM      : " << n << endl;
            cout << "Nama     : " << nm << endl;
            cout << "----------------------------------------\n";
        }

        if (!adaData) {
            cout << "Riwayat masih kosong.\n";
        }

        file.close();

        cout << "\nTekan ENTER untuk kembali...";
        cin.get();
    }

    void lihatKoleksiBuku() {
        system("cls"); 
        cout << "+=======================================================+\n";
        cout << "|                 DATABASE MASTER BUKU                  |\n";
        cout << "+-----------+-------------------------------+-----------+\n";
        cout << "| Kode Buku | Judul Buku                    | Stok      |\n";
        cout << "+-----------+-------------------------------+-----------+\n";

        perpustakaanAdmin.lihatKoleksiBukuAdmin();
        
        cout << "+-----------+-------------------------------+-----------+\n";
        cout << "\nTekan ENTER untuk kembali...";
        cin.get();
    }

    void tambahBukuBaru() {
        system("cls"); 
        cout << "+==================================================+\n";
        cout << "|                   TAMBAH BUKU BARU                 |\n";
        cout << "+==================================================+\n";

        string kode, judul;
        int stok;
        
        cout << "Masukkan Kode Buku (misal: BK006): ";
        getline(cin, kode);
        
        cout << "Masukkan Judul Buku: ";
        getline(cin, judul);
        
        cout << "Masukkan Jumlah Stok Awal: ";
        int inputStok;
        cin >> inputStok;
        stok = inputStok;
        cin.ignore(1000, '\n');

        if (perpustakaanAdmin.tambahBukuBaruAdmin(kode, judul, stok)) {
            cout << "\nBuku '" << judul << "' berhasil ditambahkan ke database.\n";
            
            // Kirim notifikasi ke semua mahasiswa
            string pesan = "Buku baru tersedia: " + judul + " (" + kode + "). Segera kunjungi perpustakaan!";
            kirimNotifikasiKePenerima("ALL", "MAHASISWA", pesan, "PERPUSTAKAAN");
            
            cout << "Notifikasi telah dikirim ke semua mahasiswa.\n";
        } else {
            cout << "\nGagal menambah buku. Kode buku mungkin sudah ada.\n";
        }
        
        cout << "Tekan ENTER untuk kembali...";
        cin.get();
    }

    void hapusBuku() {
        system("cls"); 
        cout << "+==================================================+\n";
        cout << "|                    HAPUS BUKU                    |\n";
        cout << "+==================================================+\n";
        
        cout << "+-----------+-------------------------------+-----------+\n";
        perpustakaanAdmin.lihatKoleksiBukuAdmin();
        cout << "+-----------+-------------------------------+-----------+\n";

        string kodeHapus;
        cout << "\nMasukkan Kode Buku yang akan dihapus: ";
        getline(cin, kodeHapus);

        if (perpustakaanAdmin.hapusBukuAdmin(kodeHapus)) {
            cout << "\nBuku berhasil dihapus.\n";
        } else {
            cout << "\nKode buku tidak ditemukan atau database kosong.\n";
        }
        
        cout << "Tekan ENTER untuk kembali...";
        cin.get();
    }

    void menuManajemenBuku() {
        int pilih;
        do {
            cout << "\n+==================================================+\n";
            cout << "|            MANAJEMEN BUKU PERPUSTAKAAN           |\n";
            cout << "+==================================================+\n";
            cout << "1. Lihat Semua Koleksi Buku\n";
            cout << "2. Tambah Buku Baru\n";
            cout << "3. Hapus Buku\n";
            cout << "4. Kembali ke Menu Admin\n";
            cout << "+--------------------------------------------------+\n";
            cout << "Pilih :: ";

            int input;
            cin >> input;
            pilih = input;
            cin.ignore(1000, '\n');

            switch (pilih) {
            case 1:
                lihatKoleksiBuku();
                break;
            case 2:
                tambahBukuBaru();
                break;
            case 3:
                hapusBuku();
                break;
            case 4:
                break; 
            default:
                cout << "Pilihan tidak valid!\n"; 
            }
        } while (pilih != 4);
    }

    // =====================================================================
    // FITUR ADMIN: MANAJEMEN MATA KULIAH DAN JADWAL
    // =====================================================================
    void menuManajemenMatkul() {
        int pilih;
        do {
            system("cls");
            cout << "\n+==================================================+\n";
            cout << "|         MANAJEMEN MATA KULIAH DAN JADWAL         |\n";
            cout << "+==================================================+\n";
            cout << "1. Tambah Mata Kuliah Baru\n";
            cout << "2. Lihat Semua Mata Kuliah\n";
            cout << "3. Update Mata Kuliah\n";
            cout << "4. Hapus Mata Kuliah\n";
            cout << "5. Tambah Jadwal Kuliah\n";
            cout << "6. Lihat Semua Jadwal\n";
            cout << "7. Hapus Jadwal\n";
            cout << "8. Kembali ke Menu Admin\n";
            cout << "+--------------------------------------------------+\n";
            cout << "Pilih :: ";

            int input;
            cin >> input;
            pilih = input;
            cin.ignore(1000, '\n');

            switch (pilih) {
            case 1:
                tambahMatkulBaru();
                break;
            case 2:
                lihatSemuaMatkul();
                break;
            case 3:
                updateMatkul();
                break;
            case 4:
                hapusMatkul();
                break;
            case 5:
                tambahJadwalKuliah();
                break;
            case 6:
                lihatSemuaJadwal();
                break;
            case 7:
                hapusJadwalKuliah();
                break;
            case 8:
                break;
            default:
                cout << "Pilihan tidak valid!\n";
                cout << "Tekan ENTER untuk melanjutkan...";
                cin.get();
            }
        } while (pilih != 8);
    }

    void tambahMatkulBaru() {
        system("cls");
        cout << "\n+==================================================+\n";
        cout << "|              TAMBAH MATA KULIAH BARU            |\n";
        cout << "+==================================================+\n";

        Matkul matkulBaru;
        
        cout << "Masukkan Kode Mata Kuliah (contoh: IF101): ";
        getline(cin, matkulBaru.kode);
        
        for (int i = 0; i < jumlahMatkul; i++) {
            if (daftarMatkul[i].kode == matkulBaru.kode) {
                cout << "Kode mata kuliah sudah ada!\n";
                cout << "Tekan ENTER untuk kembali...";
                cin.get();
                return;
            }
        }
        
        cout << "Masukkan Nama Mata Kuliah: ";
        getline(cin, matkulBaru.nama);
        
        cout << "Masukkan Jumlah SKS (1-4): ";
        int sksInput;
        cin >> sksInput;
        if (sksInput < 1 || sksInput > 4) {
            cout << "SKS tidak valid! Harus antara 1-4.\n";
            cout << "Tekan ENTER untuk kembali...";
            cin.get();
            cin.ignore(1000, '\n');
            return;
        }
        matkulBaru.sks = sksInput;
        cin.ignore(1000, '\n');
        
        cout << "Masukkan Semester (1-8): ";
        getline(cin, matkulBaru.semester);
        
        cout << "Masukkan Program Studi: ";
        getline(cin, matkulBaru.prodi);
        
        tambahMatkulGlobal(matkulBaru);
        
        simpanMatkulToFile();
        
        // Kirim notifikasi ke dosen
        string pesan = "Mata kuliah baru telah ditambahkan: " + matkulBaru.nama + " (" + matkulBaru.kode + ")";
        kirimNotifikasiKePenerima("ALL", "DOSEN", pesan, "ADMIN");
        
        cout << "\nMata kuliah berhasil ditambahkan!\n";
        cout << "Notifikasi telah dikirim ke semua dosen.\n";
        cout << "Tekan ENTER untuk kembali...";
        cin.get();
    }

    void simpanMatkulToFile() {
        ofstream file("matkul.txt");
        for (int i = 0; i < jumlahMatkul; i++) {
            file << daftarMatkul[i].kode << "|"
                 << daftarMatkul[i].nama << "|"
                 << daftarMatkul[i].sks << "|"
                 << daftarMatkul[i].semester << "|"
                 << daftarMatkul[i].prodi << "\n";
        }
        file.close();
    }

    void lihatSemuaMatkul() {
        system("cls");
        cout << "\n+================================================================================+\n";
        cout << "|                         DAFTAR SEMUA MATA KULIAH                            |\n";
        cout << "+===========+===============================================+=====+========+========+\n";
        cout << "| Kode      | Nama Mata Kuliah                              | SKS | Semester| Prodi  |\n";
        cout << "+===========+===============================================+=====+========+========+\n";
        
        if (jumlahMatkul == 0) {
            cout << "|                                 Belum ada mata kuliah                                 |\n";
        } else {
            for (int i = 0; i < jumlahMatkul; i++) {
                cout << "| " << setw(9) << left << daftarMatkul[i].kode
                     << "| " << setw(45) << left << daftarMatkul[i].nama
                     << "| " << setw(3) << right << daftarMatkul[i].sks
                     << "  | " << setw(6) << left << daftarMatkul[i].semester
                     << " | " << setw(6) << left << daftarMatkul[i].prodi << " |\n";
                cout << "+-----------+-----------------------------------------------+-----+--------+--------+\n";
            }
        }
        
        cout << "\nTekan ENTER untuk kembali...";
        cin.get();
    }

    void updateMatkul() {
        system("cls");
        lihatSemuaMatkul();
        
        if (jumlahMatkul == 0) {
            cout << "\nTidak ada mata kuliah yang bisa diupdate.\n";
            cout << "Tekan ENTER untuk kembali...";
            cin.get();
            return;
        }
        
        string kodeUpdate;
        cout << "\nMasukkan Kode Mata Kuliah yang akan diupdate: ";
        getline(cin, kodeUpdate);
        
        int index = -1;
        for (int i = 0; i < jumlahMatkul; i++) {
            if (daftarMatkul[i].kode == kodeUpdate) {
                index = i;
                break;
            }
        }
        
        if (index == -1) {
            cout << "Kode mata kuliah tidak ditemukan!\n";
            cout << "Tekan ENTER untuk kembali...";
            cin.get();
            return;
        }
        
        cout << "\nData Lama:\n";
        cout << "Kode: " << daftarMatkul[index].kode << endl;
        cout << "Nama: " << daftarMatkul[index].nama << endl;
        cout << "SKS: " << daftarMatkul[index].sks << endl;
        cout << "Semester: " << daftarMatkul[index].semester << endl;
        cout << "Prodi: " << daftarMatkul[index].prodi << endl;
        
        cout << "\nMasukkan data baru (kosongkan jika tidak ingin mengubah):\n";
        
        string input;
        cout << "Nama Mata Kuliah Baru: ";
        getline(cin, input);
        if (!input.empty()) {
            daftarMatkul[index].nama = input;
        }
        
        cout << "SKS Baru (1-4, tekan ENTER untuk tidak mengubah): ";
        getline(cin, input);
        if (!input.empty()) {
            int sksBaru = stoi(input);
            if (sksBaru >= 1 && sksBaru <= 4) {
                daftarMatkul[index].sks = sksBaru;
            } else {
                cout << "SKS tidak valid, tetap menggunakan nilai lama.\n";
            }
        }
        
        cout << "Semester Baru (1-8): ";
        getline(cin, input);
        if (!input.empty()) {
            daftarMatkul[index].semester = input;
        }
        
        cout << "Program Studi Baru: ";
        getline(cin, input);
        if (!input.empty()) {
            daftarMatkul[index].prodi = input;
        }
        
        simpanMatkulToFile();
        
        // Kirim notifikasi ke dosen
        string pesan = "Mata kuliah " + daftarMatkul[index].nama + " (" + daftarMatkul[index].kode + ") telah diupdate.";
        kirimNotifikasiKePenerima("ALL", "DOSEN", pesan, "ADMIN");
        
        cout << "\nMata kuliah berhasil diupdate!\n";
        cout << "Notifikasi telah dikirim ke semua dosen.\n";
        cout << "Tekan ENTER untuk kembali...";
        cin.get();
    }

    void hapusMatkul() {
        system("cls");
        lihatSemuaMatkul();
        
        if (jumlahMatkul == 0) {
            cout << "\nTidak ada mata kuliah yang bisa dihapus.\n";
            cout << "Tekan ENTER untuk kembali...";
            cin.get();
            return;
        }
        
        string kodeHapus;
        cout << "\nMasukkan Kode Mata Kuliah yang akan dihapus: ";
        getline(cin, kodeHapus);
        
        int index = -1;
        for (int i = 0; i < jumlahMatkul; i++) {
            if (daftarMatkul[i].kode == kodeHapus) {
                index = i;
                break;
            }
        }
        
        if (index == -1) {
            cout << "Kode mata kuliah tidak ditemukan!\n";
            cout << "Tekan ENTER untuk kembali...";
            cin.get();
            return;
        }
        
        bool adaDiKRS = false;
        for (int i = 0; i < jumlahKRS; i++) {
            if (daftarKRS[i].kodeMatkul == kodeHapus) {
                adaDiKRS = true;
                break;
            }
        }
        
        bool adaDiJadwal = false;
        for (int i = 0; i < jumlahJadwal; i++) {
            if (daftarJadwal[i].kodeMatkul == kodeHapus) {
                adaDiJadwal = true;
                break;
            }
        }
        
        if (adaDiKRS || adaDiJadwal) {
            cout << "\nPERINGATAN: Mata kuliah ini masih digunakan!\n";
            if (adaDiKRS) cout << "- Masih ada di KRS mahasiswa\n";
            if (adaDiJadwal) cout << "- Masih ada di jadwal kuliah\n";
            cout << "\nApakah tetap ingin menghapus? (y/n): ";
            char konfirmasi;
            cin >> konfirmasi;
            cin.ignore(1000, '\n');
            
            if (konfirmasi != 'y' && konfirmasi != 'Y') {
                cout << "Penghapusan dibatalkan.\n";
                cout << "Tekan ENTER untuk kembali...";
                cin.get();
                return;
            }
        }
        
        cout << "\nData yang akan dihapus:\n";
        cout << "Kode: " << daftarMatkul[index].kode << endl;
        cout << "Nama: " << daftarMatkul[index].nama << endl;
        cout << "SKS: " << daftarMatkul[index].sks << endl;
        cout << "Semester: " << daftarMatkul[index].semester << endl;
        cout << "Prodi: " << daftarMatkul[index].prodi << endl;
        
        cout << "\nApakah yakin ingin menghapus? (y/n): ";
        char konfirmasi;
        cin >> konfirmasi;
        cin.ignore(1000, '\n');
        
        if (konfirmasi == 'y' || konfirmasi == 'Y') {
            hapusMatkulGlobal(index);
            simpanMatkulToFile();
            
            // Kirim notifikasi ke dosen
            string pesan = "Mata kuliah " + daftarMatkul[index].nama + " (" + daftarMatkul[index].kode + ") telah dihapus dari sistem.";
            kirimNotifikasiKePenerima("ALL", "DOSEN", pesan, "ADMIN");
            
            cout << "Mata kuliah berhasil dihapus!\n";
            cout << "Notifikasi telah dikirim ke semua dosen.\n";
        } else {
            cout << "Penghapusan dibatalkan.\n";
        }
        
        cout << "Tekan ENTER untuk kembali...";
        cin.get();
    }

    void tambahJadwalKuliah() {
        system("cls");
        cout << "\n+==================================================+\n";
        cout << "|              TAMBAH JADWAL KULIAH               |\n";
        cout << "+==================================================+\n";

        lihatSemuaMatkul();
        
        JadwalKuliah jadwalBaru;
        
        cout << "\nMasukkan Kode Mata Kuliah: ";
        getline(cin, jadwalBaru.kodeMatkul);
        
        Matkul* matkul = nullptr;
        for (int i = 0; i < jumlahMatkul; i++) {
            if (daftarMatkul[i].kode == jadwalBaru.kodeMatkul) {
                matkul = &daftarMatkul[i];
                jadwalBaru.namaMatkul = matkul->nama;
                break;
            }
        }
        
        if (!matkul) {
            cout << "Kode mata kuliah tidak ditemukan!\n";
            cout << "Tekan ENTER untuk kembali...";
            cin.get();
            return;
        }
        
        cout << "Masukkan Semester: ";
        getline(cin, jadwalBaru.semester);
        
        if (jadwalBaru.semester != matkul->semester) {
            cout << "Warning: Semester jadwal (" << jadwalBaru.semester 
                 << ") tidak sesuai dengan semester mata kuliah (" << matkul->semester << ")!\n";
            cout << "Apakah ingin melanjutkan? (y/n): ";
            char lanjut;
            cin >> lanjut;
            cin.ignore(1000, '\n');
            if (lanjut != 'y' && lanjut != 'Y') {
                return;
            }
        }
        
        cout << "Masukkan Kelas (contoh: A, B, C): ";
        getline(cin, jadwalBaru.kelas);
        
        cout << "Masukkan Hari (Senin, Selasa, Rabu, Kamis, Jumat, Sabtu): ";
        getline(cin, jadwalBaru.hari);
        
        string hariValid[] = {"Senin", "Selasa", "Rabu", "Kamis", "Jumat", "Sabtu"};
        bool hariValidFlag = false;
        for (int i = 0; i < 6; i++) {
            if (jadwalBaru.hari == hariValid[i]) {
                hariValidFlag = true;
                break;
            }
        }
        if (!hariValidFlag) {
            cout << "Hari tidak valid! Harus Senin, Selasa, Rabu, Kamis, Jumat, atau Sabtu.\n";
            cout << "Tekan ENTER untuk kembali...";
            cin.get();
            return;
        }
        
        cout << "Masukkan Jam Mulai (format HH:MM, contoh: 08:00): ";
        getline(cin, jadwalBaru.jamMulai);
        
        if (jadwalBaru.jamMulai.length() != 5 || 
            jadwalBaru.jamMulai[2] != ':' ||
            !isdigit(jadwalBaru.jamMulai[0]) || !isdigit(jadwalBaru.jamMulai[1]) ||
            !isdigit(jadwalBaru.jamMulai[3]) || !isdigit(jadwalBaru.jamMulai[4])) {
            cout << "Format jam tidak valid! Gunakan format HH:MM\n";
            cout << "Tekan ENTER untuk kembali...";
            cin.get();
            return;
        }
        
        cout << "Masukkan Jam Selesai (format HH:MM, contoh: 10:00): ";
        getline(cin, jadwalBaru.jamSelesai);
        
        if (jadwalBaru.jamSelesai.length() != 5 || 
            jadwalBaru.jamSelesai[2] != ':' ||
            !isdigit(jadwalBaru.jamSelesai[0]) || !isdigit(jadwalBaru.jamSelesai[1]) ||
            !isdigit(jadwalBaru.jamSelesai[3]) || !isdigit(jadwalBaru.jamSelesai[4])) {
            cout << "Format jam tidak valid! Gunakan format HH:MM\n";
            cout << "Tekan ENTER untuk kembali...";
            cin.get();
            return;
        }
        
        int jamMulai = stoi(jadwalBaru.jamMulai.substr(0, 2));
        int menitMulai = stoi(jadwalBaru.jamMulai.substr(3, 2));
        int jamSelesai = stoi(jadwalBaru.jamSelesai.substr(0, 2));
        int menitSelesai = stoi(jadwalBaru.jamSelesai.substr(3, 2));
        
        if (jamSelesai < jamMulai || (jamSelesai == jamMulai && menitSelesai <= menitMulai)) {
            cout << "Jam selesai harus setelah jam mulai!\n";
            cout << "Tekan ENTER untuk kembali...";
            cin.get();
            return;
        }
        
        cout << "Masukkan Ruangan: ";
        getline(cin, jadwalBaru.ruangan);
        
        cout << "Masukkan Nama Dosen Pengampu: ";
        getline(cin, jadwalBaru.dosen);
        
        if (cekKonflikJadwal(jadwalBaru.hari, jadwalBaru.jamMulai, jadwalBaru.jamSelesai, 
                            jadwalBaru.ruangan, jadwalBaru.semester, jadwalBaru.kelas)) {
            cout << "\nPERINGATAN: Konflik jadwal ditemukan!\n";
            cout << "Mungkin ada jadwal lain pada hari, waktu, dan ruangan yang sama.\n";
            cout << "Apakah tetap ingin menambahkan? (y/n): ";
            char lanjut;
            cin >> lanjut;
            cin.ignore(1000, '\n');
            if (lanjut != 'y' && lanjut != 'Y') {
                return;
            }
        }
        
        tambahJadwal(jadwalBaru);
        simpanJadwalToFile();
        
        // Kirim notifikasi ke dosen terkait
        string pesan = "Jadwal baru untuk mata kuliah " + jadwalBaru.namaMatkul + " telah ditambahkan: " + 
                      jadwalBaru.hari + " " + jadwalBaru.jamMulai + "-" + jadwalBaru.jamSelesai + " di " + jadwalBaru.ruangan;
        
        // Cari dosen berdasarkan nama (sederhana)
        for (int i = 0; i < jumlahAkun; i++) {
            if (daftarAkun[i].role == "dosen" && daftarAkun[i].nama.find(jadwalBaru.dosen) != string::npos) {
                kirimNotifikasiKePenerima(daftarAkun[i].username, "TERSENDIRI", pesan, "ADMIN");
                break;
            }
        }
        
        cout << "\nJadwal berhasil ditambahkan!\n";
        cout << "Notifikasi telah dikirim ke dosen pengampu.\n";
        cout << "Tekan ENTER untuk kembali...";
        cin.get();
    }

    void simpanJadwalToFile() {
        ofstream file("jadwal.txt");
        for (int i = 0; i < jumlahJadwal; i++) {
            file << daftarJadwal[i].kodeMatkul << "|" 
                 << daftarJadwal[i].namaMatkul << "|"
                 << daftarJadwal[i].hari << "|"
                 << daftarJadwal[i].jamMulai << "|"
                 << daftarJadwal[i].jamSelesai << "|"
                 << daftarJadwal[i].ruangan << "|"
                 << daftarJadwal[i].semester << "|"
                 << daftarJadwal[i].kelas << "|"
                 << daftarJadwal[i].dosen << "\n";
        }
        file.close();
    }

    void lihatSemuaJadwal() {
        system("cls");
        cout << "\n+==============================================================================================================+\n";
        cout << "|                                          DAFTAR SEMUA JADWAL KULIAH                                         |\n";
        cout << "+===========+===============================================+========+===========+========+========+===========+\n";
        cout << "| Kode      | Mata Kuliah                                  | Semester| Kelas    | Hari   | Jam    | Ruangan  |\n";
        cout << "+===========+===============================================+========+===========+========+========+===========+\n";
        
        if (jumlahJadwal == 0) {
            cout << "|                                           Belum ada jadwal kuliah                                          |\n";
        } else {
            for (int i = 0; i < jumlahJadwal; i++) {
                string jam = daftarJadwal[i].jamMulai + "-" + daftarJadwal[i].jamSelesai;
                cout << "| " << setw(9) << left << daftarJadwal[i].kodeMatkul
                     << "| " << setw(45) << left << daftarJadwal[i].namaMatkul
                     << "| " << setw(6) << left << daftarJadwal[i].semester
                     << "  | " << setw(7) << left << daftarJadwal[i].kelas
                     << " | " << setw(6) << left << daftarJadwal[i].hari
                     << " | " << setw(6) << left << jam
                     << " | " << setw(7) << left << daftarJadwal[i].ruangan << " |\n";
                cout << "+-----------+-----------------------------------------------+--------+-----------+--------+--------+-----------+\n";
            }
        }
        
        cout << "\nTekan ENTER untuk kembali...";
        cin.get();
    }

    void hapusJadwalKuliah() {
        system("cls");
        lihatSemuaJadwal();
        
        if (jumlahJadwal == 0) {
            cout << "\nTidak ada jadwal yang bisa dihapus.\n";
            cout << "Tekan ENTER untuk kembali...";
            cin.get();
            return;
        }
        
        string kodeHapus, kelasHapus;
        cout << "\nMasukkan Kode Mata Kuliah yang akan dihapus jadwalnya: ";
        getline(cin, kodeHapus);
        
        cout << "Masukkan Kelas: ";
        getline(cin, kelasHapus);
        
        int index = -1;
        for (int i = 0; i < jumlahJadwal; i++) {
            if (daftarJadwal[i].kodeMatkul == kodeHapus && daftarJadwal[i].kelas == kelasHapus) {
                index = i;
                break;
            }
        }
        
        if (index == -1) {
            cout << "Jadwal tidak ditemukan!\n";
            cout << "Pastikan kode mata kuliah dan kelas benar.\n";
        } else {
            cout << "\nJadwal yang akan dihapus:\n";
            cout << "Mata Kuliah: " << daftarJadwal[index].namaMatkul << endl;
            cout << "Kelas: " << daftarJadwal[index].kelas << endl;
            cout << "Hari: " << daftarJadwal[index].hari << endl;
            cout << "Jam: " << daftarJadwal[index].jamMulai << "-" << daftarJadwal[index].jamSelesai << endl;
            cout << "Ruangan: " << daftarJadwal[index].ruangan << endl;
            
            cout << "\nApakah yakin ingin menghapus? (y/n): ";
            char konfirmasi;
            cin >> konfirmasi;
            cin.ignore(1000, '\n');
            
            if (konfirmasi == 'y' || konfirmasi == 'Y') {
                hapusJadwal(index);
                simpanJadwalToFile();
                
                // Kirim notifikasi ke dosen
                string pesan = "Jadwal untuk " + daftarJadwal[index].namaMatkul + " kelas " + 
                              daftarJadwal[index].kelas + " telah dihapus.";
                kirimNotifikasiKePenerima("ALL", "DOSEN", pesan, "ADMIN");
                
                cout << "Jadwal berhasil dihapus!\n";
                cout << "Notifikasi telah dikirim ke semua dosen.\n";
            } else {
                cout << "Penghapusan dibatalkan.\n";
            }
        }
        
        cout << "Tekan ENTER untuk kembali...";
        cin.get();
    }

    void editProfil() {
        system("cls"); 
        int pilih;
        string input;
        cout << "+==================================================+\n";
        cout << "|                     EDIT PROFIL                  |\n";
        cout << "+==================================================+\n";
        cout << "Data Anda Saat Ini (Role: " << akunAdmin->role << ")\n";
        cout << "1. Nama          : " << akunAdmin->nama << "\n";
        cout << "2. ID Admin      : " << akunAdmin->nim << "\n";
        cout << "3. Departemen    : " << akunAdmin->prodi << "\n";
        cout << "4. Password      : [RAHASIA]\n";
        cout << "5. Kembali\n";
        cout << "+--------------------------------------------------+\n";
        cout << "Apa yang ingin Anda ubah? (1-4): ";
        
        int inputPilih;
        cin >> inputPilih;
        pilih = inputPilih;
        cin.ignore(1000, '\n');

        switch(pilih) {
            case 1:
                cout << "Masukkan Nama baru: ";
                getline(cin, akunAdmin->nama);
                cout << "Nama berhasil diubah!\n";
                break;
            case 2:
                cout << "Masukkan ID Admin baru: ";
                long long int newID;
                cin >> newID;
                akunAdmin->nim = newID;
                cin.ignore(1000, '\n');
                cout << "ID Admin berhasil diubah!\n";
                break;
            case 3:
                cout << "Masukkan Departemen baru: ";
                getline(cin, akunAdmin->prodi);
                cout << "Departemen berhasil diubah!\n";
                break;
            case 4:
                cout << "Masukkan Password baru: ";
                getline(cin, akunAdmin->password);
                cout << "Password berhasil diubah!\n";
                break;
            case 5:
                return;
            default:
                cout << "Pilihan tidak valid.\n";
        }
        
        simpanSemuaAkunKeFile();
        cout << "Tekan ENTER untuk kembali...";
        cin.get();
    }

    void menuKirimNotifikasi() {
        int pilih;
        do {
            system("cls");
            cout << "+==================================================+\n";
            cout << "|                KIRIM NOTIFIKASI                 |\n";
            cout << "+==================================================+\n";
            cout << "1. Kirim ke Semua Pengguna\n";
            cout << "2. Kirim ke Semua Mahasiswa\n";
            cout << "3. Kirim ke Semua Dosen\n";
            cout << "4. Kirim ke Pengguna Tertentu\n";
            cout << "5. Kembali ke Menu Admin\n";
            cout << "+--------------------------------------------------+\n";
            cout << "Pilih :: ";
            
            int input;
            cin >> input;
            pilih = input;
            cin.ignore(1000, '\n');
            
            string pesan, penerima, kategori;
            
            switch(pilih) {
                case 1:
                    kategori = "SEMUA";
                    break;
                case 2:
                    kategori = "MAHASISWA";
                    break;
                case 3:
                    kategori = "DOSEN";
                    break;
                case 4:
                    kategori = "TERSENDIRI";
                    break;
                case 5:
                    return;
                default:
                    cout << "Pilihan tidak valid!\n";
                    cout << "Tekan ENTER untuk melanjutkan...";
                    cin.get();
                    continue;
            }
            
            system("cls");
            cout << "+==================================================+\n";
            cout << "|             TULIS PESAN NOTIFIKASI              |\n";
            cout << "+==================================================+\n";
            
            if (pilih == 4) {
                cout << "Masukkan username atau NIM penerima: ";
                getline(cin, penerima);
            } else {
                penerima = kategori;
            }
            
            cout << "\nMasukkan pesan notifikasi (maks 200 karakter):\n";
            cout << ">> ";
            getline(cin, pesan);
            
            if (pesan.empty()) {
                cout << "Pesan tidak boleh kosong!\n";
            } else {
                kirimNotifikasiKePenerima(penerima, (pilih == 4) ? "TERSENDIRI" : kategori, 
                                         pesan, akunAdmin->username);
                cout << "\nTekan ENTER untuk melanjutkan...";
                cin.get();
            }
            
        } while (pilih != 5);
    }

    void lihatSemuaNotifikasiAdmin() {
        system("cls");
        cout << "\n+================================================================================+\n";
        cout << "|                         RIWAYAT NOTIFIKASI (ADMIN)                           |\n";
        cout << "+================================================================================+\n";
        
        if (isQueueEmpty()) {
            cout << "|                              Tidak ada notifikasi                               |\n";
        } else {
            cout << "+----+---------------------+---------------------+-------------------------------+--------+\n";
            cout << "| No | Pengirim            | Penerima            | Pesan                         | Status |\n";
            cout << "+----+---------------------+---------------------+-------------------------------+--------+\n";
            
            int index = queueNotifikasi.front;
            for (int i = 0; i < queueNotifikasi.count; i++) {
                Notifikasi notif = queueNotifikasi.data[index];
                
                string pesan = notif.pesan;
                if (pesan.length() > 25) {
                    pesan = pesan.substr(0, 22) + "...";
                }
                
                string penerima = notif.penerima;
                if (penerima == "ALL") penerima = "Semua";
                else if (penerima == "MAHASISWA") penerima = "Mahasiswa";
                else if (penerima == "DOSEN") penerima = "Dosen";
                
                cout << "| " << setw(2) << right << (i + 1) << " "
                     << "| " << setw(19) << left << notif.pengirim
                     << "| " << setw(19) << left << penerima
                     << "| " << setw(29) << left << pesan
                     << "| " << setw(6) << left << notif.status << "|\n";
                
                index = (index + 1) % queueNotifikasi.kapasitas;
            }
            cout << "+----+---------------------+---------------------+-------------------------------+--------+\n";
        }
        
        cout << "\nTotal Notifikasi: " << queueNotifikasi.count << "\n";
        cout << "\nTekan ENTER untuk kembali...";
        cin.get();
    }

    void hapusNotifikasiTertentu() {
        system("cls");
        lihatSemuaNotifikasiAdmin();
        
        if (isQueueEmpty()) {
            cout << "\nTidak ada notifikasi untuk dihapus.\n";
            cout << "Tekan ENTER untuk kembali...";
            cin.get();
            return;
        }
        
        cout << "\nMasukkan nomor notifikasi yang akan dihapus (0 untuk batal): ";
        int nomor;
        cin >> nomor;
        cin.ignore(1000, '\n');
        
        if (nomor <= 0 || nomor > queueNotifikasi.count) {
            cout << "Nomor tidak valid!\n";
            cout << "Tekan ENTER untuk kembali...";
            cin.get();
            return;
        }
        
        // Karena queue, kita perlu membuat queue baru tanpa notifikasi yang dihapus
        QueueNotifikasi queueBaru;
        queueBaru.kapasitas = queueNotifikasi.kapasitas;
        queueBaru.data = new Notifikasi[queueBaru.kapasitas];
        queueBaru.front = -1;
        queueBaru.rear = -1;
        queueBaru.count = 0;
        
        int index = queueNotifikasi.front;
        for (int i = 0; i < queueNotifikasi.count; i++) {
            if (i + 1 != nomor) { // Skip notifikasi yang akan dihapus
                if (queueBaru.count == 0) {
                    queueBaru.front = 0;
                    queueBaru.rear = 0;
                } else {
                    queueBaru.rear = (queueBaru.rear + 1) % queueBaru.kapasitas;
                }
                queueBaru.data[queueBaru.rear] = queueNotifikasi.data[index];
                queueBaru.count++;
            }
            index = (index + 1) % queueNotifikasi.kapasitas;
        }
        
        // Ganti queue lama dengan queue baru
        delete[] queueNotifikasi.data;
        queueNotifikasi = queueBaru;
        
        simpanNotifikasiKeFile();
        
        cout << "\nNotifikasi berhasil dihapus!\n";
        cout << "Tekan ENTER untuk kembali...";
        cin.get();
    }

public:
    Admin(Akun* akun) {
        akunAdmin = akun;
        topUndo = -1;
        kapasitasUndo = 10;
        undoStack = new DataHapus[kapasitasUndo];
        chatSystem.setCurrentUser(akun->username);
        muatUndoDariFile();
    }

    ~Admin() {
        if (undoStack) {
            delete[] undoStack;
        }
    }

    void menuAdmin() {
        // VALIDASI: Pastikan akunAdmin tidak null
        if (!akunAdmin) {
            cout << "ERROR: Admin account not found!\n";
            return;
        }
        
        // VALIDASI: Pastikan undoStack terinisialisasi
        if (!undoStack) {
            cout << "ERROR: Undo stack not initialized!\n";
            // Coba inisialisasi ulang
            kapasitasUndo = 10;
            undoStack = new DataHapus[kapasitasUndo];
            topUndo = -1;
        }
        
        int pilih;
        do {
            // Hitung notifikasi belum dibaca untuk admin
            int notifBelumDibaca = hitungNotifikasiBelumDibaca(akunAdmin->username);
            
            cout << "\n+==================================================+\n";
            cout << "            PORTAL ADMIN - " << akunAdmin->nama << "            \n";
            if (notifBelumDibaca > 0) {
                cout << "|  NOTIFIKASI: " << notifBelumDibaca << " belum dibaca" << setw(26) << " |\n";
            }
            cout << "+==================================================+\n";
            cout << "1. Buat Akun Pengguna Baru\n";
            cout << "2. Lihat Semua Akun Pengguna\n";           
            cout << "3. Hapus Akun Pengguna\n";
            cout << "4. Lihat Riwayat Akun yang dihapus\n";
            cout << "5. Kembalikan Akun yang dihapus terakhir kali \n";              
            cout << "6. Manajemen Buku Perpustakaan\n"; 
            cout << "7. Manajemen Mata Kuliah & Jadwal\n";
            cout << "8. Kirim Notifikasi\n";
            cout << "9. Lihat Riwayat Notifikasi\n";
            cout << "10. Hapus Notifikasi\n";
            cout << "11. Sistem Chat & Pesan\n";
            cout << "12. Baca Notifikasi Saya\n";
            cout << "13. Edit Profil\n";
            cout << "14. Statistik Mahasiswa\n";
            cout << "0. Logout\n";
            cout << "+--------------------------------------------------+\n";
            cout << "Pilih :: ";

            int input;
            cin >> input;
            pilih = input;
            cin.ignore(1000, '\n');

            switch (pilih) {
            case 1:
                menuBuatAkun();
                break;
            case 2:
                lihatSemuaAkun(); 
                break;
            case 3:
                hapusAkunPengguna(); 
                break;
            case 4:
                riwayatAkunDihapus();
                break;
            case 5: 
                kembalikanAkun();
                break;
            case 6:
                menuManajemenBuku(); 
                break;
            case 7:
                menuManajemenMatkul();
                break;
            case 8:
                menuKirimNotifikasi();
                break;
            case 9:
                lihatSemuaNotifikasiAdmin();
                break;
            case 10:
                hapusNotifikasiTertentu();
                break;
            case 11:
                chatSystem.menuChat();
                break;
            case 12:
                tampilkanNotifikasiUntukUser(akunAdmin->username);
                break;
            case 13:
                editProfil();
                break;
            case 14:
                statistikMahasiswa();
                break;
            case 0:
                cout << "\nAnda telah logout. Terima kasih!\n"; 
                break;
            default:
                cout << "Pilihan tidak valid!\n"; 
                cout << "Tekan ENTER untuk melanjutkan...";
                cin.get();
            }
        } while (pilih != 0);
    }
};

// =====================================================================
// CLASS LOGIN
// =====================================================================
class Login {
private:
    Perkuliahan perkuliahan;     
    Kemahasiswaan kemahasiswaan;
    ChatSystem chatSystem;
    
    struct LoginAttempt {
        string username;
        int attempts;
    };
    
    LoginAttempt* loginGagal;
    int jumlahLoginGagal;
    int kapasitasLoginGagal;

    string fileAkunMahasiswa;
    string fileAkunDosen;
    string fileAkunAdmin;

    int getLoginAttempts(const string& username) {
        for (int i = 0; i < jumlahLoginGagal; i++) {
            if (loginGagal[i].username == username) {
                return loginGagal[i].attempts;
            }
        }
        return 0;
    }

    void setLoginAttempts(const string& username, int attempts) {
        for (int i = 0; i < jumlahLoginGagal; i++) {
            if (loginGagal[i].username == username) {
                loginGagal[i].attempts = attempts;
                return;
            }
        }
        
        if (jumlahLoginGagal >= kapasitasLoginGagal) {
            kapasitasLoginGagal = kapasitasLoginGagal == 0 ? 10 : kapasitasLoginGagal * 2;
            LoginAttempt* temp = new LoginAttempt[kapasitasLoginGagal];
            for (int i = 0; i < jumlahLoginGagal; i++) {
                temp[i] = loginGagal[i];
            }
            delete[] loginGagal;
            loginGagal = temp;
        }
        
        loginGagal[jumlahLoginGagal].username = username;
        loginGagal[jumlahLoginGagal].attempts = attempts;
        jumlahLoginGagal++;
    }

    void resetLoginAttempts(const string& username) {
        for (int i = 0; i < jumlahLoginGagal; i++) {
            if (loginGagal[i].username == username) {
                for (int j = i; j < jumlahLoginGagal - 1; j++) {
                    loginGagal[j] = loginGagal[j + 1];
                }
                jumlahLoginGagal--;
                break;
            }
        }
    }

    void lupaPassword() {
        system("cls"); 
        string uname, jawaban;
        cout << "\n=== Lupa Password ===\n";
        cout << "Masukkan username Anda: ";
        getline(cin, uname);

        Akun* curr = cariAkunByUsername(uname);
        if (curr) {
            cout << "Pertanyaan keamanan: " << curr->pertanyaan << endl;
            cout << "Jawaban Anda: ";
            getline(cin, jawaban);

            if (jawaban == curr->jawaban) {
                cout << "\nJawaban benar. Password Anda adalah: " << curr->password << endl;
            } else {
                cout << "\nJawaban salah!\n";
            }
        } else {
            cout << "\nUsername tidak ditemukan.\n";
        }
        cout << "Tekan ENTER untuk kembali...";
        cin.get();
    }

    void editProfil(Akun* akun) {
        system("cls"); 
        int pilih;
        string input;
        cout << "+==================================================+\n";
        cout << "|                     EDIT PROFIL                  |\n";
        cout << "+==================================================+\n";
        cout << "Data Anda Saat Ini (Role: " << akun->role << ")\n";
        cout << "1. Nama          : " << akun->nama << "\n";
        cout << "2. ID (NIM/NIP)  : " << akun->nim << "\n";
        cout << "3. Prodi/Dept    : " << akun->prodi << "\n";
        cout << "4. Password      : [RAHASIA]\n";
        cout << "5. Kembali\n";
        cout << "+--------------------------------------------------+\n";
        cout << "Apa yang ingin Anda ubah? (1-4): ";
        
        int inputPilih;
        cin >> inputPilih;
        pilih = inputPilih;
        cin.ignore(1000, '\n');

        switch(pilih) {
            case 1:
                cout << "Masukkan Nama baru: ";
                getline(cin, akun->nama);
                cout << "Nama berhasil diubah!\n";
                break;
            case 2:
                cout << "Masukkan ID (NIM/NIP) baru: ";
                long long int newID;
                cin >> newID;
                akun->nim = newID;
                cin.ignore(1000, '\n');
                cout << "ID berhasil diubah!\n";
                break;
            case 3:
                cout << "Masukkan Prodi/Dept baru: ";
                getline(cin, akun->prodi);
                cout << "Prodi/Dept berhasil diubah!\n";
                break;
            case 4:
                cout << "Masukkan Password baru: ";
                getline(cin, akun->password);
                cout << "Password berhasil diubah!\n";
                break;
            case 5:
                return;
            default:
                cout << "Pilihan tidak valid.\n";
        }
        
        simpanAkunToFile(akun->role);
        
        if (akun->role == "mahasiswa") {
            perkuliahan.setMahasiswaInfo(akun->nama, akun->nim, akun->prodi);
            kemahasiswaan.setMahasiswaInfo(akun->nama, akun->nim);
        }

        cout << "Tekan ENTER untuk kembali...";
        cin.get();
    }

    void simpanAkunToFile(const string& role) {
        string filename;
        if (role == "mahasiswa") filename = "akun_mahasiswa.txt";
        else if (role == "dosen") filename = "akun_dosen.txt";
        else if (role == "admin") filename = "akun_admin.txt";
        else return;
        
        ofstream file(filename.c_str(), ios::trunc);
        for (int i = 0; i < jumlahAkun; i++) {
            if (daftarAkun[i].role == role) {
                file << daftarAkun[i].username << "|"
                     << daftarAkun[i].password << "|"
                     << daftarAkun[i].nama << "|"
                     << daftarAkun[i].nim << "|"
                     << daftarAkun[i].prodi << "|"
                     << daftarAkun[i].pertanyaan << "|"
                     << daftarAkun[i].jawaban << "|"
                     << daftarAkun[i].role << "\n";
            }
        }
        file.close();
    }

    void menuMahasiswa(Akun* akun) {
        int pilih;
        do {
            // Hitung notifikasi belum dibaca
            int notifBelumDibaca = hitungNotifikasiBelumDibaca(akun->username);
            
            cout << "\n+==================================================+\n";
            cout << "         PORTAL MAHASISWA - " << akun->nama << "         \n";
            if (notifBelumDibaca > 0) {
                cout << "|  NOTIFIKASI: " << notifBelumDibaca << " belum dibaca" << setw(26) << " |\n";
            }
            cout << "+==================================================+\n";
            cout << "1. Menu Perkuliahan (KRS, KHS, IPK)\n";
            cout << "2. Menu Kemahasiswaan (Surat, P2K, Perpus)\n";
            cout << "3. Lihat Jadwal Kuliah\n";
            cout << "4. Sistem Chat & Pesan\n";
            cout << "5. Baca Notifikasi\n";
            cout << "6. Menu Lain-lain(Wifi, Kelulusan, dan Sertifikasi Profesi)\n"; 
            cout << "7. Edit Profil\n"; 
            cout << "8. Logout\n";
            cout << "+--------------------------------------------------+\n";
            cout << "Pilih :: ";
            
            int input;
            cin >> input;
            pilih = input;
            cin.ignore(1000, '\n');

            switch(pilih) {
                case 1:
                    perkuliahan.menuPerkuliahan();
                    break;
                case 2:
                    kemahasiswaan.menuKemahasiswaan();
                    break;
                case 3:
                    perkuliahan.tampilkanJadwalKuliahMahasiswa(akun->nim);
                    break;
                case 4:
                    chatSystem.setCurrentUser(akun->username);
                    chatSystem.menuChat();
                    break;
                case 5:
                    tampilkanNotifikasiUntukUser(akun->username);
                    break;
                case 6:
                    perkuliahan.lain_lain();
                    break;
                case 7:
                    editProfil(akun);
                    break;
                case 8:
                    cout << "\nAnda telah logout. Terima kasih!\n"; 
                    break;
                default:
                    cout << "Pilihan tidak valid!\n"; 
            }
        } while (pilih != 8);
    }

    void loadDataFromFiles() {
        // Pastikan SEMUA file akun dimuat
        loadAkunFromFile("akun_mahasiswa.txt");
        loadAkunFromFile("akun_dosen.txt");
        loadAkunFromFile("akun_admin.txt");
        
        loadBukuFromFile();
        loadPinjamanFromFile();
        loadKRSFromFile();
        loadNilaiFromFile();
        loadPresensiFromFile();
        loadPengajuanFromFile();
        loadPembayaranFromFile();
        loadJadwalFromFile();
        loadMatkulFromFile();
    }

    void loadAkunFromFile(const string& filename) {
        ifstream file(filename.c_str());
        if (!file) {
            // File tidak ada (biasa terjadi pertama kali)
            return;
        }

        string line;
        while (getline(file, line)) {
            if (line.empty()) continue;
            
            Akun baru;
            int pos1 = 0, pos2 = 0;
            
            pos2 = line.find('|', pos1);
            if (pos2 == string::npos) continue;
            baru.username = line.substr(pos1, pos2 - pos1);
            
            pos1 = pos2 + 1;
            pos2 = line.find('|', pos1);
            if (pos2 == string::npos) continue;
            baru.password = line.substr(pos1, pos2 - pos1);
            
            pos1 = pos2 + 1;
            pos2 = line.find('|', pos1);
            if (pos2 == string::npos) continue;
            baru.nama = line.substr(pos1, pos2 - pos1);
            
            pos1 = pos2 + 1;
            pos2 = line.find('|', pos1);
            if (pos2 == string::npos) continue;
            string nim_str = line.substr(pos1, pos2 - pos1);
            baru.nim = safe_stoll(nim_str);
            
            pos1 = pos2 + 1;
            pos2 = line.find('|', pos1);
            if (pos2 == string::npos) continue;
            baru.prodi = line.substr(pos1, pos2 - pos1);
            
            pos1 = pos2 + 1;
            pos2 = line.find('|', pos1);
            if (pos2 == string::npos) continue;
            baru.pertanyaan = line.substr(pos1, pos2 - pos1);
            
            pos1 = pos2 + 1;
            pos2 = line.find('|', pos1);
            if (pos2 == string::npos) continue;
            baru.jawaban = line.substr(pos1, pos2 - pos1);
            
            pos1 = pos2 + 1;
            baru.role = line.substr(pos1);
            
            // Periksa apakah akun sudah ada sebelum menambah
            bool sudahAda = false;
            for (int i = 0; i < jumlahAkun; i++) {
                if (daftarAkun[i].username == baru.username) {
                    sudahAda = true;
                    break;
                }
            }
            
            if (!sudahAda) {
                tambahAkun(baru);
            }
        }
        file.close();
    }

    void loadBukuFromFile() {
        ifstream file("buku.txt");
        if (!file) return;

        string line;
        while (getline(file, line)) {
            if (line.empty()) continue;
            
            BukuMaster buku;
            int pos1 = 0, pos2 = 0;
            
            pos2 = line.find('|', pos1);
            if (pos2 == string::npos) continue;
            buku.kode = line.substr(pos1, pos2 - pos1);
            
            pos1 = pos2 + 1;
            pos2 = line.find('|', pos1);
            if (pos2 == string::npos) continue;
            buku.judul = line.substr(pos1, pos2 - pos1);
            
            pos1 = pos2 + 1;
            string stok_str = line.substr(pos1);
            
            buku.stok = (int)safe_stoll(stok_str);
            
            tambahBuku(buku);
        }
        file.close();
    }

    void loadPinjamanFromFile() {
        ifstream file("pinjaman.txt");
        if (!file) return;

        string line;
        while (getline(file, line)) {
            if (line.empty()) continue;
            
            BukuPinjam pinjam;
            int pos1 = 0, pos2 = 0;
            
            pos2 = line.find('|', pos1);
            if (pos2 == string::npos) continue;
            string nim_str = line.substr(pos1, pos2 - pos1);
            pinjam.nim = safe_stoll(nim_str);
            
            pos1 = pos2 + 1;
            pos2 = line.find('|', pos1);
            if (pos2 == string::npos) continue;
            pinjam.kode = line.substr(pos1, pos2 - pos1);
            
            pos1 = pos2 + 1;
            pos2 = line.find('|', pos1);
            if (pos2 == string::npos) continue;
            pinjam.judul = line.substr(pos1, pos2 - pos1);
            
            pos1 = pos2 + 1;
            pinjam.tglPinjamStr = line.substr(pos1);
            
            tambahPinjaman(pinjam);
        }
        file.close();
    }

    void loadKRSFromFile() {
        ifstream file("krs.txt");
        if (!file) return;

        string line;
        while (getline(file, line)) {
            if (line.empty()) continue;
            
            KRSMahasiswa krs;
            int pos1 = 0, pos2 = 0;
            
            pos2 = line.find('|', pos1);
            if (pos2 == string::npos) continue;
            string nim_str = line.substr(pos1, pos2 - pos1);
            krs.nim = safe_stoll(nim_str);
            
            pos1 = pos2 + 1;
            pos2 = line.find('|', pos1);
            if (pos2 == string::npos) continue;
            krs.kodeMatkul = line.substr(pos1, pos2 - pos1);
            
            pos1 = pos2 + 1;
            pos2 = line.find('|', pos1);
            if (pos2 == string::npos) continue;
            krs.namaMatkul = line.substr(pos1, pos2 - pos1);
            
            pos1 = pos2 + 1;
            string sks_str = line.substr(pos1);
            
            krs.sks = (int)safe_stoll(sks_str);
            
            tambahKRS(krs);
        }
        file.close();
    }

    void loadNilaiFromFile() {
        ifstream file("nilai.txt");
        if (!file) return;

        string line;
        while (getline(file, line)) {
            if (line.empty()) continue;
            
            NilaiMahasiswa nilai;
            int pos1 = 0, pos2 = 0;
            
            pos2 = line.find('|', pos1);
            if (pos2 == string::npos) continue;
            string nim_str = line.substr(pos1, pos2 - pos1);
            nilai.nim = safe_stoll(nim_str);
            
            pos1 = pos2 + 1;
            pos2 = line.find('|', pos1);
            if (pos2 == string::npos) continue;
            nilai.kodeMatkul = line.substr(pos1, pos2 - pos1);
            
            pos1 = pos2 + 1;
            pos2 = line.find('|', pos1);
            if (pos2 == string::npos) continue;
            nilai.namaMatkul = line.substr(pos1, pos2 - pos1);
            
            pos1 = pos2 + 1;
            pos2 = line.find('|', pos1);
            if (pos2 == string::npos) continue;
            string sks_str = line.substr(pos1, pos2 - pos1);
            nilai.sks = (int)safe_stoll(sks_str);
            
            pos1 = pos2 + 1;
            nilai.nilai = line.substr(pos1);
            
            if (jumlahNilai >= kapasitasNilai) {
                kapasitasNilai = kapasitasNilai == 0 ? 10 : kapasitasNilai * 2;
                NilaiMahasiswa* temp = new NilaiMahasiswa[kapasitasNilai];
                for (int i = 0; i < jumlahNilai; i++) {
                    temp[i] = daftarNilai[i];
                }
                delete[] daftarNilai;
                daftarNilai = temp;
            }
            daftarNilai[jumlahNilai++] = nilai;
        }
        file.close();
    }

    void loadPresensiFromFile() {
        ifstream file("presensi.txt");
        if (!file) return;

        string line;
        while (getline(file, line)) {
            if (line.empty()) continue;
            
            Presensi presensi;
            int pos1 = 0, pos2 = 0;
            
            pos2 = line.find('|', pos1);
            if (pos2 == string::npos) continue;
            string nim_str = line.substr(pos1, pos2 - pos1);
            presensi.nim = safe_stoll(nim_str);
            
            pos1 = pos2 + 1;
            pos2 = line.find('|', pos1);
            if (pos2 == string::npos) continue;
            presensi.kodeMatkul = line.substr(pos1, pos2 - pos1);
            
            pos1 = pos2 + 1;
            pos2 = line.find('|', pos1);
            if (pos2 == string::npos) continue;
            presensi.namaMatkul = line.substr(pos1, pos2 - pos1);
            
            pos1 = pos2 + 1;
            pos2 = line.find('|', pos1);
            if (pos2 == string::npos) continue;
            string hadir_str = line.substr(pos1, pos2 - pos1);
            presensi.hadir = (int)safe_stoll(hadir_str);
            
            pos1 = pos2 + 1;
            pos2 = line.find('|', pos1);
            if (pos2 == string::npos) continue;
            string sakit_str = line.substr(pos1, pos2 - pos1);
            presensi.sakit = (int)safe_stoll(sakit_str);
            
            pos1 = pos2 + 1;
            pos2 = line.find('|', pos1);
            if (pos2 == string::npos) continue;
            string izin_str = line.substr(pos1, pos2 - pos1);
            presensi.izin = (int)safe_stoll(izin_str);
            
            pos1 = pos2 + 1;
            string alpha_str = line.substr(pos1);
            presensi.alpha = (int)safe_stoll(alpha_str);
            
            if (jumlahPresensi >= kapasitasPresensi) {
                kapasitasPresensi = kapasitasPresensi == 0 ? 10 : kapasitasPresensi * 2;
                Presensi* temp = new Presensi[kapasitasPresensi];
                for (int i = 0; i < jumlahPresensi; i++) {
                    temp[i] = daftarPresensi[i];
                }
                delete[] daftarPresensi;
                daftarPresensi = temp;
            }
            daftarPresensi[jumlahPresensi++] = presensi;
        }
        file.close();
    }

    void loadPengajuanFromFile() {
        ifstream file("pengajuan.txt");
        if (!file) return;

        string line;
        while (getline(file, line)) {
            if (line.empty()) continue;
            
            Pengajuan pengajuan;
            int pos1 = 0, pos2 = 0;
            
            pos2 = line.find('|', pos1);
            if (pos2 == string::npos) continue;
            string nim_str = line.substr(pos1, pos2 - pos1);
            pengajuan.nim = safe_stoll(nim_str);
            
            pos1 = pos2 + 1;
            pos2 = line.find('|', pos1);
            if (pos2 == string::npos) continue;
            pengajuan.jenis = line.substr(pos1, pos2 - pos1);
            
            pos1 = pos2 + 1;
            pos2 = line.find('|', pos1);
            if (pos2 == string::npos) continue;
            pengajuan.tanggal = line.substr(pos1, pos2 - pos1);
            
            pos1 = pos2 + 1;
            pengajuan.status = line.substr(pos1);
            
            if (jumlahPengajuan >= kapasitasPengajuan) {
                kapasitasPengajuan = kapasitasPengajuan == 0 ? 10 : kapasitasPengajuan * 2;
                Pengajuan* temp = new Pengajuan[kapasitasPengajuan];
                for (int i = 0; i < jumlahPengajuan; i++) {
                    temp[i] = daftarPengajuan[i];
                }
                delete[] daftarPengajuan;
                daftarPengajuan = temp;
            }
            daftarPengajuan[jumlahPengajuan++] = pengajuan;
        }
        file.close();
    }

    void loadPembayaranFromFile() {
        ifstream file("pembayaran_detail.txt");
        if (!file) return;

        string line;
        while (getline(file, line)) {
            if (line.empty()) continue;
            
            Pembayaran bayar;
            int pos1 = 0, pos2 = 0;
            
            pos2 = line.find('|', pos1);
            if (pos2 == string::npos) continue;
            string nim_str = line.substr(pos1, pos2 - pos1);
            bayar.nim = safe_stoll(nim_str);
            
            pos1 = pos2 + 1;
            pos2 = line.find('|', pos1);
            if (pos2 == string::npos) continue;
            bayar.tanggal = line.substr(pos1, pos2 - pos1);
            
            pos1 = pos2 + 1;
            pos2 = line.find('|', pos1);
            if (pos2 == string::npos) continue;
            string nominal_str = line.substr(pos1, pos2 - pos1);
            bayar.nominal = safe_stoll(nominal_str);
            
            pos1 = pos2 + 1;
            bayar.keterangan = line.substr(pos1);
            
            if (jumlahPembayaran >= kapasitasPembayaran) {
                kapasitasPembayaran = kapasitasPembayaran == 0 ? 10 : kapasitasPembayaran * 2;
                Pembayaran* temp = new Pembayaran[kapasitasPembayaran];
                for (int i = 0; i < jumlahPembayaran; i++) {
                    temp[i] = daftarPembayaran[i];
                }
                delete[] daftarPembayaran;
                daftarPembayaran = temp;
            }
            daftarPembayaran[jumlahPembayaran++] = bayar;
        }
        file.close();
    }

    void loadJadwalFromFile() {
        ifstream file("jadwal.txt");
        if (!file) return;

        string line;
        while (getline(file, line)) {
            if (line.empty()) continue;
            
            JadwalKuliah jadwal;
            int pos1 = 0, pos2 = 0;
            
            pos2 = line.find('|', pos1);
            jadwal.kodeMatkul = line.substr(pos1, pos2 - pos1);
            
            pos1 = pos2 + 1;
            pos2 = line.find('|', pos1);
            jadwal.namaMatkul = line.substr(pos1, pos2 - pos1);
            
            pos1 = pos2 + 1;
            pos2 = line.find('|', pos1);
            jadwal.hari = line.substr(pos1, pos2 - pos1);
            
            pos1 = pos2 + 1;
            pos2 = line.find('|', pos1);
            jadwal.jamMulai = line.substr(pos1, pos2 - pos1);
            
            pos1 = pos2 + 1;
            pos2 = line.find('|', pos1);
            jadwal.jamSelesai = line.substr(pos1, pos2 - pos1);
            
            pos1 = pos2 + 1;
            pos2 = line.find('|', pos1);
            jadwal.ruangan = line.substr(pos1, pos2 - pos1);
            
            pos1 = pos2 + 1;
            pos2 = line.find('|', pos1);
            jadwal.semester = line.substr(pos1, pos2 - pos1);
            
            pos1 = pos2 + 1;
            pos2 = line.find('|', pos1);
            jadwal.kelas = line.substr(pos1, pos2 - pos1);
            
            pos1 = pos2 + 1;
            jadwal.dosen = line.substr(pos1);
            
            tambahJadwal(jadwal);
        }
        file.close();
    }

    void loadMatkulFromFile() {
        ifstream file("matkul.txt");
        if (!file) {
            perkuliahan.inisialisasiTabelMatkul();
            return;
        }

        string line;
        while (getline(file, line)) {
            if (line.empty()) continue;
            
            Matkul matkul;
            int pos1 = 0, pos2 = 0;
            
            pos2 = line.find('|', pos1);
            matkul.kode = line.substr(pos1, pos2 - pos1);
            
            pos1 = pos2 + 1;
            pos2 = line.find('|', pos1);
            matkul.nama = line.substr(pos1, pos2 - pos1);
            
            pos1 = pos2 + 1;
            pos2 = line.find('|', pos1);
            string sks_str = line.substr(pos1, pos2 - pos1);
            matkul.sks = (int)safe_stoll(sks_str);
            
            pos1 = pos2 + 1;
            pos2 = line.find('|', pos1);
            matkul.semester = line.substr(pos1, pos2 - pos1);
            
            pos1 = pos2 + 1;
            matkul.prodi = line.substr(pos1);
            
            tambahMatkulGlobal(matkul);
        }
        file.close();
    }

    void login(){
        system("cls"); 
        string username, password;
        cout << "\n=== LOGIN ===\n";
        cout << "Username : ";
        getline(cin, username);

        int attempts = getLoginAttempts(username);
        if (attempts >= 3) {
            cout << "Akun Anda terkunci sementara karena 3x gagal login.\n";
            cout << "Silakan hubungi admin atau coba lagi nanti.\n";
            cout << "Tekan ENTER untuk kembali...";
            cin.get();
            return;
        }

        cout << "Password : ";
        getline(cin, password);

        Akun* akunLogin = cariAkunByUsername(username);
        
        if (akunLogin && akunLogin->password == password) {
            resetLoginAttempts(username);
            cout << "\nLogin berhasil! Selamat datang, " << akunLogin->nama << "!\n";
            cout << "Role Anda: " << akunLogin->role << "\n";
            
            // Tampilkan jumlah notifikasi belum dibaca
            int notifBelumDibaca = hitungNotifikasiBelumDibaca(username);
            if (notifBelumDibaca > 0) {
                cout << "Anda memiliki " << notifBelumDibaca << " notifikasi belum dibaca.\n";
            }
            
            cout << "Tekan ENTER untuk masuk ke portal...";
            cin.get();
            
            if (akunLogin->role == "mahasiswa") {
                perkuliahan.setMahasiswaInfo(akunLogin->nama, akunLogin->nim, akunLogin->prodi);
                kemahasiswaan.setMahasiswaInfo(akunLogin->nama, akunLogin->nim);
                menuMahasiswa(akunLogin); 
            } else if (akunLogin->role == "dosen") {
                Dosen dosen(akunLogin);
                dosen.menuDosen();
            } else if (akunLogin->role == "admin") {
                Admin admin(akunLogin);
                admin.menuAdmin();
            } else {
                cout << "Role tidak dikenal. Logout...\n";
            }

        } else {
            setLoginAttempts(username, attempts + 1);
            cout << "\nUsername atau password salah. (Percobaan " << (attempts + 1) << "/3)\n";

            if (attempts + 1 >= 3) {
                cout << "Akun Anda terkunci sementara karena 3x gagal login.\n";
                cout << "Tekan ENTER untuk kembali...";
                cin.get();
                return;
            }

            char pilih;
            cout << "Lupa password? (y/n): ";
            cin >> pilih;
            cin.ignore(1000, '\n'); 

            if (pilih == 'y' || pilih == 'Y') {
                lupaPassword();
            }
        }
    }

public:
    Login() {
        fileAkunMahasiswa = "akun_mahasiswa.txt";
        fileAkunDosen = "akun_dosen.txt";
        fileAkunAdmin = "akun_admin.txt";
        
        loginGagal = nullptr;
        jumlahLoginGagal = 0;
        kapasitasLoginGagal = 0;
        
        daftarAkun = nullptr;
        jumlahAkun = 0;
        kapasitasAkun = 0;
        
        daftarBuku = nullptr;
        jumlahBuku = 0;
        kapasitasBuku = 0;
        
        daftarPinjaman = nullptr;
        jumlahPinjaman = 0;
        kapasitasPinjaman = 0;
        
        daftarMatkul = nullptr;
        jumlahMatkul = 0;
        kapasitasMatkul = 0;
        
        daftarJadwal = nullptr;
        jumlahJadwal = 0;
        kapasitasJadwal = 0;
        
        daftarKRS = nullptr;
        jumlahKRS = 0;
        kapasitasKRS = 0;
        
        daftarNilai = nullptr;
        jumlahNilai = 0;
        kapasitasNilai = 0;
        
        daftarPresensi = nullptr;
        jumlahPresensi = 0;
        kapasitasPresensi = 0;
        
        daftarPengajuan = nullptr;
        jumlahPengajuan = 0;
        kapasitasPengajuan = 0;
        
        daftarPembayaran = nullptr;
        jumlahPembayaran = 0;
        kapasitasPembayaran = 0;
        
        sesiChat = nullptr;
        jumlahSesiChat = 0;
        kapasitasSesiChat = 0;
        
        // Inisialisasi queue notifikasi
        initQueueNotifikasi();
        
        loadDataFromFiles();
        muatNotifikasiDariFile();
        
        // Inisialisasi random seed
        srand(time(0));
    }

    ~Login() {
        if (daftarAkun) delete[] daftarAkun;
        if (daftarBuku) delete[] daftarBuku;
        if (daftarPinjaman) delete[] daftarPinjaman;
        if (daftarMatkul) delete[] daftarMatkul;
        if (daftarJadwal) delete[] daftarJadwal;
        if (daftarKRS) delete[] daftarKRS;
        if (daftarNilai) delete[] daftarNilai;
        if (daftarPresensi) delete[] daftarPresensi;
        if (daftarPengajuan) delete[] daftarPengajuan;
        if (daftarPembayaran) delete[] daftarPembayaran;
        if (loginGagal) delete[] loginGagal;
        
        if (sesiChat) {
            for (int i = 0; i < jumlahSesiChat; i++) {
                if (sesiChat[i].pesan) delete[] sesiChat[i].pesan;
            }
            delete[] sesiChat;
        }
        
        if (queueNotifikasi.data) delete[] queueNotifikasi.data;
    }

    void menu() {
        int pilihan;
        do {
            cout<<"\n|=================================== PORTAL ===================================|"<< endl;
            cout<<"|                  |                                                           |"<< endl;
            cout<<"|                  |    ____________________________                           |"<< endl;
            cout<<"|                  |   |  1. Login                  |                          |"<< endl;
            cout<<"|                  |   |  0. Exit                   |                          |"<< endl;
            cout<<"|                  |   |____________________________|                          |"<< endl;
            cout<<"|    UNIVERSITAS   |                                                           |"<< endl;
            cout<<"|   AHMAD  DAHLAN  |   -----------------------------------------               |"<< endl;
            cout<<"|       (UAD)      |   >>> CREATE AN ACCOUNT FIRST TO LOG IN <<<               |\n";
            cout<<"|                  |   -----------------------------------------               |"<< endl;
            cout<<"|                  |                                                           |"<< endl;
            cout<<"|                  |                                                           |"<< endl;
            cout<<"|==============================================================================|"<< endl;
            cout<< endl <<"Pilih :: ";
            
            int input;
            cin >> input;
            pilihan = input;
            cin.ignore(1000, '\n'); 
            
            switch(pilihan){
                case 1:
                    login();
                    break;
                case 0:
                    cout << "Terima kasih telah menggunakan portal UAD!\n"; 
                    break;
                default:
                    cout << "Pilihan tidak valid! Tekan ENTER untuk mengulang..."; 
                    cin.get();
            }
        } while (pilihan != 0);
    }
};

// =====================================================================
// Debug/demo untuk AVL dan Huffman
// =====================================================================
void test_avl() {
    AVLTreeDynamic<int> tree;
    int sample[] = {50, 20, 70, 10, 30, 60, 80};
    for (int v : sample) tree.sisipkan(v);
    cout << "AVL inorder: ";
    int size = tree.ukuran();
    int* out = tree.dapatkanSemuaTerurut();
    for (int i = 0; i < size; ++i) cout << out[i] << " ";
    cout << "\nPohon:\n";
    tree.cetakPohon();
    delete[] out;

    tree.hapus(20);
    cout << "\nSetelah hapus 20, inorder: ";
    int* out2 = tree.dapatkanSemuaTerurut();
    int size2 = tree.ukuran();
    for (int i = 0; i < size2; ++i) cout << out2[i] << " ";
    cout << "\n";
    delete[] out2;

    cout << "Tekan ENTER untuk kembali...";
    cin.get();
}

void test_huffman() {
    HuffmanTreeDynamic huff;
    string teks = "this is an example for huffman encoding";
    string kompres = huff.kompres(teks);
    cout << "Teks asli: " << teks << "\n";
    cout << "Terkompresi (bits): " << kompres << "\n";
    cout << "Kode per karakter:\n";
    huff.tampilkanKode();
    string dekompres = huff.dekompres(kompres);
    cout << "Didekompres: " << dekompres << "\n";

    cout << "Tekan ENTER untuk kembali...";
    cin.get();
}

// =====================================================================
// FUNGSI MAIN()
// =====================================================================
int main() {
    Login aplikasiPortal;
    int choice = -1;
    while (choice != 0) {
        cout << "\n== Debug / Demo Struktur Data ==\n";
        cout << "1. Jalankan aplikasi (menu utama)\n";
        cout << "2. Tes AVL Tree (sisip/hapus/cetak)\n";
        cout << "3. Tes Huffman (kompres/dekompres)\n";
        cout << "0. Keluar\n";
        cout << "Pilih: ";
        if (!(cin >> choice)) {
            cin.clear(); cin.ignore(1000,'\n');
            continue;
        }
        cin.ignore(1000,'\n');

        if (choice == 1) {
            aplikasiPortal.menu();
        } else if (choice == 2) {
            test_avl();
        } else if (choice == 3) {
            test_huffman();
        }
    }

    return 0;
}