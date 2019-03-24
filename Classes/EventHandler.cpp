#include <iostream>

#include "EventHandler.h"

namespace EventHandlerPrivate
{
	static int s_EventIDGenerator = 0;
	int GenerateEventID()
	{
		return ++s_EventIDGenerator;
	}
}

EventHandler::EventHandler()
	: m_id(0)
{ }

EventHandler::EventHandler(const Func &func)
	: m_func(func)
{
	m_id = EventHandlerPrivate::GenerateEventID();
}

void EventHandler::operator()()
{
	m_func();
}

void EventHandler::operator=(const EventHandler &func)
{
	if (m_func == nullptr)
	{
		m_func = func;
		m_id = EventHandlerPrivate::GenerateEventID();
	}
}

bool EventHandler::operator==(const EventHandler &del)
{
	return m_id == del.GetID();
}

bool EventHandler::operator!=(nullptr_t)
{
	return m_func != nullptr;
}