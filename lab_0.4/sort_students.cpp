#include "student.h"
#include "test_runner.h"
#include "profile.h"
#include <algorithm>

using namespace std;

// Неоптимізована функція Compare
bool Compare(Student first, Student second) {
    return first.Less(second);
}

void TestComparison() {
    Student newbie {
        "Ivan", "Ivaniv", {
            {"C++", 1.0},
            {"Algorithms", 3.0}
        },
        2.0
    };

    Student cpp_expert {
        "Petro", "Petriv", {
            {"C++", 9.5},
            {"Algorithms", 6.0}
        },
        7.75
    };

    Student guru {
        "Stepan", "Stepaniv", {
            {"C++", 10.0},
            {"Algorithms", 10.0}
        },
        10.0
    };
    ASSERT(Compare(guru, newbie));
    ASSERT(Compare(guru, cpp_expert));
    ASSERT(!Compare(newbie, cpp_expert));
}

void TestSorting() {
    vector<Student> students {
        {"Stepan", "Stepaniv", {{"Maths", 2.}}, 2.},
        {"Semen", "Semeniv", {{"Maths", 4.}}, 4.},
        {"Ivan", "Ivaniv", {{"Maths", 5.}}, 5.},
        {"Petro", "Petriv", {{"Maths", 3.}}, 3.},
        {"Oleksandr", "Oleksandrenko", {{"Maths", 1.}}, 1.}
    };

    sort(students.begin(), students.end(), Compare);
    ASSERT(is_sorted(students.begin(), students.end(),
        [](Student first, Student second) {
            return first.Less(second);
        })
    );
}

int main() {
    TestRunner tr;
    RUN_TEST(tr, TestComparison);

    vector<Student> students(25000); // Створюємо велику кількість студентів
    // Наповнюємо вектор студентами
    for (int i = 0; i < 25000; ++i) {
        students[i] = {
            "Name" + to_string(i), 
            "Surname" + to_string(i),
            {{"Maths", 1.0 * i}},
            1.0 * i
        };
    }

    {
        LOG_DURATION("Sort students (unoptimized)");
        sort(students.begin(), students.end(), Compare);
    }

    RUN_TEST(tr, TestSorting);
    return 0;
}