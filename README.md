Bookstore Inventory System (C++)
Overview

This project is a console-based bookstore inventory and sales management system written in C++.
It simulates a real-world retail bookstore environment by managing catalog data, processing sales records, and applying discount rules using STL containers.

The program demonstrates practical use of:

std::map for structured catalog organization

std::vector for dynamic data storage

File I/O for reading and processing external datasets

Object-oriented programming with multiple header and implementation files

Features

 Load bookstore catalog from text files

 Apply configurable discount rules

 Process weekly and monthly sales data

 Track bestselling and contemporary book data

 Organized using modular class design

Technologies Used

C++

STL (Standard Template Library)

File I/O

Object-Oriented Programming principles

File Structure
Book9.cpp / Book9.h
BookCatalog.cpp / BookCatalog.h
DiscountRule.h
SalesRecord.h
main.cpp

Data files:

contemporary-books.txt
discount-rules.txt
monthly-sales-history.txt
nonfiction-bestsellers.txt
sales-data-weekly.txt

How to Compile

Using g++:

g++ *.cpp -o bookstore
./bookstore
Learning Objectives

This project was developed as part of a Computer Science coursework assignment and demonstrates:

Efficient data organization using maps

Rule-based business logic implementation

Structured multi-file C++ application design

Parsing and processing structured data from text files

Author

James Dalton Baker
Computer Science Student