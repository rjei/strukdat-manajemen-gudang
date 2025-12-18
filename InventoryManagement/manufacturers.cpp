#include "manufacturers.h"
#include <QString>
#include <QCryptographicHash>

Manufacturers::Manufacturers() = default;

Manufacturers::~Manufacturers(){
    for (Seller* seller : m_manufacturers){
        delete seller;
    }
}

void Manufacturers::addManufact(Seller* seller){
    m_manufacturers.push_back(seller);
}

bool Manufacturers::usernameExistence(const std::string_view username) const{
    return std::any_of(m_manufacturers.cbegin(), m_manufacturers.cend(),
                       [username](const Seller* seller){
                           return seller->getUsername() == username;
                       });
}

bool Manufacturers::NINexistence(const std::string_view NIN) const{
    return std::any_of(m_manufacturers.cbegin(), m_manufacturers.cend(),
                       [NIN](const Seller* seller){
                           return seller->getNIN() == NIN;
                       });
}

bool Manufacturers::MIDexistence(const std::string_view MID) const{
    return std::any_of(m_manufacturers.cbegin(), m_manufacturers.cend(),
                       [MID](const Seller* seller){
                           return seller->getMID() == MID;
                       });
}

bool Manufacturers::phoneNumberExistence(const std::string_view phoneNumber) const{
    return std::any_of(m_manufacturers.cbegin(), m_manufacturers.cend(),
                       [phoneNumber](const Seller* seller){
                           return seller->getUsername() == phoneNumber;
                       });
}

bool Manufacturers::checkPassword(std::string_view username, std::string_view password) const{

    QByteArray enteredPasswordBytes = QString::fromStdString(std::string(password)).toUtf8();
    QByteArray hashedEnteredPassword = QCryptographicHash::hash(enteredPasswordBytes, QCryptographicHash::Sha256).toHex();

    if (QString::fromStdString(getManufacturerByUsername(username)->getPassword()) == hashedEnteredPassword)
        return true;
    else
        return false;
}

const Seller &Manufacturers::getSellerByMID(const std::string_view MID) const{
    for (const Seller* seller : m_manufacturers){
        if (seller->getMID() == MID)
            return *seller;
    }
    throw std::runtime_error("Seller not found.");
}

const Seller* Manufacturers::getManufacturerByUsername(const std::string_view username) const{
    for (Seller* seller: m_manufacturers){
        if (seller->getUsername() == username)
            return seller;
    }
    throw std::runtime_error("Seller not found.");
}

Seller* Manufacturers::editSeller(const std::string_view username){
    for (Seller* seller: m_manufacturers){
        if (seller->getUsername() == username)
            return seller;
    }
    throw std::runtime_error("Seller not found.");

}

Seller* Manufacturers::editSellerByMID(const std::string_view MID){
    for (Seller* seller : m_manufacturers){
        if (seller->getMID() == MID){
            return seller;
        }
    }
    throw std::runtime_error("Seller not found.");
}

Seller *Manufacturers::editSellerByManuName(const std::string_view manufacturerName){
    for (Seller* seller: m_manufacturers){
        if (seller->getManufactureName() == manufacturerName)
            return seller;
    }
    throw std::runtime_error("Seller not found.");
}

const std::vector<Seller *> Manufacturers::getManufacturers() const{
    return m_manufacturers;
}

std::vector<Seller *> Manufacturers::editManufacturers(){
    return m_manufacturers;
}
