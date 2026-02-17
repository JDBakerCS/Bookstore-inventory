#pragma once
#include <string>

using namespace std;

struct DiscountRule {
    string tier;         // e.g., "CLEARANCE", "SLOWMOVER"
    int minSales;        // min monthly units
    int maxSales;        // max monthly units
    double percent;      // discount percentage (e.g., 30.0 for 30%)
    string description;  // human-readable description

    DiscountRule()
        : tier(""), minSales(0), maxSales(0), percent(0.0), description("") {}

    DiscountRule(const string& t, int minS, int maxS,
                 double p, const string& desc)
        : tier(t), minSales(minS), maxSales(maxS),
          percent(p), description(desc) {}
};
