#ifndef __SAVELOAD__
#define __SAVELOAD_HPP__

#include <fstream>
#include <string>
#include <vector>
#include <sstream>

using namespace std;

struct Koordinat
{
    int x;
    int y;
};

struct DataSave
{
    int bobotHadiah;
    string isiBuffer;
    vector<Koordinat> koordinatList;
};

struct Ukuran
{
    int baris;
    int kolom;
};

struct DataLoad
{
    int ukuranBuffer;
    vector<Ukuran> ukuranMatriks;
    vector<vector<string>> matriks;
    int numberOfSequences;
    vector<string> sequences;
    vector<int> sequenceReward;
};

/***** LOAD - MEMUAT DATA DARI INPUT (.txt) *****/
/*
1. Ukuran buffer
2. Ukuran matriks (m x n) / baris x kolom
3. Koordinat (dari list koordinat) *jika ada*
4. Waktu eksekusi
*/

// Fungsi untuk memuat data dari file
DataLoad loadData(const string &filename);

// Prosedur memuat data dari file
void load();

/***** SAVE - MENYIMPAN DATA HASIL OUTPUT *****/
/*
1. Bobot Hadiah
2. Isi Buffer *jika ada*
3. Koordinat (dari list koordinat) *jika ada*
4. Waktu eksekusi
*/

// Prosedur untuk menyimpan data ke file
void saveData(const string &filename, const DataSave &data, int waktuEksekusi);

#endif