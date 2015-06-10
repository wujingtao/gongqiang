/*
 * =====================================================================================
 *
 *       Filename:  MyList.h
 *        Created:  2014/8/20 14:38:59
 *       Compiler:  vs2010 
 *         Author:  jingtao.wu
 *
 *    Description:  自己添加的模板队列
 *
 * =====================================================================================
 */

#ifndef  MYLIST_INC
#define  MYLIST_INC

#include <qmutex.h>

template <typename T>
class MyList
{
public:
MyList(void)
{
	mHead= 0;
	mCur= 0;
	mTotal = 0;
}
	
private:
struct Node
{
	T cp;	
	Node *next;
};

public:

/* 
 *功能: 添加到队列
 *参数: null
 *返回: void
 */
void push_back(T cp)
{
	if(cp == 0)
	{return;}

	mMutex.lock();
	Node *node = new Node();
	node->cp = cp;
	node->next = 0;

	if(mHead == 0)
	{
		mHead = node;	
		mCur = node;
	}
	else
	{
		mCur->next = node;	
		mCur = node;
	}

	++mTotal;
	mMutex.unlock();
	
}


/* 
 *功能: 获取队列头
 *参数: null
 *返回: void
 */
T front()
{
	if(mTotal == 0)
	{
		return 0;	
	}
	return mHead->cp;
}


/* 
 *功能: 删除队列头并释放该内存
 *参数: null
 *返回: void
 */
void pop_front()
{
	mMutex.lock();

	if(mTotal != 0)
	{
		T tmp = mHead->cp;
		mHead = mHead->next;	
		--mTotal;

		delete[] tmp;
	}
	mMutex.unlock();
}

//返回队列大小
int size()
{
	return mTotal;
}

private:
	Node *mHead;
	Node *mCur;

	QMutex mMutex;
	int mTotal;
};

#endif   /* ----- #ifndef MYLIST_INC  ----- */
