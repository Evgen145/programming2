﻿#include "airline_ticket.h"
#include "test_runner.h"

#include <algorithm>
#include <numeric>
using namespace std;

#define SORT_BY(field) [](const AirlineTicket& lhs, const AirlineTicket& rhs) { return lhs.field < rhs.field; }

bool operator<(const Date& lhs, const Date& rhs) {
    return tie(lhs.year, lhs.month, lhs.day) < tie(rhs.year, rhs.month, rhs.day);
}

bool operator==(const Date& lhs, const Date& rhs) {
    return tie(lhs.year, lhs.month, lhs.day) == tie(rhs.year, rhs.month, rhs.day);
}

ostream& operator<<(ostream& out, const Date& date) {
    return out << date.year << "-" << date.month << "-" << date.day;
}

bool operator<(const Time& lhs, const Time& rhs) {
    return tie(lhs.hours, lhs.minutes) < tie(rhs.hours, rhs.minutes);
}

bool operator==(const Time& lhs, const Time& rhs) {
    return tie(lhs.hours, lhs.minutes) == tie(rhs.hours, rhs.minutes);
}

ostream& operator<<(ostream& out, const Time& time) {
    return out << time.hours << ":" << time.minutes;
}

void TestSortBy() {
    vector<AirlineTicket> tixs = {
      {"BORYSPIL", "LVIV", "MAU", {2022, 2, 28}, {17, 40}, {2022, 2, 28}, {20,  0}, 1200},
      {"CHARKIV", "BORYSPIL", "MAU", {2022, 3,  5}, {14, 15}, {2022, 3,  5}, {16, 30}, 1700},
      {"IVFR", "CHARKIV", "BORYSPIL", {2022, 3,  5}, {18, 30}, {2022, 3,  5}, {20, 30}, 2300},
      {"LVIV", "IVFR", "Iberia", {2022, 2,  8}, {23, 00}, {2022, 2,  9}, { 3, 30}, 9000},
      {"IVFR", "Warsaw", "AirFrance", {2022, 3,  1}, {13, 00}, {2022, 3,  1}, {17, 30}, 8000},
    };

    sort(begin(tixs), end(tixs), SORT_BY(price));
    ASSERT_EQUAL(tixs.front().price, 1200);
    ASSERT_EQUAL(tixs.back().price, 9000);

    sort(begin(tixs), end(tixs), SORT_BY(from));
    ASSERT_EQUAL(tixs.front().from, "BORYSPIL");
    ASSERT_EQUAL(tixs.back().from, "LVIV");

    sort(begin(tixs), end(tixs), SORT_BY(arrival_date));
    ASSERT_EQUAL(tixs.front().arrival_date, (Date{ 2022, 2, 9 }));
    ASSERT_EQUAL(tixs.back().arrival_date, (Date{ 2022, 3, 5 }));
}

int main() {
    TestRunner tr;
    RUN_TEST(tr, TestSortBy);
}
