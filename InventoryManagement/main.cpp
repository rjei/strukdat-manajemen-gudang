#include <QApplication>
#include "GUI/home_window.h"
#include "datahandler.h"
#include "manufacturers.h"
#include "seller.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    // 1. Load Data
    DataHandler data;
    Manufacturers* manufacturersData = new Manufacturers;
    data.readDataLogin(manufacturersData);

    // 2. Ambil User Pertama (Default)
    Seller* currentUser = nullptr;
    std::vector<Seller*> sellers = manufacturersData->getManufacturers();

    if (!sellers.empty()) {
        currentUser = sellers[0];
    } else {
        // Jika kosong, buat user dummy agar tidak crash
        currentUser = new Seller();
        currentUser->setName("Default");
        currentUser->setLastName("User");
        currentUser->setUsername("admin");
        currentUser->setPassword("admin");
        currentUser->setNIN("0000000000");
        currentUser->setPhoneNumber("0000000000");
    }

    // 3. Langsung Buka Dashboard
    home_window w(manufacturersData, currentUser);
    w.show();

    return a.exec();
}
