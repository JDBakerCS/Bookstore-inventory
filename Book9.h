#pragma once
#include <string>
using namespace std;

class Book {
private: 
    string author;
    string title; 
    int year;
    string isbn;
    double price;
    int quantity;
    string category;
    double discountedPrice;
    
public:
    Book(); 
    Book(string author, string title, int year, string isbn, double price, int quantity, string category);
    
    string getISBN() const;
    string getTitle() const {return title;}
    string getAuthor() const {return author;}
    int    getYear() const {return year;}
    
    double getPrice() const {return price;}
    int    getQuantity() const {return quantity;}
    string getCategory() const {return category;}
    double getDiscountedPrice() const {return discountedPrice;}
    
    void setPrice(double p);
    void setQuantity(int q);
    void setCategory(const string& c);
    void setDiscountedPrice(double d);
    
    void reduceQuantity(int amount);
    
    
    void display() const;
};



