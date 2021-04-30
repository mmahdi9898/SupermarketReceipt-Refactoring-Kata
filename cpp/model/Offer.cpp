#include "Offer.h"

Offer::Offer(const SpecialOfferType& offerType, const std::vector<Product>& product, std::vector<double> argument)
        : offerType(offerType), products(products), arguments(arguments) {}

SpecialOfferType Offer::getOfferType() const {
        return offerType;
}

std::vector<Product> Offer::getProducts() const {
        return products;
}

std::vector<double> Offer::getArguments() const {
        return arguments;
}
