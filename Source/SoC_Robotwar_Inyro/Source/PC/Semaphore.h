#pragma once
#ifndef __WIN32_SEM_H__
#define __WIN32_SEM_H__

#include <Windows.h>
#include <string>

using namespace std;

class KSemaphore
{
public:
	KSemaphore();
	virtual ~KSemaphore();
	bool create(string name, int initialCount = 1, int maxCount = 1);
	void open(string name);
	void close();
	void lock();
	void unlock();

private:
	string m_SemaphoreName;
	HANDLE m_hKeySemaphore;
};

#endif