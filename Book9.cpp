#include "Book9.h"
#include <iostream>
using namespace std;

Book::Book(): author(""),title(""),year(0),isbn(""),price(0.0),quantity(0),
      category(""), discountedPrice(0.0) {}

Book::Book(string author, string title, int year, string isbn, double price, 
           int quantity, 
           string category) {
               
    this->author  =author;
    this->title   =title;
    this->year    =year;
    this->isbn    =isbn;
    this->quantity=quantity;
    this->category=category;
    this->price   =price;
    
    discountedPrice= price;
}
string Book::getISBN() const {
    return isbn;
}
void Book::setPrice(double p) {
    price=p;
}
void Book::setQuantity(int q) {
    quantity=q;
}
void Book::setCategory(const string& c) {
    category=c;
}
void Book::setDiscountedPrice(double d) {
    discountedPrice=d;
}
void Book::reduceQuantity(int amount) {
    if (amount<=0) 
        return;
    if (amount>=quantity)
        quantity=0;
    else 
        quantity-=amount;
    
}

void Book::display() const {
    cout << "Title: "<<title<<endl;
    cout << "Author: "<<author<<endl;
    cout << "Publish Year: "<<year<<endl;
    cout << "ISBN: "<<isbn<< endl;
    cout << "Category: "<<category<<endl;
    cout << "Price: $"<<price<<endl;
    cout << "Discounted Price: $"<<discountedPrice<<endl;
    cout << "Quantity: "<<quantity<<endl;
    cout << "------------------------------"<<endl;
}