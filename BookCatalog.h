#pragma once

#include <map>
#include <string>
#include <vector>
#include "Book9.h"
#include "SalesRecord.h"
#include "DiscountRule.h"

using std::map;
using std::string;
using std::vector;

// Loading / printing 

void loadBooksFromCSV(const string& filename,
                      const string& category,
                      map<string, Book>& catalog);

void loadAllBookFiles(map<string, Book>& catalog);

void printCatalog(const map<string, Book>& catalog);

// Sales + discount file loaders

vector<SalesRecord> loadWeeklySales(const string& filename);

void loadMonthlySalesHistory(const string& filename,
                             map<string, int>& unitsMap,
                             map<string, double>& revenueMap);

vector<DiscountRule> loadDiscountRules(const string& filename);

//  Price-based analytics 

Book getMostExpensiveBook(const map<string, Book>& catalog);
Book getLeastExpensiveBook(const map<string, Book>& catalog);

// Sales-based analytics

Book getBestSeller(const map<string, Book>& catalog,
                   const map<string, int>& monthlyUnitsSold);

Book getSlowestSeller(const map<string, Book>& catalog,
                      const map<string, int>& monthlyUnitsSold);
                      


// total inventory value (price * quantity)
double totalInventoryValue(const map<string, Book>& catalog);

// count how many books have quantity < 10
int countLowStock(const map<string, Book>& catalog);

// get a vector of low-stock books (< 10 units)
vector<Book> getLowStockBooks(const map<string, Book>& catalog);

// search helpers
Book* findBookByISBN(map<string, Book>& catalog, const string& isbn);
vector<Book> booksByAuthor(const map<string, Book>& catalog, const string& author);

// sorting helpers
vector<Book> sortByPrice(const map<string, Book>& catalog);
vector<Book> sortByTitle(const map<string, Book>& catalog);
vector<Book> sortByYear(const map<string, Book>& catalog);

// apply weekly sales to catalog quantities
void applyWeeklySales(map<string, Book>& catalog,
                      const vector<SalesRecord>& weeklySales);

// apply discount rules (returns discounted copy of catalog)
vector<Book> applyDiscounts(const map<string, Book>& catalog,
                            const map<string, int>& monthlyUnitsSold,
                            const vector<DiscountRule>& rules);

