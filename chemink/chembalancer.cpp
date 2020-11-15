#include "chembalancer.h"
#include "balance/equation.hpp"
#include "balance/balance_util.hpp"

using namespace std;

static Equation equation;

std::string ChemBalancer::balance(const std::string &_input) {
    equation.mInput = _input;
    if (!equation.exec()) {
        equation.clear();
        return "[ERROR]";
    }
    std::string result = equation.p();
    equation.clear();
    return result;
}

std::string ChemBalancer::addHtml(const std::string& _input) {
    return add_html(_input);
}
