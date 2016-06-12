#include<algorithm>
#include<string>
#include<vector>
#include<iomanip>
#include"termcolor.h"


#define TEST_METHOD(class_name, method_name) \
bool class_name##_##method_name(); \
static TestFunctionAdder class_name##_##method_name##Adder(#class_name, #method_name, &class_name##_##method_name);    \
bool class_name##_##method_name()

typedef bool(*UnitTestFunction)();

struct TestMethod
{
	std::string MethodName;
	UnitTestFunction TestFunction;

	TestMethod(std::string methodName, UnitTestFunction testFunction) : MethodName(methodName), TestFunction(testFunction)
	{
	}
};

struct TestClass
{
	std::string ClassName;
	std::vector<TestMethod> TestMethodList;

	TestClass(std::string className, std::vector<TestMethod> testMethodList) : ClassName(className), TestMethodList(testMethodList)
	{
	}
};

bool operator==(const TestClass& lhs, const std::string& rhs);

class UnitTest
{
	public:
		typedef bool(*TestFunction)();

		bool Run();

	private:
		friend class TestFunctionAdder;
		static std::vector<TestClass> m_TestClassList;
};

class TestFunctionAdder
{
	public:
		TestFunctionAdder(std::string className, std::string methodName, UnitTest::TestFunction func)
		{
			std::vector<TestClass>::iterator it;
			std::vector<TestClass>::iterator itEnd;


			itEnd = UnitTest::m_TestClassList.end();
			it = std::find(UnitTest::m_TestClassList.begin(), itEnd, className);
			if (it == itEnd)
			{
				std::vector<TestMethod> testMethodList = { { methodName, func } };
				UnitTest::m_TestClassList.emplace_back(className, testMethodList);
			}
			else
			{
				it->TestMethodList.emplace_back(methodName, func);
			}
		}
};
