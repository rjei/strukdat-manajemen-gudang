#include "inventory_widget.h"
#include "ui_inventory_widget.h"
#include "add_product_window.h"
#include <QStringListModel>
#include <QMessageBox>
#include <QHeaderView>
#include <QInputDialog>
#include <QDialog>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QComboBox>
#include <QTextEdit>
#include "edit_product_window.h"
#include "../datahandler.h"

inventory_widget::inventory_widget(Manufacturers* manufacturers, Seller* user,QWidget *parent) :
    QWidget(parent),
    ui(new Ui::inventory_widget)
{
    ui->setupUi(this);

    customizeListItems();

    m_manufacturers = manufacturers;
    m_user = user;

    initializeTableView();

    updateTable();

    updateFilterBrand();
    updateFilterCategory();

    // Connect the selectionChanged signal of the brand list view's selection model
    // to the onSelectionChangedBrands slot of the purchase_widget
    connect(ui->LV_brandList->selectionModel(), &QItemSelectionModel::selectionChanged,
            this, &inventory_widget::onSelectionChangedBrands);

    // Connect the selectionChanged signal of the category list view's selection model
    // to the onSelectionChangedCategories slot of the purchase_widget
    connect(ui->LV_categoryList->selectionModel(), &QItemSelectionModel::selectionChanged,
            this, &inventory_widget::onSelectionChangedCategories);

    connect(ui->LE_search, &QLineEdit::textChanged, this, &inventory_widget::onTextChanged);

    // Inisialisasi data graf gudang
    setupGraphData();
}

inventory_widget::~inventory_widget()
{
    delete ui;
}

void inventory_widget::customizeListItems(){
    class CustomItemDelegate_QListView : public QStyledItemDelegate {
    public:
        void paint(QPainter* painter, const QStyleOptionViewItem& option, const QModelIndex& index) const override {
            if (option.state & QStyle::State_Selected) {
                // Customize the selected item's appearance
                painter->save();

                // Draw a rounded border around the selected item
                QColor bgColor(179,183,255);        // Background color of the selected item
                int borderRadius = 8;             // Radius of the rounded corners
                int borderWidth = 4;              // Width of the border

                painter->setRenderHint(QPainter::Antialiasing, true); // Enable antialiasing for smooth edges
                painter->setPen(Qt::NoPen);       // No outline pen for the background
                painter->setBrush(bgColor);       // Set the background color
                painter->drawRoundedRect(option.rect.adjusted(borderWidth, borderWidth, -borderWidth, -borderWidth), borderRadius, borderRadius);
                painter->restore();
            } else {
                // Customize the non-selected item's appearance
                painter->fillRect(option.rect, Qt::transparent);
            }

            // Draw the text centered within the item's rectangle
            painter->setPen(Qt::black);            // Set the text color to black
            painter->setFont(QFont("Calibri", 9, QFont::Weight::Medium)); // Set the font and size
            painter->drawText(option.rect, Qt::AlignCenter, index.data().toString());
        }
    };

    CustomItemDelegate_QListView* listDelegate = new CustomItemDelegate_QListView ;
    ui->LV_brandList->setItemDelegate(listDelegate);
    ui->LV_categoryList->setItemDelegate(listDelegate);
    ui->LV_brandList->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->LV_categoryList->setEditTriggers(QAbstractItemView::NoEditTriggers);
}

