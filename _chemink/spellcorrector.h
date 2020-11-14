#pragma once

#include <string>
#include <vector>
#include <map>
#include <iostream>

class SpellCorrector {
public:
    SpellCorrector();

    ~SpellCorrector();

    //  人工标记字符类型
    enum LETTERTYPE {
        DIGIT,        //  数字
        CAPITAL,    //  大写字母
        LITTLE,        //  小写字母
        INFO,        //  注释符如上下箭头
        EXPLICTCUT,    //  分隔符+、=
        LB,            //  左括号
        RB,            //  右括号
        NONE,        //  无意义字符
    };

    static std::string getChemAscii(const std::string &_src);

    int getCnnIndex(int outIndex);

    int getYoloIndex(int outIndex);

    static const std::vector<std::vector<std::string>> similarWords;

    std::vector<std::string> getResult(const std::vector<std::vector<std::string>> &_src);

    void getBestResult(const std::vector<std::vector<std::string>> &_src, std::vector<std::string> &result);

    static int getWordType(const std::string &_word);

private:
    static std::map<std::string, std::string> unicode2chemAsciiMap;
    //  static std::map<std::string, int> chemAscii2typeMap;
    static std::vector<std::string> validWords;

    std::string getValidWord(const std::vector<std::vector<std::string>> &_src,
                             int start, int end);
};
