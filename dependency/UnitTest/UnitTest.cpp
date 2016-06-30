#include "UnitTest.h"


using namespace std;

UnitTest::UnitTest()
{
}

UnitTest::~UnitTest()
{
	vector<ITestClass *>::iterator iTestClass = m_TestClassList.begin();
	vector<ITestClass *>::iterator iEndTestClass = m_TestClassList.end();

	while (iTestClass != iEndTestClass)
	{
		delete *iTestClass;
		++iTestClass;
	}
	m_TestClassList.clear();
}

void UnitTest::run()
{
	vector<ITestClass*>::iterator iTestClass = m_TestClassList.begin();
	vector<ITestClass*>::iterator iEndTestClass = m_TestClassList.end();

	while (iTestClass != iEndTestClass)
	{
		(*iTestClass)->runTests();
		++iTestClass;
	}
}

void UnitTest::addTestClass(ITestClass* testClass)
{
	m_TestClassList.push_back(testClass);
}
