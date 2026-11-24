// main.cpp - menu utama SiPus dan integrasi F1-F2 (tugas bersama)
// Kelompok 6 - Milestone 1 dan sebagian Milestone 2
#include <iostream>
#include <sstream>
#include "sipus.h"
using namespace std;

// ================= Fungsi bantu validasi masukan =================

// Menghapus spasi di awal dan akhir teks.
static string rapikan(const string& s) {
    size_t awal = s.find_first_not_of(" \t\r\n");
    if (awal == string::npos) return "";
    size_t akhir = s.find_last_not_of(" \t\r\n");
    return s.substr(awal, akhir - awal + 1);
}

// Mengubah huruf kecil menjadi kapital (agar "bk-001" diterima).
static string kapital(string s) {
    for (size_t i = 0; i < s.size(); i++) {
        if (s[i] >= 'a' && s[i] <= 'z') s[i] = s[i] - 'a' + 'A';
    }
    return s;
}

// Membaca satu baris. Bernilai false jika masukan berakhir (EOF).
static bool bacaBaris(const string& prompt, string& hasil) {
    cout << prompt;
    string baris;
    if (!getline(cin, baris)) return false;
    hasil = rapikan(baris);
    return true;
}

// Mengubah teks menjadi bilangan bulat utuh; false jika tidak valid.
static bool keAngka(const string& s, int& hasil) {
    istringstream ss(s);
    int nilai;
    char sisa;
    if (!(ss >> nilai) || (ss >> sisa)) return false;
    hasil = nilai;
    return true;
}

// Membaca teks yang tidak boleh kosong.
static bool bacaTeks(const string& prompt, string& hasil) {
    while (bacaBaris(prompt, hasil)) {
        if (!hasil.empty()) return true;
        cout << "  [!] Masukan tidak boleh kosong.\n";
    }
    return false;
}

// Membaca bilangan bulat dalam rentang [minimum, maksimum].
static bool bacaAngka(const string& prompt, int minimum, int maksimum,
                      int& hasil) {
    string s;
    while (bacaBaris(prompt, s)) {
        if (keAngka(s, hasil) && hasil >= minimum && hasil <= maksimum)
            return true;
        cout << "  [!] Masukkan bilangan bulat " << minimum << " s.d. "
             << maksimum << ".\n";
    }
    return false;
}

// Seperti bacaTeks, tetapi masukan kosong mempertahankan nilai lama.
static bool bacaTeksOpsional(const string& prompt, string& nilai) {
    string s;
    if (!bacaBaris(prompt + " [" + nilai + "]: ", s)) return false;
    if (!s.empty()) nilai = s;
    return true;
}

// Seperti bacaAngka, tetapi masukan kosong mempertahankan nilai lama.
static bool bacaAngkaOpsional(const string& prompt, int minimum,
                              int maksimum, int& nilai) {
    string s;
    int hasil;
    while (bacaBaris(prompt + " [" + to_string(nilai) + "]: ", s)) {
        if (s.empty()) return true;
        if (keAngka(s, hasil) && hasil >= minimum && hasil <= maksimum) {
            nilai = hasil;
            return true;
        }
        cout << "  [!] Masukkan bilangan bulat " << minimum << " s.d. "
             << maksimum << ".\n";
    }
    return false;
}

// Pointer ke fungsi pemeriksa format (formatKodeValid/formatIdValid).
typedef bool (*Pemeriksa)(const string&);

// Membaca kode berformat BK-xxx atau ID berformat AG-xxx.
static bool bacaKode(const string& prompt, Pemeriksa formatValid,
                     const string& contoh, string& hasil) {
    while (bacaBaris(prompt, hasil)) {
        hasil = kapital(hasil);
        if (formatValid(hasil)) return true;
        cout << "  [!] Format salah. Contoh yang benar: " << contoh
             << ".\n";
    }
    return false;
}

static void tampilkanPilihanKategori() {
    cout << "  Kategori:";
    for (int i = 0; i < JML_KATEGORI; i++) {
        cout << ' ' << i + 1 << '=' << NAMA_KATEGORI[i];
    }
    cout << '\n';
}

