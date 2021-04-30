#include "ShoppingCart.h"

void addItemQuantity(const Product& product, double quantity);

std::vector<ProductQuantity> ShoppingCart::getItems() const {
    return items;
}

std::map<Product, double> ShoppingCart::getProductQuantities() const {
    return productQuantities;
}

void ShoppingCart::addItem(const Product& product) {
    addItemQuantity(product, 1.0);
}

void ShoppingCart::addItemQuantity(const Product& product, double quantity) {
    items.emplace_back(product, quantity);
    if (productQuantities.find(product) != productQuantities.end()) {
        productQuantities[product] += quantity;
    } else {
        productQuantities[product] = quantity;
    }
}

void ShoppingCart::handleOffers(Receipt& receipt, std::map<Product, Offer> offers, SupermarketCatalog* catalog) {
    for (const auto& productQuantity : productQuantities) {
        Product product = productQuantity.first;
        double quantity = productQuantity.second;
        if (offers.find(product) != offers.end()) {
            auto offer = offers[product];
            double unitPrice = catalog->getUnitPrice(product);
            int quantityAsInt = (int) quantity;
            Discount* discount = nullptr;
            int offerCount = 1;

            if (offer.getOfferType() == SpecialOfferType::Bundles){ // product is included in bundle offer
                bool isBundle = true; // flag to indentify bundles
                std::vector<int> localCountOffer; // keep track of the local offer counts
                for(const auto& p : offer.getProducts()){ // for every product in the bundle offer
                    if(productQuantities.find(product) != productQuantities.end()){ // product found in quantities
                        double limit = offers.getArguments()[offer.getProducts().find(product) - offer.getProducts().begin() + offers.getArguments().begin()]; // argument of the product
                        if(productQuantities[product] >= limit){
                            localCountOffer.push_back(int(productQuantities[product] / limit));
                        }
                        else{
                            isBundle = false;
                            break; // amount not reached the minimum limit
                        }
                    }
                    else{
                        isBundle = false;
                        break; // element not found
                    }
                }
                if(isBundle == true){ // a suitable bundle found
                    int offerWhole = std::min_element(localCountOffer.begin(), localCountOffer.end()); // number of wholes in bundle
                    double limit = offers.getArguments()[offer.getProducts().find(product) - offer.getProducts().begin() + offers.getArguments().begin()]; // argument of the product
                    double percentage = offers.getArguments().back(); // get the discount percentage
                    double discountAmount = quantity - (percentage*offerWhole*limit + quantityAsInt % (offerWhole*limit)); // difference
                    discount = new Discount("Bundles", -1 * unitPrice *discountAmount, product); // create discount
                }
            }
            if (offer.getOfferType() == SpecialOfferType::ThreeForTwo) {
                offerCount = 3;
            } else if (offer.getOfferType() == SpecialOfferType::TwoForAmount) {
                offerCount = 2;
                if (quantityAsInt >= 2) {
                    double total = offer.getArguments().front() * (quantityAsInt / offerCount) + quantityAsInt % 2 * unitPrice;
                    double discountN = unitPrice * quantity - total;
                    discount = new Discount("2 for " + std::to_string(offer.getArguments().front()), -discountN, product);
                }
            } if (offer.getOfferType() == SpecialOfferType::FiveForAmount) {
                offerCount = 5;
            }
            int offerWhole = quantityAsInt / offerCount;
            if (offer.getOfferType() == SpecialOfferType::ThreeForTwo && quantityAsInt > 2) {
                double discountAmount = quantity * unitPrice - ((offerWhole * 2 * unitPrice) + quantityAsInt % 3 * unitPrice);
                discount = new Discount("3 for 2", -discountAmount, product);
            }
            if (offer.getOfferType() == SpecialOfferType::TenPercentDiscount) {
                discount = new Discount(std::to_string(offer.getArguments().front()) + "% off", -quantity * unitPrice * offer.getArguments().front() / 100.0, product);
            }
            if (offer.getOfferType() == SpecialOfferType::FiveForAmount && quantityAsInt >= 5) {
                double discountTotal = unitPrice * quantity - (offer.getArguments().front() * offerWhole + quantityAsInt % 5 * unitPrice);
                discount = new Discount(std::to_string(offerCount) + " for " + std::to_string(offer.getArguments().front()), -discountTotal, product);
            }
            if (discount != nullptr)
                receipt.addDiscount(*discount);
        }
    }
}