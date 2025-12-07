#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

// --- DEFINISI MAKRO & STRUCT (Management & Struct) ---
#define MAX_USERS 100
#define FILE_NAME "data_nasabah.txt"

// Struct untuk menampung data nasabah (Type Data & Struct)
typedef struct {
    char noRek[20];
    char pin[10];
    char nama[50];
    long long saldo; // Tipe data long long untuk uang
} Account;

// --- PROTOTYPE FUNGSI (Function, Parameter, Return Value) ---
void loadData(Account users[], int *count);
void saveData(Account users[], int count);
int searchUser(Account users[], int count, char *inputRek); // Searching
void bubbleSortUsers(Account users[], int count); // Sorting
void menuUtama(Account *user, Account users[], int count, int userIndex);
void menuAdmin(Account users[], int *count);
void tarikTunai(Account *user); // Pointer
void setorTunai(Account *user); // Pointer
void transfer(Account *user, Account users[], int count);
void addNasabah(Account users[], int *count);
void deleteNasabah(Account users[], int *count);
double hitungBungaRekursif(double saldo, double bunga, int tahun); // Rekursi

// --- MAIN PROGRAM (Iterasi/Looping Utama) ---
int main() {
    Account users[MAX_USERS]; // Array of Struct
    int userCount = 0;
    int pilihan;
    char inputRek[20], inputPin[10];
    int userIndex = -1;

    // Coba load data, jika gagal, buat data demo
    loadData(users, &userCount);
    if (userCount == 0) {
        printf("File data tidak ditemukan. Membuat data demo...\n");
        // Data Dummy (Admin dan 2 Nasabah)
        strcpy(users[0].noRek, "11223"); strcpy(users[0].pin, "5555"); strcpy(users[0].nama, "Administrator Bank"); users[0].saldo = 99999999;
        strcpy(users[1].noRek, "12345"); strcpy(users[1].pin, "1234"); strcpy(users[1].nama, "Budi Santoso"); users[1].saldo = 5000000;
        strcpy(users[2].noRek, "67890"); strcpy(users[2].pin, "0000"); strcpy(users[2].nama, "Siti Aminah"); users[2].saldo = 1500000;
        userCount = 3;
        saveData(users, userCount);
    }

    do { // Looping Utama Program
        system("cls"); // Output
        printf("=== SELAMAT DATANG DI ATM C-BANK ===\n");
        printf("1. Login\n");
        printf("2. Keluar\n");
        printf("Pilihan: ");
        if (scanf("%d", &pilihan) != 1) { // Input
            // Handle input non-integer
            while (getchar() != '\n');
            continue;
        }

        switch (pilihan) { // Switch Case
            case 1:
                printf("\nMasukkan No Rekening: ");
                scanf("%s", inputRek);
                
                userIndex = searchUser(users, userCount, inputRek); // Searching
                
                if (userIndex != -1) {
                    printf("Masukkan PIN: ");
                    scanf("%s", inputPin);
                    
                    if (strcmp(users[userIndex].pin, inputPin) == 0) {
                        // Cek apakah yang login adalah Admin (No Rek: 11223)
                        if (strcmp(users[userIndex].noRek, "11223") == 0) {
                            menuAdmin(users, &userCount);
                        } else {
                            menuUtama(&users[userIndex], users, userCount, userIndex); // Pointer digunakan di sini
                            saveData(users, userCount); 
                        }
                    } else {
                        printf("PIN Salah!\n");
                        getchar(); getchar();
                    }
                } else {
                    printf("No Rekening tidak ditemukan!\n");
                    getchar(); getchar();
                }
                break;

            case 2:
                printf("Terima kasih telah menggunakan ATM kami.\n");
                break;
                
            default:
                printf("Pilihan tidak valid.\n");
                getchar(); getchar();
        }
    } while (pilihan != 2);

    return 0;
}

// --- IMPLEMENTASI FUNGSI FILE MANAGEMENT ---

// File Management: Membaca data dari file
void loadData(Account users[], int *count) {
    FILE *file = fopen(FILE_NAME, "r"); 
    if (file != NULL) {
        *count = 0;
        // fscanf: membaca data berdasarkan format yang ditentukan
        while (fscanf(file, "%[^,],%[^,],%[^,],%lld\n", 
               users[*count].noRek, users[*count].pin, users[*count].nama, &users[*count].saldo) != EOF) {
            (*count)++;
        }
        fclose(file);
    }
}

