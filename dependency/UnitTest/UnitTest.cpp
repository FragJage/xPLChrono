#include"UnitTest/UnitTest.h"

bool operator==(const TestClass& lhs, const std::string& rhs)
{
	return lhs.ClassName == rhs;
}

bool UnitTest::Run()
{
	std::vector<TestClass>::iterator itClass;
	std::vector<TestClass>::iterator itClassEnd;
	std::vector<TestMethod>::iterator itMethod;
	std::vector<TestMethod>::iterator itMethodEnd;
	int padLenght = 0;
	int len;
  bool result = true;


	itClass = UnitTest::m_TestClassList.begin();
	itClassEnd = UnitTest::m_TestClassList.end();

	while (itClass != itClassEnd)
	{
		len = itClass->ClassName.length();
		if (len > padLenght) padLenght = len;
		while (itMethod != itMethodEnd)
		{
			len = itMethod->MethodName.length();
			if (len > padLenght) padLenght = len;
			++itMethod;
		}
		++itClass;
	}
	padLenght += 10;

	itClass = UnitTest::m_TestClassList.begin();
	itClassEnd = UnitTest::m_TestClassList.end();

	while (itClass != itClassEnd)
	{
		std::cout << termcolor::lightYellow << "- " << itClass->ClassName << " " << std::setfill('-') << std::setw(padLenght+10 - itClass->ClassName.length()) << "-" << std::endl;
		itMethod = itClass->TestMethodList.begin();
		itMethodEnd = itClass->TestMethodList.end();
		while (itMethod != itMethodEnd)
		{
			std::cout << "    " << termcolor::lightWhite << itMethod->MethodName << std::setfill(' ') << std::setw(padLenght - itMethod->MethodName.length()) << " ";
			try
			{
				if ((*(itMethod->TestFunction))())
					std::cout << termcolor::lightGreen << "OK";
				else
        {
          result = false;
          std::cout << termcolor::lightRed << "KO";
        }
			}
			catch(const std::exception & e)
			{
        result = false;
				std::cout << termcolor::lightRed << "FAILED" << std::endl;
				std::cout << termcolor::white << "EXCEPTION" << std::endl << e.what();
			}
			std::cout << std::endl;
			++itMethod;
		}
		std::cout << std::endl;
		++itClass;
	}
	std::cout << termcolor::white;
  return result;
}

