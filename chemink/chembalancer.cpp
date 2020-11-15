#include "chembalancer.h"
#include "balance/equation.hpp"
#include "balance/balance_util.hpp"

using namespace std;

static Equation equation;

std::string ChemBalancer::balance(std::string &input) {
    equation.mInput = input;
    if (!equation.exec()) {
        equation.clear();
        return "[ERROR]";
    }
    std::string result = equation.p();
    equation.clear();
    return result;
}

std::string ChemBalancer::addHtml(std::string input) {
    return add_html(input);
}