// File Management: Menyimpan data ke file
void saveData(Account users[], int count) {
    FILE *file = fopen(FILE_NAME, "w"); 
    if (file != NULL) {
        for (int i = 0; i < count; i++) {
            // fprintf: menulis data ke file dengan format CSV (comma-separated values)
            fprintf(file, "%s,%s,%s,%lld\n", users[i].noRek, users[i].pin, users[i].nama, users[i].saldo);
        }
        fclose(file);
    }
}

// Searching: Mencari index user berdasarkan No Rek (Return Value: index atau -1)
int searchUser(Account users[], int count, char *inputRek) {
    for (int i = 0; i < count; i++) { // Iterasi
        if (strcmp(users[i].noRek, inputRek) == 0) {
            return i; 
        }
    }
    return -1; 
}

// Sorting: Mengurutkan user berdasarkan saldo (Bubble Sort)
void bubbleSortUsers(Account users[], int count) {
    for (int i = 0; i < count - 1; i++) { // Looping (Iterasi)
        for (int j = 0; j < count - i - 1; j++) {
            if (users[j].saldo < users[j+1].saldo) { 
                Account temp = users[j]; // Variabel
                users[j] = users[j+1];
                users[j+1] = temp;
            }
        }
    }
}

// Rekursi: Menghitung estimasi investasi (Bunga Majemuk)
double hitungBungaRekursif(double saldo, double bunga, int tahun) {
    if (tahun == 0) {
        return saldo; // Base case
    } else {
        // Recursive call
        return hitungBungaRekursif(saldo * (1 + bunga), bunga, tahun - 1); 
    }
}

// --- FUNGSI NASABAH BIASA ---

void menuUtama(Account *user, Account users[], int count, int userIndex) {
    int pil;
    do {
        system("cls");
        printf("Halo, %s!\n", user->nama); // Mengakses struct via pointer (->)
        printf("Saldo: Rp %lld\n", user->saldo);
        printf("----------------------\n");
        printf("1. Setor Tunai\n");
        printf("2. Tarik Tunai\n");
        printf("3. Transfer\n");
        printf("4. Simulasi Investasi (Rekursi)\n");
        printf("5. Logout\n");
        printf("Pilihan: ");
        scanf("%d", &pil);

        switch(pil) {
            case 1: setorTunai(user); break;
            case 2: tarikTunai(user); break;
            case 3: transfer(user, users, count); break;
            case 4: {
                int thn;
                printf("Masukkan rencana tahun investasi: ");
                scanf("%d", &thn);
                double hasil = hitungBungaRekursif((double)user->saldo, 0.05, thn); // Panggilan Rekursif
                printf("Estimasi saldo %d tahun ke depan (Bunga 5%%): Rp %.2f\n", thn, hasil);
                printf("Tekan enter...");
                getchar(); getchar();
                break;
            }
        }
    } while (pil != 5);
}

void setorTunai(Account *user) {
    long long setor;
    printf("Masukkan jumlah setor: Rp ");
    scanf("%lld", &setor);
    // If Else Logic
    if (setor > 0) {
        user->saldo += setor; // Operator aritmatika & Pointer
        printf("Setor berhasil!\n");
    } else {
        printf("Nominal tidak valid.\n");
    }
    getchar(); getchar();
}

void tarikTunai(Account *user) {
    long long tarik;
    printf("Masukkan jumlah tarik: Rp ");
    scanf("%lld", &tarik);
    
    // If Else Logic & Operator
    if (tarik > user->saldo) {
        printf("Saldo tidak mencukupi!\n");
    } else if (tarik <= 0) {
        printf("Nominal tidak valid.\n");
    } else {
        user->saldo -= tarik;
        printf("Penarikan berhasil. Sisa saldo: %lld\n", user->saldo);
    }
    getchar(); getchar();
}

void transfer(Account *user, Account users[], int count) {
    char tujuanRek[20];
    long long nominal;
    
    printf("Masukkan Rekening Tujuan: ");
    scanf("%s", tujuanRek);
    
    if (strcmp(tujuanRek, user->noRek) == 0) { // Operator Boolean & Perbandingan
        printf("Tidak bisa transfer ke diri sendiri!\n");
        getchar(); getchar();
        return;
    }

    int indexTujuan = searchUser(users, count, tujuanRek);
    
    if (indexTujuan != -1) {
        printf("Tujuan: %s\n", users[indexTujuan].nama);
        printf("Nominal Transfer: Rp ");
        scanf("%lld", &nominal);
        
        if (nominal > 0 && nominal <= user->saldo) {
            user->saldo -= nominal; // Pointer
            users[indexTujuan].saldo += nominal; // Array
            printf("Transfer Berhasil!\n");
        } else {
            printf("Saldo tidak cukup atau nominal salah.\n");
        }
    } else {
        printf("Rekening tujuan tidak ditemukan.\n");
    }
    getchar(); getchar();
}