void inventory_widget::initializeTableView(){
    // Custom delegate for center-aligned text
    class CenterAlignmentDelegate : public QStyledItemDelegate {
    public:
        explicit CenterAlignmentDelegate(QObject* parent = nullptr) : QStyledItemDelegate(parent) {}

        void paint(QPainter* painter, const QStyleOptionViewItem& option, const QModelIndex& index) const override {
            QStyleOptionViewItem centeredOption = option;
            centeredOption.displayAlignment = Qt::AlignCenter;

            // --- TAMBAHKAN BAGIAN INI ---
            // Memaksa warna teks (Text) menjadi Hitam
            centeredOption.palette.setColor(QPalette::Text, Qt::black);

            // Opsional: Memaksa warna teks saat dipilih (HighlightedText) juga menjadi Hitam
            // (Jika tidak ditambahkan, teks akan berwarna putih saat baris dipilih)
            centeredOption.palette.setColor(QPalette::HighlightedText, Qt::black);
            // ---------------------------

            QStyledItemDelegate::paint(painter, centeredOption, index);
        }
    };

    // Create and set the custom delegate for center alignment
    ui->TV_products->setItemDelegate(new CenterAlignmentDelegate(ui->TV_products));

    // Kolom baru: ID, Nama Produk, Merek, Kategori, Harga (Rupiah), Stok
    m_tableViewModel.setHorizontalHeaderItem(0 ,new QStandardItem("ID"));
    m_tableViewModel.setHorizontalHeaderItem(1 ,new QStandardItem("Nama Produk"));
    m_tableViewModel.setHorizontalHeaderItem(2 ,new QStandardItem("Merek"));
    m_tableViewModel.setHorizontalHeaderItem(3 ,new QStandardItem("Kategori"));
    m_tableViewModel.setHorizontalHeaderItem(4 ,new QStandardItem("Harga (Rp)"));
    m_tableViewModel.setHorizontalHeaderItem(5 ,new QStandardItem("Stok"));

    ui->TV_products->setModel(&m_tableViewModel);
    ui->TV_products->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->TV_products->setSelectionMode(QAbstractItemView::SingleSelection);

    // Buat semua kolom selalu mengisi penuh lebar tabel (tidak ada ruang kosong di kanan)
    ui->TV_products->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);

    ui->TV_products->horizontalHeader()->setSectionsClickable(true);

    // Connect the table header signals to the respective slots for sorting table elements
    connect(ui->TV_products->horizontalHeader(), &QHeaderView::sectionDoubleClicked, this, &inventory_widget::handleHeaderDoubleClicked);
    connect(ui->TV_products->horizontalHeader(), &QHeaderView::sectionClicked, this, &inventory_widget::handleHeaderClicked);
}

void inventory_widget::updateTable() {
    const std::vector<Product>& products = m_user->getProductsModel().getProducts();

    // Clear the table
    m_tableViewModel.removeRows(0, m_tableViewModel.rowCount());

    int row = 0;
    for (const Product& product : products){
        // ID: gunakan SKU sebagai ID yang ditampilkan
        QStandardItem* ID = new QStandardItem(QString::fromStdString(product.getSku()));
        ID->setData(Qt::AlignCenter, Qt::TextAlignmentRole);
        m_tableViewModel.setItem(row,0 ,ID);
        m_tableViewModel.setItem(row,1 ,new QStandardItem(QString::fromStdString(product.getName())));      // Nama Produk
        m_tableViewModel.setItem(row,2 ,new QStandardItem(QString::fromStdString(product.getBrand())));     // Merek
        m_tableViewModel.setItem(row,3 ,new QStandardItem((QString::fromStdString(product.getCategory())))); // Kategori
        m_tableViewModel.setItem(row,4 ,new QStandardItem("Rp. " + QString::number(product.getPrice(), 'f' , 2))); // Harga (Rp)
        m_tableViewModel.setItem(row,5 ,new QStandardItem(QString::number(product.getStock())));            // Stok
        row++;
    }

    ui->LE_search->setText("");
}

void inventory_widget::updateFilterBrand(){
    QStringList brands;
    for (const Product& product : m_user->getProductsModel().getProducts()){
        QString brand = QString::fromStdString(product.getBrand());
        if (!brands.contains(brand))
            brands << brand;
    }

    m_brandFilterList.setStringList(brands);
    ui->LV_brandList->setModel(&m_brandFilterList);
}

void inventory_widget::updateFilterCategory(){
    QStringList categories;
    for (const Product& product : m_user->getProductsModel().getProducts()){
        QString category = QString::fromStdString(product.getCategory());
        if (!categories.contains(category))
            categories << category;
    }

    m_categoryFilterList.setStringList(categories);
    ui->LV_categoryList->setModel(&m_categoryFilterList);
}

