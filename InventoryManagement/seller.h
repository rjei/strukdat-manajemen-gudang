#ifndef SELLER_H
#define SELLER_H

#include "user.h"
#include "products.h"

class Seller : public User{
public:
    Seller();

    void addProduct(const Product& newProduct);
    void removerProduct(const std::string_view targetSKU);
    void editProduct(const std::string_view targetSKU, std::string_view newName, std::string_view newCategory
                     , std::string_view newSKU, std::string_view newBrand, double newStock
                     , double newAvailable, double newPrice, std::string_view newUnit, std::string_view newDescription
                     , std::string_view newAddDate, std::string_view newExDate, bool newAvailability);

    void setMID(const std::string_view MID);
    void setManufactureName(const std::string_view manufactureName);

    const std::string& getMID() const;
    const std::string& getManufactureName()const;

    const Products& getProductsModel() const;
    Products& editProducts();

private:
    std::string m_MID;
    std::string m_manufactureName;
    Products m_products;
};

#endif // SELLER_H
