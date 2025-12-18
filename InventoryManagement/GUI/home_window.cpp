#include "home_window.h"
#include "ui_home_window.h"
#include "inventory_widget.h"
#include "../datahandler.h"
#include "../seller.h"

home_window::home_window(Manufacturers* manufacturers, Seller *user, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::home_window)
{
    ui->setupUi(this);
    this->setWindowTitle("Inventory Management");

    DataHandler data;
    data.readDataHomeWindow(manufacturers, user);

    connect(this, &QDialog::finished, this, &home_window::dialogClosed);

    m_manufacturers = manufacturers;
    m_user = user;

    // 1. Inisialisasi data graf saat aplikasi dimulai
    setupGraphData();

    setLabels();
    on_PB_inventory_clicked();
}

home_window::~home_window()
{
    delete ui;
}

// Fungsi untuk memasukkan data gudang Anda
void home_window::setupGraphData() {
    // Menambah Gudang
    network.addWarehouse("A", "Jakarta", "Gudang Utama Jakarta");
    network.addWarehouse("B", "Bandung", "Gudang Area Bandung");
    network.addWarehouse("C", "Semarang", "Gudang Jawa Tengah");
    network.addWarehouse("D", "Surabaya", "Gudang Jawa Timur");
    network.addWarehouse("E", "Transit", "Gudang Hub Transit");

    // Membuat Rute (Jarak dalam KM)
    network.addRoute("A", "B", 150);
    network.addRoute("A", "E", 210);
    network.addRoute("B", "E", 130);
    network.addRoute("E", "C", 230);
    network.addRoute("C", "D", 310);
    network.addRoute("B", "D", 450);
}

void home_window::on_PB_inventory_clicked(){
    pushButtonsDefaultStyle();
    closeWidgets();

    setPushButtonSelectedtStyle(ui->PB_inventory, ":/Images/icons/inventoryIcon.png");

    // --- BAGIAN PEMANGGILAN GRAF (DINONAKTIFKAN) ---
    // Kode di bawah ini di-comment karena popup muncul setiap klik tombol inventory
    // Jika ingin mengaktifkan kembali, hapus comment-nya
    /*
    int totalKm = 0;
    std::vector<std::string> path = network.getShortestPath("A", "D", totalKm);

    if (totalKm != -1) {
        QString ruteTeks = "Rute Tercepat Ditemukan:\n";
        for (size_t i = 0; i < path.size(); ++i) {
            ruteTeks += QString::fromStdString(path[i]);
            if (i < path.size() - 1) ruteTeks += " -> ";
        }
        ruteTeks += QString("\n\nTotal Jarak: %1 KM").arg(totalKm);

        QMessageBox::information(this, "Analisis Jalur Logistik", ruteTeks);
    }
    */
    // -------------------------------

    inventory_widget* inventoryWidget = new inventory_widget(m_manufacturers, m_user, ui->mainWidget);
    inventoryWidget->setObjectName("inventory_widget");
    inventoryWidget->show();
}

// --- Fungsi Style (Tetap Sama) ---
void home_window::setPushButtonDefaultStyle(QPushButton* button, const QString& iconPath){
    button->setStyleSheet("QPushButton { background-image: url(" + iconPath + "); background-position: center; background-repeat: no-repeat; background-size: contain; border-radius: 0px; padding: 50px 10px; } QPushButton:hover { background-color: #ecebfb; }");
}

void home_window::setPushButtonSelectedtStyle(QPushButton* button, const QString& iconPath){
    button->setStyleSheet("QPushButton { background-image: url("+ iconPath +"); background-color: #ecebfb; background-position: center; background-repeat: no-repeat; background-size: contain; border-radius: 0px; padding: 50px 10px; }");
}

void home_window::pushButtonsDefaultStyle(){
    setPushButtonDefaultStyle(ui->PB_inventory, ":/Images/icons/inventoryIcon.png");
}

void home_window::closeWidgets(){
    closeChildWidget(ui->mainWidget, "inventory_widget");
}

void home_window::closeChildWidget(QWidget* parent, const QString& widgetName){
    QWidget* childWidget = parent->findChild<QWidget*>(widgetName);
    if (childWidget) childWidget->deleteLater();
}

void home_window::setLabels(){
    if (m_user) {
        QString userHeader = QString::fromStdString(m_user->getName()) + " " + QString::fromStdString(m_user->getLastName());
        ui->LB_userHeader->setText(userHeader);
    }
}
