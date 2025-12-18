#ifndef INVENTORY_WIDGET_H
#define INVENTORY_WIDGET_H

#include <QWidget>
#include <QStringList>
#include <QInputDialog>
#include <QMessageBox>
#include <vector>
#include <string>
#include "../warehouse_graph.h" //

class Manufacturers;
class Seller;

namespace Ui {
class inventory_widget;
}

class inventory_widget : public QWidget
{
    Q_OBJECT

public:
    explicit inventory_widget(Manufacturers* manufacturers, Seller *user, QWidget *parent = nullptr);
    ~inventory_widget();

private slots:
    void on_PB_remove_clicked();
    void on_PB_showGraph_clicked(); // Slot untuk tombol baru Anda

private:
    Ui::inventory_widget *ui;
    Manufacturers* m_manufacturers;
    Seller* m_user;

    WarehouseGraph warehouseNetwork; //
    void setupGraphData();
    void populateTable();
};

#endif
