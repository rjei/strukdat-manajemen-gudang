#include "home_window.h"
#include "ui_home_window.h"
#include "inventory_widget.h"
#include "../datahandler.h"
#include "../seller.h"

// Include untuk halaman lain dihapus karena tidak lagi digunakan

home_window::home_window(Manufacturers* manufacturers, Seller *user, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::home_window)
{
    ui->setupUi(this);

    this->setWindowTitle("Inventory Management");

    // Jika data handler diperlukan untuk refresh data saat window dibuat
    DataHandler data;
    data.readDataHomeWindow(manufacturers, user);

    // Hubungkan sinyal finished dialog ke sinyal dialogClosed
    connect(this, &QDialog::finished, this, &home_window::dialogClosed);

    m_manufacturers = manufacturers;
    m_user = user;
    setLabels();

    // LANGSUNG BUKA INVENTORY SAAT APLIKASI JALAN
    on_PB_inventory_clicked();
}

home_window::~home_window()
{
    delete ui;
}

void home_window::setPushButtonDefaultStyle(QPushButton* button, const QString& iconPath){
    button->setStyleSheet(
        "QPushButton {"
        "    background-image: url(" + iconPath + ");"
                     "    background-position: center;"
                     "    background-repeat: no-repeat;"
                     "    background-size: contain;"
                     "    display: inline-block;"
                     "    border-radius: 0px;"
                     "    cursor: pointer;"
                     "    color: #ffffff;"
                     "    font-family: Arial;"
                     "    font-size: 12px;"
                     "    padding: 50px 10px;"
                     "}"
                     "QPushButton:hover {"
                     "    background-color: #ecebfb;"
                     "    color: #000000;"
                     "}"
                     "QPushButton:pressed {"
                     "    background-color: #DCD7C9;"
                     "    color: #000000;"
                     "}"
                     "QPushButton:active {"
                     "    position: relative;"
                     "    top: 1px;"
                     "}"
        );
}

void home_window::setPushButtonSelectedtStyle(QPushButton* button, const QString& iconPath){
    button->setStyleSheet(
        "QPushButton {"
        "    background-image: url("+ iconPath +");"
                     "    background-color: #ecebfb;"
                     "    background-position: center;"
                     "    background-repeat: no-repeat;"
                     "    background-size: contain;"
                     "    display: inline-block;"
                     "    border-radius: 0px;"
                     "    cursor: pointer;"
                     "    color: #ffffff;"
                     "    font-family: Arial;"
                     "    font-size: 12px;"
                     "    padding: 50px 10px;"
                     "}"
        );
}

void home_window::pushButtonsDefaultStyle(){
    // Hanya atur style inventory, karena tombol lain sudah dihapus dari UI/Logic
    setPushButtonDefaultStyle(ui->PB_inventory, ":/Images/icons/inventoryIcon.png");
}

void home_window::closeWidgets(){
    closeChildWidget(ui->mainWidget, "inventory_widget");
}

void home_window::closeChildWidget(QWidget* parent, const QString& widgetName){
    QWidget* childWidget = parent->findChild<QWidget*>(widgetName);
    if (childWidget){
        childWidget->deleteLater();
    }
}

void home_window::on_PB_inventory_clicked(){
    pushButtonsDefaultStyle();
    closeWidgets();

    setPushButtonSelectedtStyle(ui->PB_inventory, ":/Images/icons/inventoryIcon.png");

    inventory_widget* inventoryWidget = new inventory_widget(m_manufacturers, m_user, ui->mainWidget);
    // Penting: Set object name agar closeChildWidget bisa menemukannya nanti
    inventoryWidget->setObjectName("inventory_widget");
    inventoryWidget->show();
}

void home_window::setLabels(){
    if (m_user) {
        QString userHeader = QString::fromStdString(m_user->getName()) + " " + QString::fromStdString(m_user->getLastName());
        ui->LB_userHeader->setText(userHeader);
    }
}
