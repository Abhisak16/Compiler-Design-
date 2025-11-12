#include <iostream>
#include <fstream>
#include <regex>
#include <string>
#include <vector>

struct StringIdentifier
{
    std::string key;
    std::string value;
};

std::vector<StringIdentifier> extractStringIdentifiers(const std::string& filename) {
    std::ifstream file(filename);
    std::string line;
    std::regex stringPattern(R"((\w+)\s*=\s*\"([^\"]+)\")");
    std::vector<StringIdentifier> results;

    while (std::getline(file, line)) {
        std::smatch match;
        if (std::regex_search(line, match, stringPattern)) {
            results.push_back({match[1], match[2]});
        }
    }

    file.close();
    return results;
}

int main()
{
    std::vector<StringIdentifier> identifiers = extractStringIdentifiers("Sample.txt");

    for (const auto& item : identifiers) {
        std::cout << "Identifier: " << item.key << ", Value: " << item.value << '\n';
    }

    return 0;
}
