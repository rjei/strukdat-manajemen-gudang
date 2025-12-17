
#include "datahandler.h"
#include "currency.h"
#include "manufacturers.h"
#include "seller.h"

DataHandler::DataHandler() = default;

void DataHandler::removeQuotes(std::string& value) {
    if (value.size() >= 2 && value.front() == '"' && value.back() == '"') {
        value = value.substr(1, value.size() - 2);
    }
}

void DataHandler::openDatabase(QSqlDatabase & data){
    data = QSqlDatabase::addDatabase("QSQLITE");
    data.setDatabaseName("dataBase.db");

    if (!data.open())
        qWarning() << "Couldn't open database!";
}

void DataHandler::closeDataBase(QSqlDatabase *data){
    data->close();
    delete data;
}

void DataHandler::readDataLogin(Manufacturers* manufacturers){
    QSqlDatabase data;
    openDatabase(data);

    QSqlQuery sellersQuery;
    sellersQuery.prepare("SELECT * FROM sellers");

    if (!sellersQuery.exec())
        qDebug() << "Query failed!";

    while (sellersQuery.next()){
        QString NIN = sellersQuery.value("nin").toString();

        QSqlQuery userQuery;
        userQuery.prepare("SELECT * FROM users WHERE nin = " + NIN);

        if (!userQuery.exec())
            qDebug() << "usersQuery failed!";

        userQuery.next();
        Seller* newSeller = new Seller;

        std::string username =userQuery.value("username").toString().toStdString();
        newSeller->setUsername(username);

        std::string password = userQuery.value("password").toString().toStdString();
        newSeller->setPassword(password);

        std::string name = userQuery.value("name").toString().toStdString();
        newSeller->setName(name);

        std::string lastName = userQuery.value("last_name").toString().toStdString();
        newSeller->setLastName(lastName);

        std::string nin = NIN.toStdString();
        newSeller->setNIN(nin);

        std::string phoneNumber = userQuery.value("phone_number").toString().toStdString();
        newSeller->setPhoneNumber(phoneNumber);

        std::string manufacturerName = sellersQuery.value("manufacturer_name").toString().toStdString();
        newSeller->setManufactureName(manufacturerName);

        std::string MID = sellersQuery.value("MID").toString().toStdString();
        newSeller->setMID(MID);

        manufacturers->addManufact(newSeller);
    }

    data.close();
}

void DataHandler::readDataHomeWindow(Manufacturers *manufacturers, Seller *user){
    Currency::currentCurrency = "USD";
    Currency::currencySymbol = "$";
    readProducts(user);
}

void DataHandler::readProducts(Seller *user){
    QSqlDatabase data;
    openDatabase(data);

    QSqlQuery query;

    QString MID = '"' + QString::fromStdString(user->getMID()) + '"';

    query.prepare("SELECT * FROM products WHERE owner_MID = " + MID);

    if (!query.exec())
        qDebug() << "Query for product failed!";

    while (query.next()){

        std::string name = query.value("name").toString().toStdString();
        std::string category = query.value("category").toString().toStdString();
        std::string SKU = query.value("sku").toString().toStdString();
        std::string brand = query.value("brand").toString().toStdString();
        double stock = query.value("stock_quantity").toDouble();
        double available = query.value("available_quantity").toDouble();
        double price = query.value("price").toDouble();
        std::string unit = query.value("unit").toString().toStdString();
        std::string description = query.value("description").toString().toStdString();
        std::string addDate = query.value("added_date").toString().toStdString();
        std::string exDate = query.value("ex_date").toString().toStdString();

        bool availability = ([available](){ return available > 0; })();

        Product newProduct(name, category, SKU, brand, stock, available, price, unit, description, addDate, exDate, availability);
        user->addProduct(newProduct);
    }

    data.close();
}