void inventory_widget::removeProduct(int row){
    // Get the index of the product SKU in the first column of the table view
    QModelIndex index = ui->TV_products->model()->index(row, 0);
    QString productSKU = ui->TV_products->model()->data(index, Qt::DisplayRole).toString();

    // Create a message box to confirm the removal of the product
    QMessageBox* messageBox = new QMessageBox(this);
    messageBox->setText("Apakah Anda yakin ingin menghapus produk dengan kode SKU " + productSKU +" dari inventaris?");
    messageBox->setWindowTitle("Hapus Produk");
    messageBox->addButton(QMessageBox::Yes);
    messageBox->addButton(QMessageBox::No);

    // Display the message box and wait for the user's response
    int buttonClicked = messageBox->exec();

    // Check if the user clicked "Yes"
    if (buttonClicked == QMessageBox::Yes){
        // Remove the product from the user's inventory
        m_user->removerProduct(productSKU.toStdString());

        // Remove the product from the data storage
        DataHandler data;
        data.removeProduct(productSKU.toStdString(), m_user->getMID());
    }
    else
        return;
}

void inventory_widget::search(const QString &text){
    updateTableViewWithSearchCriteria(text.toLower(), &Product::getName);
}

template<typename MemberFunction>
void inventory_widget::updateTableViewWithSearchCriteria(const QString& text, MemberFunction memberFunction) {
    const std::vector<Product>& products = m_user->getProductsModel().getProducts();

    // Clear the table
    m_tableViewModel.removeRows(0, m_tableViewModel.rowCount());

    int row = 0;

    for (const Product& product : products) {
        QString value = QString::fromStdString((product.*memberFunction)()).toLower();

        if (value.startsWith(text)) {
            // ID menggunakan SKU, kolom-kolom disesuaikan dengan tabel baru
            m_tableViewModel.setItem(row, 0, new QStandardItem(QString::fromStdString(product.getSku()))); // ID
            m_tableViewModel.setItem(row, 1, new QStandardItem(QString::fromStdString(product.getName()))); // Nama Produk
            m_tableViewModel.setItem(row, 2, new QStandardItem(QString::fromStdString(product.getBrand()))); // Merek
            m_tableViewModel.setItem(row, 3, new QStandardItem(QString::fromStdString(product.getCategory()))); // Kategori
            m_tableViewModel.setItem(row, 4, new QStandardItem("Rp. " + QString::number(product.getPrice(), 'f', 2))); // Harga (Rp)
            m_tableViewModel.setItem(row, 5, new QStandardItem(QString::number(product.getStock()))); // Stok
            row++;
        }
    }
}

void inventory_widget::on_TV_products_doubleClicked(const QModelIndex &index){
    // Get the row index of the double-clicked item
    int row = index.row();

    // Retrieve the SKU (item at column 0) from the model
    std::string sku = m_tableViewModel.item(row,0)->data(Qt::DisplayRole).toString().toStdString();

    // Get a reference to the product based on the SKU
    Product& product = m_user->editProducts().editProduct(sku);

    // Create the edit product window
    edit_product_window* editProductWindow = new edit_product_window(m_user, &product, this);

    // Connect the windowClosed signal from editProductWindow to the onDialogClosed slot
    connect(editProductWindow, &edit_product_window::windowClosed, this, &inventory_widget::onDialogClosed);

    // Set the window as modal and show it
    editProductWindow->setModal(true);
    editProductWindow->show();
}

void inventory_widget::on_PB_add_clicked(){
    // Create the add product window
    add_product_window* addProductWindow = new add_product_window(m_user, this);

    // Connect the dialogClosed signal from addProductWindow to the onDialogClosed slot
    connect(addProductWindow, &add_product_window::dialogClosed, this, &inventory_widget::onDialogClosed);

    // Set the window as modal and show it
    addProductWindow->setModal(true);
    addProductWindow->show();
}


void inventory_widget::onDialogClosed(){
    updateFilterBrand();
    updateFilterCategory();
    updateTable();
}

