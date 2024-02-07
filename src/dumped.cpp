#include <iostream>
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

struct Data
{
    int bobotHadiah;
    string isiBuffer;
    vector<Koordinat> koordinatList;
};

// Fungsi untuk memuat data dari file
Data loadData(const string &filename)
{
    Data data;
    ifstream file(filename);

    if (file.is_open())
    {
        string line;
        // Membaca bobot hadiah
        getline(file, line);
        stringstream(line) >> data.bobotHadiah;

        // Membaca isi buffer
        getline(file, data.isiBuffer);

        // Membaca koordinat
        while (getline(file, line))
        {
            Koordinat koordinat;
            stringstream ss(line);
            char comma;
            ss >> koordinat.x >> comma >> koordinat.y;
            if (ss)
            { // Memastikan masih ada data yang bisa dibaca
                data.koordinatList.push_back(koordinat);
            }
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

void load()
{
    // Memuat data dari file
    Data loadedData = loadData("data.txt");

    // Menampilkan data yang dimuat
    cout << "Bobot Hadiah: " << loadedData.bobotHadiah << endl;
    cout << "Isi Buffer: " << loadedData.isiBuffer << endl;
    cout << "Koordinat:" << endl;
    for (const Koordinat &koordinat : loadedData.koordinatList)
    {
        cout << koordinat.x << ", " << koordinat.y << endl;
    }
}

/*
buffer_size
matrix_width matrix_height
matrix
number_of_sequences
sequences_n
sequences_n_reward
*/