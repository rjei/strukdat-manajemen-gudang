
#include "seller.h"

Seller::Seller() = default;

void Seller::setMID(const std::string_view MID) {
    m_MID = MID;
}

void Seller::setManufactureName(const std::string_view manufactureName) {
    m_manufactureName = manufactureName;
}

void Seller::addProduct(const Product& newProduct){
    m_products.addProduct(newProduct);
}

void Seller::removerProduct(const std::string_view targetSKU){
    m_products.removeProduct(targetSKU);
}

void Seller::editProduct(const std::string_view targetSKU, std::string_view newName, std::string_view newCategory
                         , std::string_view newSKU, std::string_view newBrand, double newStock
                         , double newAvailable, double newPrice, std::string_view newUnit, std::string_view newDescription
                         , std::string_view newAddDate, std::string_view newExDate, bool newAvailability){
    Product& product = m_products.editProduct(targetSKU);
    product.setName(newName);
    product.setCategory(newCategory);
    product.setSku(newSKU);
    product.setBrand(newBrand);
    product.setStock(newStock);
    product.setAvailable(newAvailable);
    product.setPrice(newPrice);
    product.setUnit(newUnit);
    product.setDescription(newDescription);
    product.setAddedDate(newAddDate);
    product.setExDate(newExDate);
    product.setAvailability(newAvailability);
}

const std::string& Seller::getMID() const{
    return m_MID;
}

const std::string& Seller::getManufactureName() const{
    return m_manufactureName;
}

const Products& Seller::getProductsModel() const{
    return m_products;
}

Products& Seller::editProducts() {
    return m_products;
}