void DataHandler::addProduct(Product *newProduct, const std::string &MID) {
    QSqlDatabase data;
    openDatabase(data);

    QString ownerMID = QString::fromStdString(MID);
    QString name = QString::fromStdString(newProduct->getName());
    QString category = QString::fromStdString(newProduct->getCategory());
    QString sku = QString::fromStdString(newProduct->getSku());
    QString brand = QString::fromStdString(newProduct->getBrand());
    double stockQuantity = newProduct->getStock();
    double availableQuantity = newProduct->getAvailable();
    double price = newProduct->getPrice();
    QString unit = QString::fromStdString(newProduct->getUnit());
    QString description = QString::fromStdString(newProduct->getDescription());
    QString addedDate = QString::fromStdString(newProduct->getAddedDate());
    QString expiryDate = QString::fromStdString(newProduct->getExDate());

    QSqlQuery query;
    query.prepare("INSERT INTO products (owner_MID, name, category, sku, brand, stock_quantity, "
                  "available_quantity, price, unit, description, added_date, ex_date) "
                  "VALUES (:owner_MID, :name, :category, :sku, :brand, :stock_quantity, "
                  ":available_quantity, :price, :unit, :description, :added_date, :ex_date)");
    query.bindValue(":owner_MID", ownerMID);
    query.bindValue(":name", name);
    query.bindValue(":category", category);
    query.bindValue(":sku", sku);
    query.bindValue(":brand", brand);
    query.bindValue(":stock_quantity", stockQuantity);
    query.bindValue(":available_quantity", availableQuantity);
    query.bindValue(":price", price);
    query.bindValue(":unit", unit);
    query.bindValue(":description", description);
    query.bindValue(":added_date", addedDate);
    query.bindValue(":ex_date", expiryDate);

    if (!query.exec()) {
        qDebug() << "Failed to insert product into the database";
        return;
    }

    qDebug() << "Product inserted successfully";

    data.close();
}

void DataHandler::updateProduct(const std::string& productSKU, const Product &newProduct, const std::string& MID){
    QSqlDatabase data;
    openDatabase(data);

    const QString newName = QString::fromStdString(newProduct.getName());
    const QString newCategory = QString::fromStdString(newProduct.getCategory());
    const QString newSKU = QString::fromStdString(newProduct.getSku());
    const QString newBrand = QString::fromStdString(newProduct.getBrand());
    const double newStock = newProduct.getStock();
    const double newAvailable = newProduct.getAvailable();
    const double newPrice = newProduct.getPrice();
    const QString newUnit = QString::fromStdString(newProduct.getUnit());
    const QString newDescription = QString::fromStdString(newProduct.getDescription());
    const QString newAddDate = QString::fromStdString(newProduct.getAddedDate());
    const QString newExDate = QString::fromStdString(newProduct.getExDate());

    QSqlQuery query;
    query.prepare("UPDATE products SET name = :newName, category = :newCategory, sku = :newSKU, brand = :newBrand "
                  ", stock_quantity = :newStock, available_quantity = :newAvailable, price = :newPrice, unit = :newUnit "
                  ", description = :newDescription, added_date = :newAddDate, ex_date = :newExDate "
                  "WHERE owner_MID = :ownerMID AND sku = :sku");
    query.bindValue(":newName",newName);
    query.bindValue(":newCategory", newCategory);
    query.bindValue(":newSKU", newSKU);
    query.bindValue(":newBrand", newBrand);
    query.bindValue(":newStock", newStock);
    query.bindValue(":newAvailable", newAvailable);
    query.bindValue(":newPrice", newPrice);
    query.bindValue(":newUnit", newUnit);
    query.bindValue(":newDescription", newDescription);
    query.bindValue(":newAddDate", newAddDate);
    query.bindValue(":newExDate", newExDate);
    query.bindValue(":ownerMID",QString::fromStdString(MID));
    query.bindValue(":sku",QString::fromStdString(productSKU));

    if (!query.exec()) {
        qDebug() << "Product update Failed for database";
    }

    data.close();
}

void DataHandler::removeProduct(const std::string &productSKU, const std::string ownerMID){
    QSqlDatabase data;
    openDatabase(data);

    QSqlQuery query;
    QString SKU = '"' + QString::fromStdString(productSKU) + '"';
    QString MID = '"' + QString::fromStdString(ownerMID) + '"';
    query.prepare("DELETE FROM products WHERE owner_MID = " + MID + " AND sku = " + SKU);

    if (!query.exec())
        qDebug() << "Failed to delete product from database";

    data.close();
}
