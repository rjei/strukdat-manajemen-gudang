#include "inventory_widget.h"
#include "ui_inventory_widget.h"
#include "../manufacturers.h"
#include "../seller.h"
#include "../products.h"
#include "../product.h"
#include <QStandardItemModel>

inventory_widget::inventory_widget(Manufacturers* manufacturers, Seller *user, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::inventory_widget)
{
    ui->setupUi(this);
    m_manufacturers = manufacturers;
    m_user = user;

    setupGraphData();
    
    // Mengisi tabel dengan data produk
    populateTable();

    // Catatan: Koneksi untuk PB_showGraph sudah otomatis oleh Qt
    // karena nama fungsi mengikuti pola on_<objectName>_<signal>
}

inventory_widget::~inventory_widget()
{
    delete ui;
}

void inventory_widget::setupGraphData() {
    // Data inisialisasi sesuai graf_lokasi_gudang.cpp
    warehouseNetwork.addWarehouse("A", "Jakarta", "Gudang Jakarta");
    warehouseNetwork.addWarehouse("B", "Bandung", "Gudang Bandung");
    warehouseNetwork.addWarehouse("C", "Semarang", "Gudang Semarang");
    warehouseNetwork.addWarehouse("D", "Surabaya", "Gudang Surabaya");
    warehouseNetwork.addWarehouse("E", "Transit", "Gudang Transit");

    warehouseNetwork.addRoute("A", "B", 150);
    warehouseNetwork.addRoute("A", "E", 210);
    warehouseNetwork.addRoute("B", "E", 130);
    warehouseNetwork.addRoute("E", "C", 230);
    warehouseNetwork.addRoute("C", "D", 310);
    warehouseNetwork.addRoute("B", "D", 450);
}

void inventory_widget::on_PB_showGraph_clicked() {
    QStringList items = {"A", "B", "C", "D", "E"};
    bool okAsal, okTujuan;

    QString asal = QInputDialog::getItem(this, "Analisis Rute", "Pilih Gudang Asal:", items, 0, false, &okAsal);
    if (!okAsal) return;

    QString tujuan = QInputDialog::getItem(this, "Analisis Rute", "Pilih Gudang Tujuan:", items, 0, false, &okTujuan);
    if (!okTujuan) return;

    int totalKm = 0;
    // Panggil logika Dijkstra dari warehouse_graph.cpp
    std::vector<std::string> path = warehouseNetwork.getShortestPath(asal.toStdString(), tujuan.toStdString(), totalKm);

    if (totalKm != -1) {
        QString res = "Jalur Tercepat:\n";
        for (size_t i = 0; i < path.size(); ++i) {
            res += QString::fromStdString(path[i]) + (i == path.size() - 1 ? "" : " -> ");
        }
        res += "\n\nTotal Jarak: " + QString::number(totalKm) + " KM";
        QMessageBox::information(this, "Hasil Dijkstra", res);
    } else {
        QMessageBox::warning(this, "Error", "Rute tidak ditemukan!");
    }
}

void inventory_widget::on_PB_remove_clicked() {
    // Kode remove asli proyek tetap di sini
}

void inventory_widget::populateTable() {
    if (!m_user) return;
    
    const Products& products = m_user->getProductsModel();
    const std::vector<Product>& productList = products.getProducts();
    
    // Membuat model untuk tabel
    QStandardItemModel* model = new QStandardItemModel(this);
    
    // Set header kolom
    model->setHorizontalHeaderLabels({
        "Name", "SKU", "Brand", "Category", "Stock", 
        "Available", "Price", "Unit", "Added Date", "Exp Date", "Status"
    });
    
    // Mengisi data ke model
    for (size_t row = 0; row < productList.size(); ++row) {
        const Product& prod = productList[row];
        
        model->setItem(row, 0, new QStandardItem(QString::fromStdString(prod.getName())));
        model->setItem(row, 1, new QStandardItem(QString::fromStdString(prod.getSku())));
        model->setItem(row, 2, new QStandardItem(QString::fromStdString(prod.getBrand())));
        model->setItem(row, 3, new QStandardItem(QString::fromStdString(prod.getCategory())));
        model->setItem(row, 4, new QStandardItem(QString::number(prod.getStock())));
        model->setItem(row, 5, new QStandardItem(QString::number(prod.getAvailable())));
        model->setItem(row, 6, new QStandardItem(QString::number(prod.getPrice(), 'f', 2)));
        model->setItem(row, 7, new QStandardItem(QString::fromStdString(prod.getUnit())));
        model->setItem(row, 8, new QStandardItem(QString::fromStdString(prod.getAddedDate())));
        model->setItem(row, 9, new QStandardItem(QString::fromStdString(prod.getExDate())));
        model->setItem(row, 10, new QStandardItem(prod.getAvailability() ? "Available" : "Unavailable"));
    }
    
    // Set model ke TableView
    ui->TV_products->setModel(model);
    
    // Menyesuaikan lebar kolom
    ui->TV_products->resizeColumnsToContents();
}
