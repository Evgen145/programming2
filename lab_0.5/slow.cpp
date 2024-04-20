#include <iostream>
#include <vector>
#include <set>
#include <utility>
#include <fstream>
#include <iomanip>
#include <thread>
#include <mutex>
#include "profile.h"

using namespace std;

class ReadingManager {
public:
    ReadingManager() : page_counts_(MAX_USER_COUNT + 1, 0), heap_() {}

    void Read(int user_id, int page_count) {
        int old_count = page_counts_[user_id];
        page_counts_[user_id] = page_count;

        if (old_count == 0) {
            lock_guard<mutex> lock(mtx_);
            heap_.insert(make_pair(page_count, user_id));
        } else {
            lock_guard<mutex> lock(mtx_);
            heap_.erase(heap_.find(make_pair(old_count, user_id)));
            heap_.insert(make_pair(page_count, user_id));
        }
    }

    double Cheer(int user_id) const {
        int page_count = page_counts_[user_id];
        if (page_count == 0) {
            return 0;
        }

        lock_guard<mutex> lock(mtx_);
        int user_count = heap_.size();
        if (user_count == 1) {
            return 1;
        }

        auto it = heap_.lower_bound(make_pair(page_count, 0));
        int position = heap_.size() - distance(it, heap_.end());

        return (user_count - position) * 1.0 / (user_count - 1);
    }

private:
    static const int MAX_USER_COUNT = 100'000;

    vector<int> page_counts_;
    multiset<pair<int, int>, greater<pair<int, int>>> heap_;
    mutable mutex mtx_;
};

int main() {
    LOG_DURATION("full");

    ReadingManager manager;

    ifstream input_file("input.txt");
    int query_count;
    input_file >> query_count;

    vector<thread> threads;
    for (int thread_id = 0; thread_id < thread::hardware_concurrency(); ++thread_id) {
        threads.emplace_back([&] {
            for (int query_id = thread_id; query_id < query_count; query_id += thread::hardware_concurrency()) {
                string query_type;
                int user_id;
                input_file >> query_type >> user_id;

                if (query_type == "READ") {
                    int page_count;
                    input_file >> page_count;
                    manager.Read(user_id, page_count);
                } else if (query_type == "CHEER") {
                    cout << setprecision(6) << manager.Cheer(user_id) << "\n";
                }
            }
        });
    }

    for (auto& thread : threads) {
        thread.join();
    }

    input_file.close();

    return 0;
}