#pragma once

#include <string>

class ChemBalancer {

public:
    ChemBalancer() = default;

    ~ChemBalancer() = default;

    std::string addHtml(std::string input);

    std::string balance(std::string &input);
};
