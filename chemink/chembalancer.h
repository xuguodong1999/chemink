#pragma once

#include <string>

class ChemBalancer {

public:
    ChemBalancer() = default;

    ~ChemBalancer() = default;

    std::string addHtml(const std::string &_input);

    std::string balance(const std::string &_input);
};
