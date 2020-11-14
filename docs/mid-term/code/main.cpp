
/**/
#include "TaskDistributionInterface.h"
#include <QtWidgets/QApplication>
int main(int argc, char *argv[]) {
	QApplication a(argc, argv);
	TaskDistributionInterface w;
	w.show();
	return a.exec();
}
/**

#include "MojoCNN/dictionary.h"
#include <iostream>
#include <string>
using namespace std;
int main() {
	DCinit();
	string s;
	//while(cin>>s)
	//	cout << DCgetAllNext(s) << endl;
	DCdisplayAll();
	system("pause"); return 0;
}
/**/
