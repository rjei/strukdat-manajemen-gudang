#ifndef HOME_WINDOW_H
#define HOME_WINDOW_H

#include <QDialog>
#include <QPushButton>
#include <QWidget>
#include <QMessageBox>
#include <QInputDialog>  // Tambahkan ini untuk meminta input
#include <QStringList>   // Tambahkan ini untuk pilihan dropdown
#include "../warehouse_graph.h"

class Manufacturers;
class Seller;

namespace Ui {
class home_window;
}

class home_window : public QDialog
{
    Q_OBJECT

public:
    explicit home_window(Manufacturers* manufacturers, Seller *user, QWidget *parent = nullptr);
    ~home_window();

signals:
    void dialogClosed();

private slots:
    void setPushButtonDefaultStyle(QPushButton* button, const QString& iconPath);
    void setPushButtonSelectedtStyle(QPushButton* button, const QString& iconPath);
    void pushButtonsDefaultStyle();
    void closeWidgets();
    void closeChildWidget(QWidget* parent, const QString& widgetName);
    void on_PB_inventory_clicked();
    void setLabels();

private:
    Ui::home_window *ui;
    Manufacturers* m_manufacturers;
    Seller* m_user;

    WarehouseGraph network;
    void setupGraphData();
};

#endif
