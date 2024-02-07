#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <sstream>

using namespace std;

/***** MENYIMPAN DATA HASIL OUTPUT *****/
/*
1. Bobot Hadiah
2. Isi Buffer *jika ada*
3. Koordinat (dari list koordinat) *jika ada*
4. Waktu eksekusi
*/

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

/***** MEMUAT DATA DARI INPUT (.txt) *****/
/*
1. Ukuran buffer
2. Ukuran matriks (m x n) / baris x kolom
3. Koordinat (dari list koordinat) *jika ada*
4. Waktu eksekusi
*/

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

        // Membaca sekuens dan reward
        for (int i = 0; i < data.numberOfSequences; ++i)
        {
            getline(file, line); // Membaca sekuens
            data.sequences.push_back(line);
            getline(file, line); // Membaca reward
            int reward;
            stringstream(line) >> reward;
            data.sequenceReward.push_back(reward);
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
    DataLoad data = loadData("../test/load.txt");

    // Menampilkan data yang telah dimuat
    cout << "Ukuran buffer: " << data.ukuranBuffer << endl;
    cout << "Ukuran matriks: " << data.ukuranMatriks[0].baris << " x " << data.ukuranMatriks[0].kolom << endl;
    cout << "Isi matriks:" << endl;
    for (const vector<string> &row : data.matriks)
    {
        for (const string &cell : row)
        {
            cout << cell << " ";
        }
        cout << endl;
    }
    cout << "Jumlah sekuens: " << data.numberOfSequences << endl;
    for (int i = 0; i < data.numberOfSequences; ++i)
    {
        cout << "Sekuens: " << data.sequences[i] << ", reward: " << data.sequenceReward[i] << endl;
    }
}

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

int main()
{
    /*  SAVING DATA (output)
     */
    // DataSave yang akan disimpan
    DataSave dataToSave = {50, "7A BD 7A BD 1C BD 55", {{1, 1}, {1, 4}, {3, 4}, {3, 5}, {6, 5}, {6, 4}, {5, 4}}};

    // Simpan data ke file
    saveData("../test/save.txt", dataToSave, 300);

    /*  LOADING DATA (input)
     */
    load();

    return 0;
}