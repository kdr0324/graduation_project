#include "stdafx.h"
#include "Semaphore.h"


KSemaphore::KSemaphore()
{
	m_SemaphoreName = "SEMAPHORE";
	m_hKeySemaphore = NULL;
}


KSemaphore::~KSemaphore()
{
	if (m_hKeySemaphore) {
		close();
	}
}

bool KSemaphore::create(string name, int initialCount, int maxCount)
{
	if (name != "") {
		m_SemaphoreName = "SEMAPHORE" + name;
	}
	//m_KeyValue = 0x3829; // just an arbitrary number
	m_hKeySemaphore = CreateSemaphore(NULL, initialCount, maxCount, m_SemaphoreName.c_str()); // 1 critical section MAX_SEM_COUNT keys
	if (m_hKeySemaphore == NULL) {
		if (GetLastError() == ERROR_ALREADY_EXISTS)
			printf("CreateSemaphore(): opened existing %s semaphore.\n", m_SemaphoreName.c_str());
		else
			printf("CreateSemaphore(): error: %d.\n", GetLastError());
		return false;
	}
	else {
		printf("CreateSemaphore(): new %s semaphore successfully created.\n", m_SemaphoreName.c_str());
		return true;
	}
}

void KSemaphore::open(string name)
{
	if (name != "") {
		m_SemaphoreName = "SEMAPHORE" + name;
	}
	m_hKeySemaphore = OpenSemaphore(SEMAPHORE_ALL_ACCESS, FALSE, m_SemaphoreName.c_str());
	if (m_hKeySemaphore == NULL) {
		printf("OpenSemaphore(): error: %d.\n", GetLastError());
	}
	else {
		printf("OpenSemaphore(): %s semaphore successfully opened.\n", m_SemaphoreName.c_str());
	}
}

void KSemaphore::close()
{
	if (m_hKeySemaphore) {
		CloseHandle(m_hKeySemaphore);
		m_hKeySemaphore = NULL;
	}
}

void KSemaphore::lock()
{
	WaitForSingleObject(m_hKeySemaphore, INFINITE);
}

void KSemaphore::unlock()
{
	ReleaseSemaphore(m_hKeySemaphore, 1, NULL);
}