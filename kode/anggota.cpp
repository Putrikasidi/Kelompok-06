// anggota.cpp - implementasi F2 Kelola Anggota (Mahasiswa B)
// Array of struct dinamis dengan new[]/delete[] dan pembesaran kapasitas.
#include <iostream>
#include <iomanip>
#include "sipus.h"
using namespace std;

// Memotong teks agar muat pada kolom tabel selebar w karakter.
static string potong(const string& s, size_t w) {
    if (s.size() <= w) return s;
    return s.substr(0, w - 2) + "..";
}

// Mengalokasikan array kosong dengan kapasitas awal (minimal 1).
void inisialisasi(DaftarAnggota& d, int kapasitasAwal) {
    if (kapasitasAwal < 1) kapasitasAwal = 1;
    d.data = new Anggota[kapasitasAwal];
    d.n = 0;
    d.kapasitas = kapasitasAwal;
}

// Melepaskan memori array dan mengosongkan pembungkus.
void bebaskan(DaftarAnggota& d) {
    delete[] d.data;
    d.data = nullptr;
    d.n = 0;
    d.kapasitas = 0;
}

// Menggandakan kapasitas: alokasi baru, salin data, hapus array lama.
void perbesar(DaftarAnggota& d) {
    int kapasitasBaru = (d.kapasitas < 1) ? 1 : d.kapasitas * 2;
    Anggota* baru = new Anggota[kapasitasBaru];
    for (int i = 0; i < d.n; i++) {
        baru[i] = d.data[i];
    }
    delete[] d.data;
    d.data = baru;
    d.kapasitas = kapasitasBaru;
}

// Format ID wajib "AG-" diikuti tepat tiga digit, misalnya AG-001.
bool formatIdValid(const string& id) {
    if (id.size() != 6) return false;
    if (id.substr(0, 3) != "AG-") return false;
    for (int i = 3; i < 6; i++) {
        if (id[i] < '0' || id[i] > '9') return false;
    }
    return true;
}

// Memeriksa seluruh atribut anggota sebelum disimpan.
bool anggotaValid(const Anggota& a) {
    return formatIdValid(a.id)
        && !a.nama.empty() && !a.prodi.empty()
        && a.sedangPinjam >= 0 && a.sedangPinjam <= MAKS_PINJAM;
}

// Mengembalikan indeks anggota ber-ID tertentu, atau -1 jika tidak ada.
int indeksAnggota(const DaftarAnggota& d, const string& id) {
    for (int i = 0; i < d.n; i++) {
        if (d.data[i].id == id) return i;
    }
    return -1;
}

// Mengembalikan pointer ke record anggota, atau nullptr jika tidak ada.
Anggota* cariAnggota(DaftarAnggota& d, const string& id) {
    int i = indeksAnggota(d, id);
    return (i == -1) ? nullptr : &d.data[i];
}

// Menambah anggota baru; ditolak jika data tidak valid atau ID ganda.
bool tambahAnggota(DaftarAnggota& d, const Anggota& a) {
    if (!anggotaValid(a)) return false;
    if (indeksAnggota(d, a.id) != -1) return false;
    if (d.n == d.kapasitas) perbesar(d);
    d.data[d.n] = a;
    d.n++;
    return true;
}

// Mengubah nama dan prodi anggota.
// ID (kunci) dan sedangPinjam (dikelola transaksi) tidak diubah.
bool ubahAnggota(DaftarAnggota& d, const string& id,
                 const Anggota& baru) {
    Anggota* p = cariAnggota(d, id);
    if (p == nullptr) return false;
    Anggota calon = baru;
    calon.id = p->id;
    calon.sedangPinjam = p->sedangPinjam;
    if (!anggotaValid(calon)) return false;
    *p = calon;
    return true;
}

// Menampilkan seluruh anggota dalam bentuk tabel.
void tampilkanAnggota(const DaftarAnggota& d) {
    const string garis(77, '-');
    cout << "\nDAFTAR ANGGOTA (" << d.n << " data, kapasitas "
         << d.kapasitas << ")\n" << garis << '\n';
    if (d.n == 0) {
        cout << "Belum ada data anggota.\n" << garis << '\n';
        return;
    }
    cout << left << setw(4) << "No" << setw(8) << "ID"
         << setw(31) << "Nama" << setw(26) << "Program Studi"
         << right << setw(8) << "Pinjam" << '\n' << garis << '\n';
    for (int i = 0; i < d.n; i++) {
        const Anggota& a = d.data[i];
        cout << left << setw(4) << i + 1 << setw(8) << a.id
             << setw(31) << potong(a.nama, 30)
             << setw(26) << potong(a.prodi, 25)
             << right << setw(8) << a.sedangPinjam << '\n';
    }
    cout << garis << '\n';
}
