
#include "products.h"

Products::Products() = default;

void Products::addProduct(const Product& newItem) {
    if (!existence(newItem.getSku())) {
        m_products.emplace_back(newItem);
    }
    else {
        return;
    }
}

const Product& Products::getProduct(const std::string_view desiredSku) const{
    std::vector<Product>::const_iterator productIterator = std::find_if(m_products.cbegin(), m_products.cend(),
                                                                        [&desiredSku](const Product& product) {
        return product.getSku() == desiredSku;
    });

    return *productIterator;
}

Product& Products::editProduct(const std::string_view desiredSku) {
    std::vector<Product>::iterator productIterator = std::find_if(m_products.begin(), m_products.end(),
                                                                  [&desiredSku](const Product& product) {
        return product.getSku() == desiredSku;
    });

    return *productIterator;
}

bool Products::existence(const std::string_view desiredSku) const {
    return std::any_of(m_products.cbegin(), m_products.cend(),
                       [desiredSku](const Product& product) {
                           return product.getSku() == desiredSku;
                       });
}

void Products::sortBySKU(){
    std::sort(m_products.begin(), m_products.end(),
              [](const Product& a, const Product& b) {
                  return a.getSku() < b.getSku();
              });
}

void Products::sortBySKUdes(){
    std::sort(m_products.begin(), m_products.end(),
              [](const Product& a, const Product& b) {
                  return a.getSku() > b.getSku();
              });
}

void Products::removeProduct(std::string_view desiredSku) {
    m_products.erase(
        std::remove_if(m_products.begin(), m_products.end(),
                       [&desiredSku](const Product& product) {
                           return product.getSku() == desiredSku;
                       }),
        m_products.end());
}

const std::vector<Product>& Products::getProducts() const {
    return m_products;
}

std::vector<Product> &Products::editProducts(){
    return m_products;
}

void Products::sortByName() {
    std::sort(m_products.begin(), m_products.end(),
              [](const Product& a, const Product& b) {
                  return a.getName() < b.getName();
              });
}

void Products::sortByNameDes(){
    std::sort(m_products.begin(), m_products.end(),
              [](const Product& a, const Product& b) {
                  return a.getName() > b.getName();
              });
}

void Products::sortByCategory() {
    std::sort(m_products.begin(), m_products.end(),
              [](const Product& a, const Product& b) {
                  return a.getCategory() < b.getCategory();
              });
}

void Products::sortByCategoryDes(){
    std::sort(m_products.begin(), m_products.end(),
              [](const Product& a, const Product& b) {
                  return a.getCategory() > b.getCategory();
              });
}

void Products::sortByBrand() {
    std::sort(m_products.begin(), m_products.end(),
              [](const Product& a, const Product& b) {
                  return a.getBrand() < b.getBrand();
              });
}

void Products::sortByBrandDes(){
    std::sort(m_products.begin(), m_products.end(),
              [](const Product& a, const Product& b) {
                  return a.getBrand() > b.getBrand();
              });
}

void Products::sortByPrice(){
    std::sort(m_products.begin(), m_products.end(),
              [](const Product& a, const Product& b) {
                  return a.getPrice() < b.getPrice();
              });
}

void Products::sortByPriceDes(){
    std::sort(m_products.begin(), m_products.end(),
              [](const Product& a, const Product& b) {
                  return a.getPrice() > b.getPrice();
              });
}

void Products::sortByStock() {
    std::sort(m_products.begin(), m_products.end(),
              [](const Product& a, const Product& b) {
                  return a.getStock() < b.getStock();
              });
}

void Products::sortByStockDes(){
    std::sort(m_products.begin(), m_products.end(),
              [](const Product& a, const Product& b) {
                  return a.getStock() > b.getStock();
              });
}

void Products::sortByAvailable(){
    std::sort(m_products.begin(), m_products.end(),
              [](const Product& a, const Product& b) {
                  return a.getAvailable() < b.getAvailable();
              });
}

void Products::sortByAvailableDes(){
    std::sort(m_products.begin(), m_products.end(),
              [](const Product& a, const Product& b) {
                  return a.getAvailable() > b.getAvailable();
              });
}

void Products::sortByAddDate() {
    std::sort(m_products.begin(), m_products.end(),
              [](const Product& a, const Product& b) {
                  return a.getAddedDate() < b.getAddedDate();
              });
}

void Products::sortByAddDateDes(){
    std::sort(m_products.begin(), m_products.end(),
              [](const Product& a, const Product& b) {
                  return a.getAddedDate() > b.getAddedDate();
              });
}

void Products::sortByExDate(){
    std::sort(m_products.begin(), m_products.end(),
              [](const Product& a, const Product& b) {
                  return a.getExDate() < b.getExDate();
              });
}

void Products::sortByExDateDes(){
    std::sort(m_products.begin(), m_products.end(),
              [](const Product& a, const Product& b) {
                  return a.getExDate() > b.getExDate();
              });
}