void inventory_widget::on_PB_remove_clicked() {
    // Get the selection model for the table view
    QItemSelectionModel* selectionModel = ui->TV_products->selectionModel();
    // Get the currently selected index
    QModelIndex selectedIndex = selectionModel->currentIndex();

    // Check if a valid index is selected
    if (selectedIndex.isValid()) {
        int row = selectedIndex.row();

        removeProduct(row);

        updateFilterBrand();
        updateFilterCategory();
        updateTable();
    }
}

void inventory_widget::on_PB_brandFilter_clicked(){
    // Get the currently selected index
    QModelIndex selectedIndex = ui->LV_brandList->currentIndex();

    if (selectedIndex.isValid()) {
        // Get the selected text directly from the model
        QString brand = selectedIndex.data(Qt::DisplayRole).toString();

        updateTableViewWithSearchCriteria(brand.toLower(), &Product::getBrand);
        ui->LE_search->setText("");
    }

    ui->LV_categoryList->clearSelection();
}

void inventory_widget::onSelectionChangedBrands(const QItemSelection& selected, const QItemSelection& deselected){
    // Check if there is at least one selected item
    if (!selected.isEmpty()) {
        // Enable the QPushButton
        ui->PB_brandFilter->setEnabled(true);
    }
    else {
        // No selected items, disable the QPushButton
        ui->PB_brandFilter->setEnabled(false);
    }
}

void inventory_widget::onSelectionChangedCategories(const QItemSelection &selected, const QItemSelection &deselected){
    // Check if there is at least one selected item
    if (!selected.isEmpty()) {
        // Enable the QPushButton
        ui->PB_categoryFilter->setEnabled(true);
    }
    else {
        // No selected items, disable the QPushButton
        ui->PB_categoryFilter->setEnabled(false);
    }
}

void inventory_widget::on_PB_categoryFilter_clicked(){
    // Get the currently selected index
    QModelIndex selectedIndex = ui->LV_categoryList->currentIndex();

    if (selectedIndex.isValid()) {
        // Get the selected text directly from the model
        QString category = selectedIndex.data(Qt::DisplayRole).toString();

        updateTableViewWithSearchCriteria(category.toLower(), &Product::getCategory);
        ui->LE_search->setText("");
    }

    ui->LV_brandList->clearSelection();
}

void inventory_widget::handleHeaderDoubleClicked(int logicalIndex){
    switch (logicalIndex){
    case 0:
        m_user->editProducts().sortBySKU();
        break;
    case 1:
        m_user->editProducts().sortByName();
        break;
    case 2:
        m_user->editProducts().sortByBrand();
        break;
    case 3:
        m_user->editProducts().sortByCategory();
        break;
    case 4:
        m_user->editProducts().sortByPrice();
        break;
    case 5:
        m_user->editProducts().sortByStock();
        break;
    case 6:
        m_user->editProducts().sortByAvailable();
        break;
    case 7:
        m_user->editProducts().sortByAddDate();
        break;
    case 8:
        m_user->editProducts().sortByExDate();
        break;
    }
    updateTable();
}

void inventory_widget::handleHeaderClicked(int logicalIndex){
    switch (logicalIndex){
    case 0:
        m_user->editProducts().sortBySKUdes();
        break;
    case 1:
        m_user->editProducts().sortByNameDes();
        break;
    case 2:
        m_user->editProducts().sortByBrandDes();
        break;
    case 3:
        m_user->editProducts().sortByCategoryDes();
        break;
    case 4:
        m_user->editProducts().sortByPriceDes();
        break;
    case 5:
        m_user->editProducts().sortByStockDes();
        break;
    case 6:
        m_user->editProducts().sortByAvailableDes();
        break;
    case 7:
        m_user->editProducts().sortByAddDateDes();
        break;
    case 8:
        m_user->editProducts().sortByExDateDes();
        break;
    }
    updateTable();
}

void inventory_widget::onTextChanged(const QString &text){
    ui->LV_brandList->clearSelection();
    ui->LV_categoryList->clearSelection();
    search(text);
}