// --- FUNGSI ADMIN ---

// Fungsi untuk Menu Admin
void menuAdmin(Account users[], int *count) {
    int pil;
    do {
        system("cls");
        printf("=== MENU ADMINISTRATOR ===\n");
        printf("1. Tambah Nasabah Baru\n");
        printf("2. Hapus Nasabah\n");
        printf("3. Lihat Semua Nasabah (Sorting)\n");
        printf("4. Logout (Keluar dari Menu Admin)\n");
        printf("Pilihan: ");
        scanf("%d", &pil);

        switch(pil) { // Switch Case
            case 1:
                addNasabah(users, count); 
                printf("\nTekan enter kembali...");
                getchar(); getchar();
                break;
            case 2:
                deleteNasabah(users, count); 
                printf("\nTekan enter kembali...");
                getchar(); getchar();
                break;
            case 3:
                // Menampilkan hasil Sorting
                bubbleSortUsers(users, *count);
                printf("\n--- DATA NASABAH (Diurutkan Saldo Tertinggi) ---\n");
                printf("%-15s %-20s %-15s\n", "No Rek", "Nama", "Saldo");
                printf("----------------------------------------------------\n");
                for(int i = 0; i < *count; i++) {
                    printf("%-15s %-20s Rp %lld\n", users[i].noRek, users[i].nama, users[i].saldo);
                }
                printf("\nTekan enter kembali...");
                getchar(); getchar();
                break;
            case 4:
                saveData(users, *count); 
                printf("Logout dari Menu Admin.\n");
                break;
            default:
                printf("Pilihan tidak valid.\n");
        }
    } while (pil != 4);
}

// Fungsi untuk menambah nasabah baru (Pointer untuk count)
void addNasabah(Account users[], int *count) {
    if (*count >= MAX_USERS) {
        printf("\nERROR: Kapasitas maksimum nasabah (%d) sudah tercapai.\n", MAX_USERS);
        return;
    }

    printf("\n--- TAMBAH NASABAH BARU ---\n");
    printf("Masukkan No Rekening (unik): ");
    scanf("%s", users[*count].noRek);

    // Cek duplikasi menggunakan Searching
    if (searchUser(users, *count, users[*count].noRek) != -1) {
        printf("ERROR: No Rekening sudah terdaftar. Pembatalan penambahan.\n");
        return;
    }

    printf("Masukkan PIN (4 digit): ");
    scanf("%s", users[*count].pin);
    printf("Masukkan Nama Lengkap: ");
    getchar(); 
    fgets(users[*count].nama, sizeof(users[*count].nama), stdin);
    users[*count].nama[strcspn(users[*count].nama, "\n")] = 0; // Hapus newline

    printf("Masukkan Saldo Awal: Rp ");
    scanf("%lld", &users[*count].saldo);

    (*count)++; // Pointer untuk mengubah count
    printf("\nNasabah %s berhasil ditambahkan!\n", users[*count-1].nama);
    saveData(users, *count); 
}

// Fungsi untuk menghapus nasabah
void deleteNasabah(Account users[], int *count) {
    char rekHapus[20];
    printf("\n--- HAPUS NASABAH ---\n");
    printf("Masukkan No Rekening yang ingin dihapus: ");
    scanf("%s", rekHapus);

    int indexHapus = searchUser(users, *count, rekHapus); // Searching

    if (indexHapus != -1) {
        printf("APAKAH ANDA YAKIN INGIN MENGHAPUS NASABAH '%s' (No Rek: %s)? (y/n): ", 
               users[indexHapus].nama, users[indexHapus].noRek);
        char konfirmasi;
        scanf(" %c", &konfirmasi); 

        if (konfirmasi == 'y' || konfirmasi == 'Y') {
            // Proses penghapusan: memindahkan semua data setelah indexHapus ke depan (Iterasi)
            for (int i = indexHapus; i < *count - 1; i++) {
                users[i] = users[i+1]; // Array manipulation
            }
            (*count)--; 

            printf("Nasabah berhasil dihapus!\n");
            saveData(users, *count); 
        } else {
            printf("Penghapusan dibatalkan.\n");
        }
    } else {
        printf("ERROR: No Rekening tidak ditemukan.\n");
    }
}
