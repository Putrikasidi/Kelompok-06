// uji.cpp - pengujian awal F1 (Kelola Buku) dan F2 (Kelola Anggota)
// Pola cek() mengikuti contoh pada spesifikasi tugas besar.
#include <iostream>
#include "sipus.h"
using namespace std;

int lulus = 0, gagal = 0;
void cek(bool kondisi, const string& nama) {
    cout << (kondisi ? "[LULUS] " : "[GAGAL] ") << nama << endl;
    kondisi ? lulus++ : gagal++;
}

void ujiBuku() {
    cout << "--- F1: Kelola Buku ---\n";
    DaftarBuku d;
    inisialisasi(d, 2);
    cek(d.data != nullptr && d.n == 0 && d.kapasitas == 2,
        "U01 inisialisasi buku: kosong, kapasitas 2");
    cek(tambahBuku(d, {"BK-002", "Struktur Data", "Wati",
                       0, 2024, 3, 0}) && d.n == 1,
        "U02 tambah buku BK-002");
    tambahBuku(d, {"BK-001", "Algoritma", "Budi", 0, 2023, 1, 0});
    Buku* alamatLama = d.data;
    cek(tambahBuku(d, {"BK-003", "Basis Data", "Sari",
                       1, 2022, 2, 0}),
        "U03 tambah buku ke-3 saat array penuh");
    cek(d.n == 3 && d.kapasitas == 4 && d.data != alamatLama,
        "U04 array buku membesar (kapasitas 2 -> 4)");
    cek(d.data[0].kode == "BK-002" && d.data[1].judul == "Algoritma"
        && d.data[2].penulis == "Sari",
        "U05 data lama utuh setelah pembesaran");
    Buku ganda = {"BK-001", "X", "Y", 0, 2020, 1, 0};
    cek(!tambahBuku(d, ganda) && d.n == 3,
        "U06 kode buku ganda ditolak");
    cek(!tambahBuku(d, {"BK-01", "X", "Y", 0, 2020, 1, 0})
        && !tambahBuku(d, {"bk-009", "X", "Y", 0, 2020, 1, 0})
        && !tambahBuku(d, {"BK-0A9", "X", "Y", 0, 2020, 1, 0}),
        "U07 format kode selain BK-xxx ditolak");
    cek(!tambahBuku(d, {"BK-009", "", "Y", 0, 2020, 1, 0})
        && !tambahBuku(d, {"BK-009", "X", "Y", 5, 2020, 1, 0})
        && !tambahBuku(d, {"BK-009", "X", "Y", 0, 2020, -1, 0})
        && !tambahBuku(d, {"BK-009", "X", "Y", 0, 2099, 1, 0})
        && d.n == 3,
        "U08 judul kosong/kategori/stok/tahun salah ditolak");
    Buku* p = cariKode(d, "BK-003");
    cek(p != nullptr && p->judul == "Basis Data"
        && cariKode(d, "BK-404") == nullptr,
        "U09 cariKode: ditemukan dan nullptr bila tidak ada");
    p->stok = 7;  // pointer mengubah record di dalam array
    cek(d.data[2].stok == 7, "U10 ubah stok melalui pointer cariKode");
    cek(ubahBuku(d, "BK-001", {"", "Algoritma Lanjut", "Budi S.",
                              4, 2025, 5, 0})
        && d.data[1].judul == "Algoritma Lanjut"
        && d.data[1].kategori == 4 && d.data[1].kode == "BK-001",
        "U11 ubah data buku BK-001 (kode tetap)");
    cek(!ubahBuku(d, "BK-001", {"", "", "Budi", 0, 2025, 5, 0})
        && d.data[1].judul == "Algoritma Lanjut",
        "U12 ubah dengan judul kosong ditolak, data tidak berubah");
    cek(!ubahBuku(d, "BK-404", {"", "X", "Y", 0, 2020, 1, 0}),
        "U13 ubah buku yang tidak ada ditolak");
    cek(hapusBuku(d, "BK-002") && d.n == 2
        && d.data[0].kode == "BK-001" && d.data[1].kode == "BK-003",
        "U14 hapus BK-002, elemen sesudahnya bergeser");
    cek(!hapusBuku(d, "BK-002") && d.n == 2,
        "U15 hapus buku yang tidak ada ditolak");
    for (int i = 10; i < 20; i++) {
        tambahBuku(d, {"BK-0" + to_string(i), "Judul", "Penulis",
                       0, 2020, 1, 0});
    }
    cek(d.n == 12 && d.kapasitas == 16
        && d.data[0].kode == "BK-001" && d.data[11].kode == "BK-019",
        "U16 pembesaran berulang (4 -> 8 -> 16) tanpa kehilangan data");
    bebaskan(d);
    cek(d.data == nullptr && d.n == 0 && d.kapasitas == 0,
        "U17 memori buku dibebaskan");
}

void ujiAnggota() {
    cout << "--- F2: Kelola Anggota ---\n";
    DaftarAnggota d;
    inisialisasi(d, 2);
    cek(tambahAnggota(d, {"AG-001", "Ayu", "Teknologi Informasi", 0})
        && d.n == 1,
        "U18 tambah anggota AG-001");
    tambahAnggota(d, {"AG-002", "Bagus", "Teknik Elektro", 0});
    cek(tambahAnggota(d, {"AG-003", "Citra", "Teknik Sipil", 0})
        && d.n == 3 && d.kapasitas == 4
        && d.data[0].nama == "Ayu" && d.data[2].id == "AG-003",
        "U19 array anggota membesar tanpa kehilangan data");
    cek(!tambahAnggota(d, {"AG-001", "Dewi", "Arsitektur", 0})
        && d.n == 3,
        "U20 ID anggota ganda ditolak");
    cek(!tambahAnggota(d, {"AG-1", "X", "Y", 0})
        && !tambahAnggota(d, {"BK-004", "X", "Y", 0})
        && !tambahAnggota(d, {"AG-004", "", "Y", 0})
        && !tambahAnggota(d, {"AG-004", "X", "", 0})
        && !tambahAnggota(d, {"AG-004", "X", "Y", 4}),
        "U21 format ID/nama/prodi/sedangPinjam salah ditolak");
    cek(ubahAnggota(d, "AG-002", {"", "Bagus Pratama", "Teknik Mesin",
                                  0})
        && d.data[1].nama == "Bagus Pratama"
        && d.data[1].prodi == "Teknik Mesin"
        && d.data[1].id == "AG-002",
        "U22 ubah data anggota AG-002 (ID tetap)");
    cek(!ubahAnggota(d, "AG-404", {"", "X", "Y", 0}),
        "U23 ubah anggota yang tidak ada ditolak");
    const DaftarAnggota& baca = d;  // referensi konstan: hanya membaca
    cek(indeksAnggota(baca, "AG-003") == 2
        && indeksAnggota(baca, "AG-404") == -1,
        "U24 indeksAnggota lewat referensi konstan");
    bebaskan(d);
    cek(d.data == nullptr, "U25 memori anggota dibebaskan");
}

int main() {
    ujiBuku();
    ujiAnggota();
    cout << lulus << " lulus, " << gagal << " gagal\n";
    return gagal == 0 ? 0 : 1;
}
