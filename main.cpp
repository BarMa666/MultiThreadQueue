// TestInfomax.cpp : Этот файл содержит функцию "main". Здесь начинается и заканчивается выполнение программы.
//
#include <Windows.h>
#include <random>

#define _DEBUG_LVL 1

#include "RequestHandler.h"



#ifdef _TEST
class TestExternal
{
public:
	TestExternal(int _count) : m_count(_count), m_maxCount(_count)
	{}

	Request* GetRequest() throw()
	{
		if (m_count > 0)
		{
			--m_count;
			// srand((unsigned)time(0));
			DWORD period = 6000; // (rand() % 10) * 1000;
			Sleep(period);
			return new Request;
		}
		return nullptr;
	}

	void ProcessRequest(Request* _req) throw()
	{
		// srand((unsigned)time(0));
		DWORD period = 5000; // (rand() % 10) * 1000;
		Sleep(period);
	}

	int GetCount() const
	{
		return m_maxCount;
	}

private:
	int m_count;
	const int m_maxCount;
};

void Test(int _count)
{
	TestExternal test(_count);
	std::cout << "Test with " << test.GetCount() << " request starts\n";
	std::function<Request* (void)> get_request_foo = std::bind(&TestExternal::GetRequest, &test);
	std::function<void(Request*)> process_request_foo = std::bind(&TestExternal::ProcessRequest, &test, std::placeholders::_1);
	RequestHandler handler(get_request_foo, process_request_foo);
	handler.init();
	handler.start();
	std::cout << "Test with " << test.GetCount() << " request ends\n";
}
#endif

int main()
{
#ifdef _TEST
	Test(10);
	Test(100);
	Test(1);
	Test(15);
	Test(0);
	Test(42);
	std::cout << "Program ends here\n";
#endif
}

// Запуск программы: CTRL+F5 или меню "Отладка" > "Запуск без отладки"
// Отладка программы: F5 или меню "Отладка" > "Запустить отладку"

// Советы по началу работы 
//   1. В окне обозревателя решений можно добавлять файлы и управлять ими.
//   2. В окне Team Explorer можно подключиться к системе управления версиями.
//   3. В окне "Выходные данные" можно просматривать выходные данные сборки и другие сообщения.
//   4. В окне "Список ошибок" можно просматривать ошибки.
//   5. Последовательно выберите пункты меню "Проект" > "Добавить новый элемент", чтобы создать файлы кода, или "Проект" > "Добавить существующий элемент", чтобы добавить в проект существующие файлы кода.
//   6. Чтобы снова открыть этот проект позже, выберите пункты меню "Файл" > "Открыть" > "Проект" и выберите SLN-файл.