void inventory_widget::setupGraphData() {
    // Menambah Gudang
    m_warehouseNetwork.addWarehouse("A", "Jakarta", "Gudang Utama Jakarta");
    m_warehouseNetwork.addWarehouse("B", "Bandung", "Gudang Area Bandung");
    m_warehouseNetwork.addWarehouse("C", "Semarang", "Gudang Jawa Tengah");
    m_warehouseNetwork.addWarehouse("D", "Surabaya", "Gudang Jawa Timur");
    m_warehouseNetwork.addWarehouse("E", "Transit", "Gudang Hub Transit");

    // Membuat Rute (Jarak dalam KM)
    m_warehouseNetwork.addRoute("A", "B", 150);
    m_warehouseNetwork.addRoute("A", "E", 210);
    m_warehouseNetwork.addRoute("B", "E", 130);
    m_warehouseNetwork.addRoute("E", "C", 230);
    m_warehouseNetwork.addRoute("C", "D", 310);
    m_warehouseNetwork.addRoute("B", "D", 450);
}

void inventory_widget::on_PB_showGraph_clicked() {
    // Buat dialog custom dengan style seperti Add Product
    QDialog* dialog = new QDialog(this);
    dialog->setWindowTitle("Tampilkan Graf");
    dialog->setFixedSize(500, 400);
    dialog->setStyleSheet(
        "QDialog {"
        "   background: qlineargradient(x1:0, y1:0, x2:1, y2:1,"
        "               stop:0 #667eea, stop:0.5 #764ba2, stop:1 #6B8DD6);"
        "}"
    );

    QVBoxLayout* mainLayout = new QVBoxLayout(dialog);
    mainLayout->setContentsMargins(40, 30, 40, 30);
    mainLayout->setSpacing(15);

    // Title
    QLabel* titleLabel = new QLabel("Tampilkan Graf Gudang");
    titleLabel->setStyleSheet(
        "QLabel {"
        "   color: #1a3a4a;"
        "   font-size: 22px;"
        "   font-weight: bold;"
        "   font-family: 'Segoe UI', Arial;"
        "   background: transparent;"
        "}"
    );
    mainLayout->addWidget(titleLabel);
    mainLayout->addSpacing(10);

    // Style untuk label
    QString labelStyle = 
        "QLabel {"
        "   color: #1a5a6a;"
        "   font-size: 13px;"
        "   font-weight: bold;"
        "   font-family: 'Cabin', Arial;"
        "   background: transparent;"
        "}";

    // Style untuk ComboBox
    QString comboStyle = 
        "QComboBox {"
        "   background-color: white;"
        "   border: none;"
        "   border-radius: 8px;"
        "   padding: 12px 15px;"
        "   font-size: 14px;"
        "   color: #333;"
        "}"
        "QComboBox::drop-down {"
        "   border: none;"
        "   width: 30px;"
        "}"
        "QComboBox::down-arrow {"
        "   image: none;"
        "   border-left: 5px solid transparent;"
        "   border-right: 5px solid transparent;"
        "   border-top: 6px solid #666;"
        "   margin-right: 10px;"
        "}"
        "QComboBox QAbstractItemView {"
        "   background-color: white;"
        "   color: #333;"
        "   selection-background-color: #c3d6f4;"
        "   selection-color: #333;"
        "   border: 1px solid #ccc;"
        "   border-radius: 5px;"
        "   padding: 5px;"
        "   outline: none;"
        "}"
        "QComboBox QAbstractItemView::item {"
        "   color: #333;"
        "   background-color: white;"
        "   padding: 8px 10px;"
        "   min-height: 25px;"
        "}"
        "QComboBox QAbstractItemView::item:hover {"
        "   background-color: #e8f0fc;"
        "   color: #333;"
        "}"
        "QComboBox QAbstractItemView::item:selected {"
        "   background-color: #c3d6f4;"
        "   color: #333;"
        "}";

    // Layout horizontal untuk Asal dan Tujuan
    QHBoxLayout* inputLayout = new QHBoxLayout();
    inputLayout->setSpacing(20);

    // Gudang Asal
    QVBoxLayout* asalLayout = new QVBoxLayout();
    QLabel* asalLabel = new QLabel("Gudang Asal");
    asalLabel->setStyleSheet(labelStyle);
    QComboBox* asalCombo = new QComboBox();
    asalCombo->setStyleSheet(comboStyle);
    asalCombo->setFixedHeight(45);
    
    // Gudang Tujuan
    QVBoxLayout* tujuanLayout = new QVBoxLayout();
    QLabel* tujuanLabel = new QLabel("Gudang Tujuan");
    tujuanLabel->setStyleSheet(labelStyle);
    QComboBox* tujuanCombo = new QComboBox();
    tujuanCombo->setStyleSheet(comboStyle);
    tujuanCombo->setFixedHeight(45);

    // Isi ComboBox dengan daftar gudang
    std::vector<std::string> warehouseIds = m_warehouseNetwork.getWarehouseIds();
    for (const auto& id : warehouseIds) {
        QString itemText = QString::fromStdString(id);
        asalCombo->addItem(itemText);
        tujuanCombo->addItem(itemText);
    }

    asalLayout->addWidget(asalLabel);
    asalLayout->addWidget(asalCombo);
    tujuanLayout->addWidget(tujuanLabel);
    tujuanLayout->addWidget(tujuanCombo);
    
    inputLayout->addLayout(asalLayout);
    inputLayout->addLayout(tujuanLayout);
    mainLayout->addLayout(inputLayout);

    mainLayout->addSpacing(10);

    // Tombol Tampilkan Graf
    QPushButton* showButton = new QPushButton("Tampilkan Graf");
    showButton->setFixedHeight(50);
    showButton->setCursor(Qt::PointingHandCursor);
    showButton->setStyleSheet(
        "QPushButton {"
        "   background-color: #9b8ac4;"
        "   border: none;"
        "   border-radius: 10px;"
        "   color: white;"
        "   font-size: 15px;"
        "   font-weight: bold;"
        "   font-family: 'Segoe UI', Arial;"
        "}"
        "QPushButton:hover {"
        "   background-color: #b8a8d8;"
        "}"
        "QPushButton:pressed {"
        "   background-color: #8a7ab4;"
        "}"
    );
    mainLayout->addWidget(showButton);

    // Tombol Cancel
    QPushButton* cancelButton = new QPushButton("Cancel");
    cancelButton->setFixedHeight(50);
    cancelButton->setCursor(Qt::PointingHandCursor);
    cancelButton->setStyleSheet(
        "QPushButton {"
        "   background-color: #e8a4b8;"
        "   border: none;"
        "   border-radius: 10px;"
        "   color: white;"
        "   font-size: 15px;"
        "   font-weight: bold;"
        "   font-family: 'Segoe UI', Arial;"
        "}"
        "QPushButton:hover {"
        "   background-color: #f0b8c8;"
        "}"
        "QPushButton:pressed {"
        "   background-color: #d894a8;"
        "}"
    );
    mainLayout->addWidget(cancelButton);

    mainLayout->addStretch();

    // TextEdit untuk menampilkan hasil (hidden by default)
    QTextEdit* resultText = new QTextEdit();
    resultText->setReadOnly(true);
    resultText->setVisible(false);
    resultText->setStyleSheet(
        "QTextEdit {"
        "   background-color: white;"
        "   border: none;"
        "   border-radius: 8px;"
        "   padding: 10px;"
        "   font-family: 'Consolas', monospace;"
        "   font-size: 11px;"
        "   color: #333;"
        "}"
    );
    mainLayout->addWidget(resultText);

    // Connect tombol
    connect(cancelButton, &QPushButton::clicked, dialog, &QDialog::reject);
    
    connect(showButton, &QPushButton::clicked, [=]() {
        QString startId = asalCombo->currentText();
        QString endId = tujuanCombo->currentText();
        
        // Dapatkan representasi graf berbobot dengan hasil analisis Dijkstra
        std::string graphString = m_warehouseNetwork.getWeightedGraphString(
            startId.toStdString(), 
            endId.toStdString()
        );
        
        // Tampilkan hasil
        resultText->setVisible(true);
        resultText->setText(QString::fromStdString(graphString));
        dialog->setFixedSize(600, 600);
    });

    dialog->exec();
    delete dialog;
}