// ===================== F1: menu Kelola Buku =====================

static void menuTambahBuku(DaftarBuku& d) {
    Buku b;
    cout << "\n-- Tambah Buku --\n";
    if (!bacaKode("Kode buku (BK-xxx) : ", formatKodeValid, "BK-001",
                  b.kode)) return;
    if (indeksBuku(d, b.kode) != -1) {
        cout << ">> Gagal: kode " << b.kode << " sudah terdaftar.\n";
        return;
    }
    if (!bacaTeks("Judul              : ", b.judul)) return;
    if (!bacaTeks("Penulis            : ", b.penulis)) return;
    tampilkanPilihanKategori();
    int kat;
    if (!bacaAngka("Kategori (1-5)     : ", 1, JML_KATEGORI, kat)) return;
    b.kategori = kat - 1;
    if (!bacaAngka("Tahun terbit       : ", TAHUN_MIN, TAHUN_MAKS,
                   b.tahun)) return;
    if (!bacaAngka("Stok               : ", 0, 9999, b.stok)) return;
    b.totalDipinjam = 0;
    if (tambahBuku(d, b)) {
        cout << ">> Berhasil: buku " << b.kode << " ditambahkan. (n="
             << d.n << ", kapasitas=" << d.kapasitas << ")\n";
    } else {
        cout << ">> Gagal: data buku tidak valid.\n";
    }
}

static void menuUbahBuku(DaftarBuku& d) {
    string kode;
    cout << "\n-- Ubah Buku --\n";
    if (!bacaKode("Kode buku (BK-xxx) : ", formatKodeValid, "BK-001",
                  kode)) return;
    const Buku* lama = cariKode(d, kode);
    if (lama == nullptr) {
        cout << ">> Gagal: buku " << kode << " tidak ditemukan.\n";
        return;
    }
    cout << "(Tekan Enter untuk mempertahankan nilai lama)\n";
    Buku baru = *lama;
    int kat = baru.kategori + 1;
    if (!bacaTeksOpsional("Judul", baru.judul)) return;
    if (!bacaTeksOpsional("Penulis", baru.penulis)) return;
    tampilkanPilihanKategori();
    if (!bacaAngkaOpsional("Kategori (1-5)", 1, JML_KATEGORI, kat)) return;
    baru.kategori = kat - 1;
    if (!bacaAngkaOpsional("Tahun terbit", TAHUN_MIN, TAHUN_MAKS,
                           baru.tahun)) return;
    if (!bacaAngkaOpsional("Stok", 0, 9999, baru.stok)) return;
    if (ubahBuku(d, kode, baru)) {
        cout << ">> Berhasil: data buku " << kode << " diperbarui.\n";
    } else {
        cout << ">> Gagal: data buku tidak valid.\n";
    }
}

static void menuHapusBuku(DaftarBuku& d) {
    string kode, jawab;
    cout << "\n-- Hapus Buku --\n";
    if (!bacaKode("Kode buku (BK-xxx) : ", formatKodeValid, "BK-001",
                  kode)) return;
    const Buku* b = cariKode(d, kode);
    if (b == nullptr) {
        cout << ">> Gagal: buku " << kode << " tidak ditemukan.\n";
        return;
    }
    cout << "Buku: " << b->judul << " (" << b->penulis << ")\n";
    while (bacaBaris("Yakin menghapus? (y/t): ", jawab)) {
        jawab = kapital(jawab);
        if (jawab == "Y") {
            hapusBuku(d, kode);
            cout << ">> Berhasil: buku " << kode << " dihapus.\n";
            return;
        }
        if (jawab == "T") {
            cout << ">> Penghapusan dibatalkan.\n";
            return;
        }
        cout << "  [!] Jawab dengan y atau t.\n";
    }
}

static void menuBuku(DaftarBuku& d) {
    int pilihan = -1;
    while (pilihan != 0) {
        cout << "\n----- Kelola Buku -----\n"
             << "1. Tambah Buku\n2. Ubah Buku\n3. Hapus Buku\n"
             << "4. Tampilkan Buku\n0. Kembali\n";
        if (!bacaAngka("Pilihan: ", 0, 4, pilihan)) return;
        switch (pilihan) {
            case 1: menuTambahBuku(d); break;
            case 2: menuUbahBuku(d);   break;
            case 3: menuHapusBuku(d);  break;
            case 4: tampilkanBuku(d);  break;
        }
        if (!cin) return;
    }
}

