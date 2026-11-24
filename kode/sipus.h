// sipus.h - definisi tipe data dan prototipe fungsi SiPus
// Kelompok 6 - Versi awal (Milestone 1 dan sebagian Milestone 2)
// Mahasiswa A: Ni Nyoman Giri Putri Kasidi Amurwa Bumi (260530911150)
// Mahasiswa B: Kadek Puspa Sari Nitisara Swarananda   (260530911033)
#ifndef SIPUS_H
#define SIPUS_H
#include <string>
using namespace std;

// ===================== Konstanta =====================
const int JML_KATEGORI   = 5;     // Ukuran baris rekap
const int JML_BULAN      = 12;    // Ukuran kolom rekap
const int MAKS_PINJAM    = 3;     // Batas buku/anggota
const int DENDA_HARIAN   = 1000;  // Rupiah per hari
const int KAPASITAS_AWAL = 4;     // Kapasitas awal array (maks. 4)
const int TAHUN_MIN      = 1500;  // Batas bawah tahun terbit
const int TAHUN_MAKS     = 2026;  // Batas atas tahun terbit

// Nama kategori buku, indeks 0..JML_KATEGORI-1 (didefinisikan di buku.cpp)
extern const string NAMA_KATEGORI[JML_KATEGORI];

// ================== Definisi Struct ==================
struct Tanggal {
    int hari, bulan, tahun;
};

struct Buku {
    string kode;          // Format "BK-001"
    string judul;
    string penulis;
    int    kategori;      // 0..JML_KATEGORI-1
    int    tahun;
    int    stok;          // Eksemplar tersedia
    int    totalDipinjam; // Untuk statistik
};

struct Anggota {
    string id;            // Format "AG-001"
    string nama;
    string prodi;
    int    sedangPinjam;  // Maksimum MAKS_PINJAM
};

struct Transaksi {
    int     nomor;
    string  idAnggota;
    string  kodeBuku;
    Tanggal tglPinjam;
    Tanggal tglKembali;   // Diisi saat buku kembali
    bool    selesai;
    int     denda;
};

// Pembungkus array dinamis: alamat, isi, kapasitas
struct DaftarBuku {
    Buku* data; int n, kapasitas;
};
struct DaftarAnggota {
    Anggota* data; int n, kapasitas;
};
struct DaftarTransaksi {
    Transaksi* data; int n, kapasitas;
};

// ====== Modul Buku - F1 (Mahasiswa A) - SUDAH DIIMPLEMENTASIKAN ======
void  inisialisasi(DaftarBuku& d, int kapasitasAwal);
void  bebaskan(DaftarBuku& d);
void  perbesar(DaftarBuku& d);
bool  formatKodeValid(const string& kode);
bool  bukuValid(const Buku& b);
bool  tambahBuku(DaftarBuku& d, const Buku& b);
bool  ubahBuku(DaftarBuku& d, const string& kode, const Buku& baru);
bool  hapusBuku(DaftarBuku& d, const string& kode);
Buku* cariKode(DaftarBuku& d, const string& kode);
int   indeksBuku(const DaftarBuku& d, const string& kode);
void  tampilkanBuku(const DaftarBuku& d);

// ==== Modul Anggota - F2 (Mahasiswa B) - SUDAH DIIMPLEMENTASIKAN ====
void     inisialisasi(DaftarAnggota& d, int kapasitasAwal);
void     bebaskan(DaftarAnggota& d);
void     perbesar(DaftarAnggota& d);
bool     formatIdValid(const string& id);
bool     anggotaValid(const Anggota& a);
bool     tambahAnggota(DaftarAnggota& d, const Anggota& a);
bool     ubahAnggota(DaftarAnggota& d, const string& id,
                     const Anggota& baru);
Anggota* cariAnggota(DaftarAnggota& d, const string& id);
int      indeksAnggota(const DaftarAnggota& d, const string& id);
void     tampilkanAnggota(const DaftarAnggota& d);

// ================================================================
// RENCANA MILESTONE BERIKUTNYA (F3-F8) - BELUM DIIMPLEMENTASIKAN.
// Prototipe disiapkan sesuai spesifikasi dan tidak dipanggil di M2.
// ================================================================
// ---- F5, F6 (Mahasiswa A) ----
void cariJudul(const DaftarBuku& d, const string& kata);
void urutkanKode(DaftarBuku& d);
int  binarySearchKode(const Buku arr[], int kiri,
                      int kanan, const string& kode);

// ---- F3, F4, F8 (Mahasiswa B) ----
void inisialisasi(DaftarTransaksi& d, int kapasitasAwal);
void bebaskan(DaftarTransaksi& d);
bool pinjamBuku(DaftarTransaksi& t, DaftarBuku& b,
                DaftarAnggota& a, const string& idAg,
                const string& kodeBk, const Tanggal& tgl);
bool kembalikanBuku(DaftarTransaksi& t, DaftarBuku& b,
                    DaftarAnggota& a, int nomor,
                    const Tanggal& tgl);
int  selisihHari(const Tanggal& awal, const Tanggal& akhir);
int  totalDendaRekursif(const Transaksi arr[], int n);

// ---- F7 Laporan (Mahasiswa B) ----
void isiRekap(const DaftarTransaksi& t, const DaftarBuku& b,
              int rekap[][JML_BULAN]);
void tampilRekap(const int rekap[][JML_BULAN],
                 const string namaKategori[]);

#endif
