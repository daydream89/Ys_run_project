#include "Event.h"

void Event::addHandler(const EventListener &listener)
{
	m_listenerList.push_back(listener);
}

void Event::removeHandler(EventListener &listener)
{
	for (EventListenerList::iterator iter = m_listenerList.begin(); iter != m_listenerList.end(); ++iter)
	{
		if (iter->listener == handler)
		{
			m_listenerList.erase(iter);
			break;
		}
	}
}

void Event::operator()(eEventListenerType type)
{
	notify(type);
}

Event &Event::operator+=(const EventListener &handler)
{
	addHandler(handler);

	return *this;
}

Event &Event::operator-=(const EventListener &handler)
{
	removeHandler(*handler.listener);

	return *this;
}

void Event::notify(eEventListenerType type)
{
	for (EventListenerList::iterator iter = m_listenerList.begin(); iter != m_listenerList.end(); ++iter)
	{
		if (*(iter->listener) != nullptr)
			(*(iter->listener))();
	}
}