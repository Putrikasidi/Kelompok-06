// buku.cpp - implementasi F1 Kelola Buku (Mahasiswa A)
// Array of struct dinamis dengan new[]/delete[] dan pembesaran kapasitas.
#include <iostream>
#include <iomanip>
#include "sipus.h"
using namespace std;

const string NAMA_KATEGORI[JML_KATEGORI] = {
    "Pemrograman", "Basis Data", "Jaringan", "Matematika", "Umum"
};

// Memotong teks agar muat pada kolom tabel selebar w karakter.
static string potong(const string& s, size_t w) {
    if (s.size() <= w) return s;
    return s.substr(0, w - 2) + "..";
}

// Mengalokasikan array kosong dengan kapasitas awal (minimal 1).
void inisialisasi(DaftarBuku& d, int kapasitasAwal) {
    if (kapasitasAwal < 1) kapasitasAwal = 1;
    d.data = new Buku[kapasitasAwal];
    d.n = 0;
    d.kapasitas = kapasitasAwal;
}

// Melepaskan memori array dan mengosongkan pembungkus.
void bebaskan(DaftarBuku& d) {
    delete[] d.data;
    d.data = nullptr;
    d.n = 0;
    d.kapasitas = 0;
}

// Menggandakan kapasitas: alokasi baru, salin data, hapus array lama.
void perbesar(DaftarBuku& d) {
    int kapasitasBaru = (d.kapasitas < 1) ? 1 : d.kapasitas * 2;
    Buku* baru = new Buku[kapasitasBaru];
    for (int i = 0; i < d.n; i++) {
        baru[i] = d.data[i];
    }
    delete[] d.data;
    d.data = baru;
    d.kapasitas = kapasitasBaru;
}

// Format kode wajib "BK-" diikuti tepat tiga digit, misalnya BK-001.
bool formatKodeValid(const string& kode) {
    if (kode.size() != 6) return false;
    if (kode.substr(0, 3) != "BK-") return false;
    for (int i = 3; i < 6; i++) {
        if (kode[i] < '0' || kode[i] > '9') return false;
    }
    return true;
}

// Memeriksa seluruh atribut buku sebelum disimpan.
bool bukuValid(const Buku& b) {
    return formatKodeValid(b.kode)
        && !b.judul.empty() && !b.penulis.empty()
        && b.kategori >= 0 && b.kategori < JML_KATEGORI
        && b.tahun >= TAHUN_MIN && b.tahun <= TAHUN_MAKS
        && b.stok >= 0 && b.totalDipinjam >= 0;
}

// Mengembalikan indeks buku berkode tertentu, atau -1 jika tidak ada.
int indeksBuku(const DaftarBuku& d, const string& kode) {
    for (int i = 0; i < d.n; i++) {
        if (d.data[i].kode == kode) return i;
    }
    return -1;
}

// Mengembalikan pointer ke record buku, atau nullptr jika tidak ada.
Buku* cariKode(DaftarBuku& d, const string& kode) {
    int i = indeksBuku(d, kode);
    return (i == -1) ? nullptr : &d.data[i];
}

// Menambah buku baru; ditolak jika data tidak valid atau kode ganda.
bool tambahBuku(DaftarBuku& d, const Buku& b) {
    if (!bukuValid(b)) return false;
    if (indeksBuku(d, b.kode) != -1) return false;
    if (d.n == d.kapasitas) perbesar(d);
    d.data[d.n] = b;
    d.n++;
    return true;
}

// Mengubah judul, penulis, kategori, tahun, dan stok buku.
// Kode (kunci) dan totalDipinjam (statistik) tidak diubah.
bool ubahBuku(DaftarBuku& d, const string& kode, const Buku& baru) {
    Buku* p = cariKode(d, kode);
    if (p == nullptr) return false;
    Buku calon = baru;
    calon.kode = p->kode;
    calon.totalDipinjam = p->totalDipinjam;
    if (!bukuValid(calon)) return false;
    *p = calon;
    return true;
}

// Menghapus buku dan menggeser elemen sesudahnya ke kiri.
bool hapusBuku(DaftarBuku& d, const string& kode) {
    int idx = indeksBuku(d, kode);
    if (idx == -1) return false;
    for (int i = idx; i < d.n - 1; i++) {
        d.data[i] = d.data[i + 1];
    }
    d.n--;
    return true;
}

// Menampilkan seluruh buku dalam bentuk tabel.
void tampilkanBuku(const DaftarBuku& d) {
    const string garis(77, '-');
    cout << "\nDAFTAR BUKU (" << d.n << " data, kapasitas "
         << d.kapasitas << ")\n" << garis << '\n';
    if (d.n == 0) {
        cout << "Belum ada data buku.\n" << garis << '\n';
        return;
    }
    cout << left << setw(4) << "No" << setw(8) << "Kode"
         << setw(21) << "Judul" << setw(15) << "Penulis"
         << setw(12) << "Kategori" << right << setw(5) << "Tahun"
         << setw(5) << "Stok" << setw(7) << "Pinjam" << '\n'
         << garis << '\n';
    for (int i = 0; i < d.n; i++) {
        const Buku& b = d.data[i];
        cout << left << setw(4) << i + 1 << setw(8) << b.kode
             << setw(21) << potong(b.judul, 20)
             << setw(15) << potong(b.penulis, 14)
             << setw(12) << NAMA_KATEGORI[b.kategori]
             << right << setw(5) << b.tahun << setw(5) << b.stok
             << setw(7) << b.totalDipinjam << '\n';
    }
    cout << garis << '\n';
}
