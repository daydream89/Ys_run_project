#pragma once

#include <functional>

class EventHandler
{
public:
	using Func = std::function<void()>;

private:
	Func m_func;

	int m_id;

public:
	EventHandler();
	EventHandler(const Func &func);
	
	void operator()();
	void operator=(const EventHandler &func);
	bool operator==(const EventHandler &del);
	bool operator!=(nullptr_t);

	int GetID() const { return m_id; }
};