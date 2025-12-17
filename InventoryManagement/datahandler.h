#ifndef DATAHANDLER_H
#define DATAHANDLER_H

#include <string>
#include <sstream>
#include <fstream>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>
#include <QCryptographicHash>

// Forward declarations
class Manufacturers;
class Seller;
class Product;

class DataHandler
{
public:
    DataHandler();

    void removeQuotes(std::string& value);
    
    // SQLite
    void openDatabase(QSqlDatabase& data);

    void closeDataBase(QSqlDatabase* data);

    void readDataLogin(Manufacturers* manufacturers);

    void readDataHomeWindow(Manufacturers* manufacturers, Seller* user);

    void readProducts(Seller* user);

    void addProduct(Product* newProduct, const std::string& MID);

    void updateProduct(const std::string& productSKU, const Product& newProduct, const std::string& MID);

    void removeProduct(const std::string& productSKU, const std::string ownerMID);
};

#endif // DATAHANDLER_H
