#ifndef HOME_WINDOW_H
#define HOME_WINDOW_H

#include <QDialog>
#include <QPushButton>
#include <QWidget>

// Forward declarations agar compiler mengenali tipe data ini
class Manufacturers;
class Seller;

namespace Ui {
class home_window;
}

class home_window : public QDialog
{
    Q_OBJECT

public:
    // Pastikan argumen constructor cocok dengan yang dipanggil di main.cpp
    explicit home_window(Manufacturers* manufacturers, Seller* user, QWidget *parent = nullptr);
    ~home_window();

signals:
    void dialogClosed();

private slots:
    // Fungsi bantuan untuk styling tombol
    void setPushButtonDefaultStyle(QPushButton* button, const QString& iconPath);
    void setPushButtonSelectedtStyle(QPushButton* button, const QString& iconPath);
    void pushButtonsDefaultStyle();

    // Fungsi untuk manajemen widget
    void closeWidgets();
    void closeChildWidget(QWidget* parent, const QString& widgetName);

    // Slot navigasi (Hanya Inventory yang tersisa)
    void on_PB_inventory_clicked();

    // Setup label user
    void setLabels();

private:
    Ui::home_window *ui;
    Manufacturers* m_manufacturers;
    Seller* m_user;
};

#endif // HOME_WINDOW_H
