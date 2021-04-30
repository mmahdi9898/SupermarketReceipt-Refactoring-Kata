#include "ReceiptPrinter.h"

ReceiptPrinter::ReceiptPrinter() : ReceiptPrinter::ReceiptPrinter(40) {}

ReceiptPrinter::ReceiptPrinter(int columns) : columns(columns){}

std::string ReceiptPrinter::printReceipt(const Receipt &receipt)
{
    std::string result;
    for (const auto &item : receipt.getItems())
    {
        result.append(ReceiptPrinter::presentReceiptItem(item));
    }
    for (const auto &discount : receipt.getDiscounts())
    {
        result.append(ReceiptPrinter::presentDiscount(discount));
    }
    result.append("\n");
    result.append(ReceiptPrinter::presentTotal(receipt));
    return result;
}

std::string ReceiptPrinter::presentReceiptItem(const ReceiptItem &item) const
{
    std::string price = ReceiptPrinter::getFormattedNumberAsString(item.getTotalPrice(), 2);
    std::string name = item.getProduct().getName();

    std::string line = ReceiptPrinter::formatLineWithWhitespace(name, price);

    if (item.getQuantity() != 1)
    {
        line += "  " + ReceiptPrinter::getFormattedNumberAsString(item.getPrice(), 2) + " * " + ReceiptPrinter::presentQuantity(item) + "\n";
    }
    return line;
}

std::string ReceiptPrinter::presentDiscount(const Discount &discount) const
{
    std::string name = discount.getDescription() + "(" + discount.getProduct().getName() + ")";
    std::string pricePresentation = ReceiptPrinter::getFormattedNumberAsString(discount.getDiscountAmount(), 2);
    return ReceiptPrinter::formatLineWithWhitespace(name, pricePresentation);
}

std::string ReceiptPrinter::presentTotal(const Receipt &receipt) const
{
    std::string total = "Total: ";
    std::string pricePresentation = ReceiptPrinter::presentPrice(receipt.getTotalPrice());
    return ReceiptPrinter::formatLineWithWhitespace(total, pricePresentation);
}

std::string ReceiptPrinter::formatLineWithWhitespace(const std::string &name, const std::string &value) const
{
    int whitespaceSize = columns - name.length() - value.length();
    std::string whitespace;
    for (int i = 0; i < whitespaceSize; i++)
    {
        whitespace.append(" ");
    }
    return name + whitespace + value + "\n";
}

std::string ReceiptPrinter::presentPrice(double price) const
{ return ReceiptPrinter::getFormattedNumberAsString(price, 2); }

static std::string ReceiptPrinter::presentQuantity(const ReceiptItem &item)
{
    return ProductUnit::Each == item.getProduct().getUnit()
            ? ReceiptPrinter::getFormattedNumberAsString(item.getQuantity(), 0)
            : ReceiptPrinter::getFormattedNumberAsString(item.getQuantity(), 3);
}

static std::string ReceiptPrinter::getFormattedNumberAsString(double number, int precision)
{
    std::stringstream stream;
    stream << std::fixed << std::setprecision(precision) << number;
    return stream.str();
}