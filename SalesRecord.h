#pragma once
#include <string>
using namespace std;

struct SalesRecord {
    string date;
    string isbn;
    int quantitySold;

    SalesRecord() : date(""), isbn(""), quantitySold(0) {}
    SalesRecord(string d, string i, int q)
        : date(d), isbn(i), quantitySold(q) {}
};
