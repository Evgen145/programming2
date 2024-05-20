#include <iostream>
#include <vector>
#include <algorithm>

int main() {
    int n;
    std::cin >> n;

    // Vector of pairs to store player numbers and their before-players
    std::vector<std::pair<int, int>> players;

    // Read player numbers and the players before whom they should stand
    for (int i = 0; i < n; ++i) {
        int current, before;
        std::cin >> current >> before;
        players.emplace_back(current, before);
    }

    // Sort the players vector based on the second element of each pair
    std::sort(players.begin(), players.end(), [](const auto& a, const auto& b) {
        return a.second < b.second;
    });

    // Output the players' numbers in the order they exit onto the field
    for (const auto& player : players) {
        std::cout << player.first << std::endl;
    }

    return 0;
}
