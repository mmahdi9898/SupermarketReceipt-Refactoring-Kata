#ifndef CPP_OFFER_H
#define CPP_OFFER_H

#include <vector>
#include "Product.h"
#include "SpecialOfferType.h"

class Offer {
public:
	
    Offer(const SpecialOfferType& offerType, const std::vector<Product>& product, std::vector<double> argument);

    SpecialOfferType getOfferType() const;

    std::vector<Product> getProducts() const;

    std::vector<double> getArguments() const;

private:
    SpecialOfferType offerType;
    std::vector<Product> products;
    std::vector<double> arguments;

};

#endif //CPP_OFFER_H
