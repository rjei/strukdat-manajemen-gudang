#include <QApplication>
#include "GUI/home_window.h"
#include "datahandler.h"
#include "manufacturers.h"
#include "seller.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    // 1. Inisialisasi DataHandler dan Manufacturers
    // Logika ini sebelumnya ada di constructor MainWindow, kita pindahkan ke sini.
    DataHandler data;
    Manufacturers* manufacturersData = new Manufacturers;
    data.readDataLogin(manufacturersData);

    // 2. Pilih Pengguna Default (Auto-Login)
    // Karena kita melewati layar login, kita harus menentukan user mana yang sedang aktif.
    // Di sini kita mengambil user pertama yang tersedia dari database.
    Seller* currentUser = nullptr;
    std::vector<Seller*> sellers = manufacturersData->getManufacturers();

    if (!sellers.empty()) {
        currentUser = sellers[0]; // Menggunakan user pertama sebagai default

        // Alternatif: Jika ingin login sebagai user tertentu (misal "A.Benzema"):
        // try { currentUser = manufacturersData->editSeller("A.Benzema"); } catch(...) {}
    }

    // 3. Tampilkan Dashboard (Home Window) jika user ditemukan
    if (currentUser) {
        home_window w(manufacturersData, currentUser);
        w.show();

        // Bersihkan memori manufacturersData biasanya ditangani saat aplikasi ditutup
        return a.exec();
    } else {
        // Fallback jika tidak ada data user sama sekali
        return 0;
    }
}
