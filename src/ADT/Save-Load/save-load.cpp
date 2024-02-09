#include <iostream>
#include "save-load.hpp"

/***** SAVE - MENYIMPAN DATA HASIL OUTPUT *****/
/*
1. Bobot Hadiah
2. Isi Buffer *jika ada*
3. Koordinat (dari list koordinat) *jika ada*
4. Waktu eksekusi
*/

// Fungsi untuk menyimpan data ke file
void saveData(const string &filename, const DataSave &data, int waktuEksekusi)
{
    ofstream file(filename);

    if (file.is_open())
    {
        file << data.bobotHadiah << endl;
        file << data.isiBuffer << endl;
        for (const Koordinat &koordinat : data.koordinatList)
        {
            file << koordinat.x << ", " << koordinat.y << endl;
        }
        file << endl
             << waktuEksekusi << " ms" << endl;
        file.close();
        cout << "Data berhasil disimpan ke " << filename << endl;
    }
    else
    {
        cout << "Gagal membuka file untuk disimpan" << endl;
    }
}

/* SAVING DATA (output)
    // DataSave yang akan disimpan
    DataSave dataToSave = {50, "7A BD 7A BD 1C BD 55", {{1, 1}, {1, 4}, {3, 4}, {3, 5}, {6, 5}, {6, 4}, {5, 4}}};

    // Simpan data ke file
    string fileSave;
    cin >> fileSave;
    saveData("../test/" + fileSave, dataToSave, 300); // SESUAIKAN INPUT USER NAMA FILENYA YANG INGIN DI-SAVE
*/


/***** LOAD - MEMUAT DATA DARI INPUT (.txt) *****/
/*
1. Ukuran buffer
2. Ukuran matriks (m x n) / baris x kolom
3. Matriks
4. Number of sequences
5. Sequences
6. Sequence bobot
*/

// Fungsi untuk memuat data dari file
DataLoad loadData(const string &filename)
{
    DataLoad data;
    ifstream file(filename);

    if (file.is_open())
    {
        string line;
        // Membaca ukuran buffer
        getline(file, line);
        stringstream(line) >> data.ukuranBuffer;

        // Membaca ukuran matriks
        getline(file, line);
        stringstream ss(line);
        Ukuran ukuran;
        ss >> ukuran.baris >> ukuran.kolom;
        data.ukuranMatriks.push_back(ukuran);

        // Membaca isi matriks
        for (int i = 0; i < ukuran.baris; ++i)
        {
            getline(file, line);
            stringstream ss(line);
            vector<string> row;
            string token;

            for (int j = 0; j < ukuran.kolom; ++j)
            {
                ss >> token;
                row.push_back(token);
            }
            data.matriks.push_back(row);
        }

        // Membaca jumlah sekuens
        getline(file, line);
        stringstream(line) >> data.numberOfSequences;

        // Membaca sekuens dan bobot
        for (int i = 0; i < data.numberOfSequences; ++i)
        {
            getline(file, line); // Membaca sekuens
            data.sequences.push_back(line);

            getline(file, line); // Membaca bobot
            int bobot;
            stringstream(line) >> bobot;
            data.sequenceReward.push_back(bobot);
        }

        file.close();
        cout << "Data berhasil dimuat dari " << filename << endl;
    }
    else
    {
        cout << "Gagal membuka file untuk dimuat" << endl;
    }
    return data;
}

// Prosedur memuat data dari file
void load()
{
    // Memuat data dari file
    string fileLoad;
    cin >> fileLoad;
    DataLoad data = loadData("../test/" + fileLoad); // SESUAIKAN INPUT USER NAMA FILENYA YANG INGIN DI-LOAD
}

// struct Token
// {
//     string token;
//     vector<Koordinat> koordinat;
// };

// struct evaluasi
// {
//     int bobot;
//     int langkah;
// };

// void findOptimalSolution()
// {
    
// }