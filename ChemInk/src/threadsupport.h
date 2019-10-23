#pragma once
#include <QRunnable>

enum WorkIndex { IDLE, LOAD, PREDICT };

class ThreadSupport :public QRunnable {
public:
	ThreadSupport();
	~ThreadSupport();
	//  流程：设置任务索引、设置工作负载、线程池调用、检查任务是否结束
	bool isWorking();//  认为同一时刻最多执行一个任务
	void setWorkIndex(WorkIndex _index);//  映射到功能函数
	WorkIndex getWorkIndex();
	void run()override;//  不让传参进去，设置任务索引加以区分
private:
	WorkIndex index;
	bool flag;
protected:// 线程类主要用来支持神经网络模型和分子动力学模拟
	virtual void load() = 0;
	virtual void predict() = 0;

//	class ThreadSupportInterface {
//	protected:
//		virtual void load() = 0;
//		virtual void predict() = 0;
//	};
//private:
//	ThreadSupportInterface* tsinterface;
//public:
//	void regist(ThreadSupportInterface* _tsinterface) {
//		tsinterface = _tsinterface;
//	}
//	void run()override {
//		if (tsinterface) {
//			//TODO: do sth
//		}
//	}
};