// =================== F2: menu Kelola Anggota ===================

static void menuTambahAnggota(DaftarAnggota& d) {
    Anggota a;
    cout << "\n-- Tambah Anggota --\n";
    if (!bacaKode("ID anggota (AG-xxx): ", formatIdValid, "AG-001",
                  a.id)) return;
    if (indeksAnggota(d, a.id) != -1) {
        cout << ">> Gagal: ID " << a.id << " sudah terdaftar.\n";
        return;
    }
    if (!bacaTeks("Nama               : ", a.nama)) return;
    if (!bacaTeks("Program studi      : ", a.prodi)) return;
    a.sedangPinjam = 0;
    if (tambahAnggota(d, a)) {
        cout << ">> Berhasil: anggota " << a.id << " ditambahkan. (n="
             << d.n << ", kapasitas=" << d.kapasitas << ")\n";
    } else {
        cout << ">> Gagal: data anggota tidak valid.\n";
    }
}

static void menuUbahAnggota(DaftarAnggota& d) {
    string id;
    cout << "\n-- Ubah Anggota --\n";
    if (!bacaKode("ID anggota (AG-xxx): ", formatIdValid, "AG-001",
                  id)) return;
    const Anggota* lama = cariAnggota(d, id);
    if (lama == nullptr) {
        cout << ">> Gagal: anggota " << id << " tidak ditemukan.\n";
        return;
    }
    cout << "(Tekan Enter untuk mempertahankan nilai lama)\n";
    Anggota baru = *lama;
    if (!bacaTeksOpsional("Nama", baru.nama)) return;
    if (!bacaTeksOpsional("Program studi", baru.prodi)) return;
    if (ubahAnggota(d, id, baru)) {
        cout << ">> Berhasil: data anggota " << id << " diperbarui.\n";
    } else {
        cout << ">> Gagal: data anggota tidak valid.\n";
    }
}

static void menuAnggota(DaftarAnggota& d) {
    int pilihan = -1;
    while (pilihan != 0) {
        cout << "\n----- Kelola Anggota -----\n"
             << "1. Tambah Anggota\n2. Ubah Anggota\n"
             << "3. Tampilkan Anggota\n0. Kembali\n";
        if (!bacaAngka("Pilihan: ", 0, 3, pilihan)) return;
        switch (pilihan) {
            case 1: menuTambahAnggota(d); break;
            case 2: menuUbahAnggota(d);   break;
            case 3: tampilkanAnggota(d);  break;
        }
        if (!cin) return;
    }
}

// ========================== Program utama ==========================

int main() {
    DaftarBuku buku;
    DaftarAnggota anggota;
    inisialisasi(buku, KAPASITAS_AWAL);     // Kapasitas awal kecil agar
    inisialisasi(anggota, KAPASITAS_AWAL);  // pembesaran dapat teruji

    int pilihan = -1;
    while (pilihan != 0) {
        cout << "\n===== SiPus: Perpustakaan Mini =====\n"
             << "1. Kelola Buku\n"
             << "2. Kelola Anggota\n"
             << "3. Tampilkan Daftar Buku\n"
             << "4. Tampilkan Daftar Anggota\n"
             << "0. Keluar\n";
        if (!bacaAngka("Pilihan: ", 0, 4, pilihan)) break;
        switch (pilihan) {
            case 1: menuBuku(buku);            break;
            case 2: menuAnggota(anggota);      break;
            case 3: tampilkanBuku(buku);       break;
            case 4: tampilkanAnggota(anggota); break;
        }
        if (!cin) break;  // Masukan berakhir (EOF): keluar dengan aman
    }

    bebaskan(buku);       // Wajib: tidak boleh ada kebocoran memori
    bebaskan(anggota);
    cout << "\nTerima kasih telah menggunakan SiPus.\n";
    return 0;
}
