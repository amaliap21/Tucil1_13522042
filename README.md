# Cyberpunk 2077

Penyelesaian Cyberpunk 2077 Breach Protocol dengan Algoritma Brute Force

## Table of Contents

- [Deskripsi Singkat](#deskripsi-singkat)
- [Kompilasi Program](#kompilasi-program)
- [Alur Program](#alur-program)
- [Creator](#creator)

## Deskripsi Singkat

Cyberpunk 2077 Breach Protocol adalah minigame meretas pada permainan video Cyberpunk 2077. Minigame ini merupakan simulasi peretasan jaringan local dari ICE (Intrusion Countermeasures Electronics) pada permainan Cyberpunk 2077. Komponen pada permainan ini antara lain adalah:

1. Token – terdiri dari dua karakter alfanumerik (kombinasi [A..Z] dan/atau [0..9]), seperti E9, BD, dan 55.
2. Matriks – terdiri atas token-token yang akan dipilih untuk menyusun urutan kode.
3. Sekuens – sebuah rangkaian token (dua atau lebih) yang harus dicocokkan.
4. Buffer – jumlah maksimal token yang dapat disusun secara sekuensial.

## Kompilasi Program

1. Clone repositori ini.

```
$ git clone https://github.com/amaliap21/Tucil1_13522042.git
```

2. Pada current directory: `Tucil1_13522042`, jalankan perintah berikut untuk mengkompilasi program.

### Windows

```
run.bat
```

### Linux

```
run.sh
```

## Alur Program

```
.
└── randomizer
    ├── y/Y
    │   └── Input jumlah token unik,
    │       token,
    │       ukuran buffer,
    │       ukuran matriks,
    │       jumlah sekuens,
    │       dan ukuran maksimal sekuens
    │              └─────────────
    └── n/N                     |
        └── Input nama file     |
            └── Simpan hasil? ───
                ├── y/Y
                │   └── Input nama file yang disimpan
                │       └── Keluar dari program
                └── n/N
                    └── Keluar dari program

```

## Creator

| **NIM**  |   **Nama**   |
| :------: | :----------: |
| 13522042 | Amalia Putri |
