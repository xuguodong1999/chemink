#include "threadsupport.h"
#include <ctime>
#include <iostream>
using namespace std;

ThreadSupport::ThreadSupport() :flag(false), index(IDLE) { 
	setAutoDelete(false);
}

ThreadSupport::~ThreadSupport() { ; }
bool ThreadSupport::isWorking() { return flag; }
void ThreadSupport::setWorkIndex(WorkIndex _index) {
	index = _index;
}

WorkIndex ThreadSupport::getWorkIndex() {
	return index;
}

void ThreadSupport::run() {
	time_t start;
	switch (index) {
	case LOAD:
        flag = true;
		load();
		//qDebug() << "thread id " << QThread::currentThreadId() << "end loading";
		break;
	case PREDICT:
        flag = true;
		start = clock();
		predict();
        //cout << "predictÓÃÊ±" << (clock() - start)/CLOCKS_PER_SEC<< " ms" << endl;
		//qDebug() << "thread id " << QThread::currentThreadId() << ": end cnn calculating";
		break;
	default:
		break;
	}
	flag = false;
}
