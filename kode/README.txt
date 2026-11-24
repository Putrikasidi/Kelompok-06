SiPus - Sistem Peminjaman Perpustakaan Mini
Tugas Besar Pemrograman dan Struktur Data - KELOMPOK 6
Cakupan: Milestone 1 dan sebagian Milestone 2 (F1 dan F2 berfungsi)

Anggota:
  Mahasiswa A : Ni Nyoman Giri Putri Kasidi Amurwa Bumi - 260530911150
  Mahasiswa B : Kadek Puspa Sari Nitisara Swarananda   - 260530911033

Berkas:
  sipus.h      definisi konstanta, struct, dan prototipe fungsi
  buku.cpp     F1 Kelola Buku (Mahasiswa A)
  anggota.cpp  F2 Kelola Anggota (Mahasiswa B)
  main.cpp     menu utama dan validasi masukan (bersama)
  uji.cpp      25 kasus uji otomatis F1 dan F2

Kompilasi dan menjalankan program utama:
  g++ -std=c++17 -Wall main.cpp buku.cpp anggota.cpp -o sipus
  ./sipus            (Windows: .\sipus.exe)

Kompilasi dan menjalankan pengujian:
  g++ -std=c++17 -Wall uji.cpp buku.cpp anggota.cpp -o uji
  ./uji              (Windows: .\uji.exe)

Pemeriksaan memori (Linux):
  g++ -std=c++17 -Wall -g -fsanitize=address uji.cpp buku.cpp anggota.cpp -o uji_asan
  ./uji_asan
  valgrind --leak-check=full ./uji

Catatan: F3-F8 belum diimplementasikan; prototipenya di sipus.h
merupakan rancangan untuk Milestone 3 dan 4.
