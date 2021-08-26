#include <exception>
#include "RequestHandler.h"

#ifndef BAD_ALLOC
#define BAD_ALLOC_RESULT 111
#endif

#ifdef _TEST
RequestHandler::RequestHandler(	std::function<Request* () throw()> _get_request, 
								std::function<void(Request*) throw()> _process_request) noexcept
	: m_data()
	, m_alive(_get_request && _process_request)
	, GetRequest(_get_request)
	, ProcessRequest(_process_request)
{
}
#else

RequestHandler::RequestHandler() noexcept
	: m_data()
	, m_alive(true)
{
}
#endif

RequestHandler::~RequestHandler()
{
	m_alive = false;
	m_condition.notify_all();
	for (int i = 0; i < NumberOfThreads; ++i)
	{
		if (threads[i]->joinable())
		{
			threads[i]->join();
		}
	}
#if _DEBUG_LVL > 1
	std::cout << m_data.size() << " requests left\n";
#endif
}

void RequestHandler::init()
{
	for (int i = 0; i < NumberOfThreads; ++i)
	{
		try
		{
			threads[i] = std::make_unique<std::thread>(&RequestHandler::doWork, this);
		
#if _DEBUG_LVL > 1
		std::cout << threads[i]->get_id() << ": created\n";
#endif
		}
		catch (const std::bad_alloc& e)
		{
			std::cerr << e.what() << "\n";
			exit(BAD_ALLOC_RESULT);
		}
	}
}

void RequestHandler::start()
{
	Request* request = nullptr;
	if (alive())
	{
		while (request = GetRequest())
		{
			{
				std::unique_lock<std::mutex> lk(m_mutex);
				m_data.push(std::make_unique<Request>(*request));
			}
			m_condition.notify_one();
		}
	}
#if _DEBUG_LVL > 1
	std::cout << "i'm done here\n";
#endif
}

bool RequestHandler::alive() const
{
	return m_alive.load();
}

/*
* »де€ вз€та из книги Ёнтони ”иль€мса "ѕрактика многопоточного программировани€ —++"
* и "разбавлена" с учетом замечаний https://www.modernescpp.com/index.php/c-core-guidelines-be-aware-of-the-traps-of-condition-variables
*/
void RequestHandler::doWork()
{
	while(alive())
	{
		std::unique_lock<std::mutex> lk(m_mutex); 
		// ожидание прекратитс€ если во врем€ уведомлени€ очередь не пуста и обработчик "жив"
		m_condition.wait(lk, [this] {return !(m_data.empty() && alive()); });
		// нужно проверить, действительно ли очередь не пуста
		if (!m_data.empty())
		{
			auto request = std::move(m_data.front());
			m_data.pop();
			// освободим мьютекс, чтобы позволить другому потоку работать с очередью
			lk.unlock();
#if _DEBUG_LVL > 2
			std::cout << std::this_thread::get_id() << ": start work\n";
#endif
			ProcessRequest(request.get());
#if _DEBUG_LVL > 2
			std::cout << std::this_thread::get_id() << ": end work\n";
#endif
		}
	}
#if _DEBUG_LVL > 1
	std::cout << std::this_thread::get_id() << " done\n";
#endif
}
