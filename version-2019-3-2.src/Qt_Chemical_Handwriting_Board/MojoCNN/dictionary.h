#ifndef DICTIONARY_H
#define DICTIONARY_H

#include <map>
#include <iostream>
#include <vector>
#include <string>
#include <list>
#include <algorithm>
using std::list; using std::cout; using std::vector; using std::map; using std::endl; using std::string;
//三层hash处理单词的前三个字母,后面的字母用string存
#define SUM_OF_KEY_CHAR 64
struct HashNode2 {
	string txt;
	bool Exist;
	HashNode2();
};
struct HashNode1 {
	HashNode2 hashnode2[SUM_OF_KEY_CHAR];
	bool Exist;
	HashNode1();
};
struct HashNode0 {
	HashNode1 hashnode1[SUM_OF_KEY_CHAR];
	bool Exist;
	HashNode0();
};
int char2int(char c);
char int2char(int i);
string DCgetAllNext(string _prefix);
void DCinsert(string _word);
void DCinit();
void DCdisplayAll();
int DCmakeChoice(string _prefix, vector<float>& weight);
#endif // !DICTIONARY_H

