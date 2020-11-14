#ifndef FUNCTION_H
#define FUNCTION_H
#include"config.h"
char *i2a(int num, char *str, int radix);
void deleteAllMark(std::string &s, const std::string &mark);
void replaceAllMark(std::string& str, const std::string& old_value, const std::string& new_value);
std::string addHtml(std::string& tar);
int Get_proton_num(std::string& ele);
double Get_relative_atomic_mass(std::string& ele);
double Get_relative_atomic_mass(int proton_num);
double Get_electronegativity(int proton_num);
void printMemory();
#endif 
