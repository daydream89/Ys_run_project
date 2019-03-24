#include "EventManager.h"

EventManager *EventManager::m_pEventManager = nullptr;
EventManager *EventManager::GetInstance()
{
	if (m_pEventManager == nullptr)
		m_pEventManager = new EventManager;
	return m_pEventManager;
}

void EventManager::Init()
{
	m_idGenerator = 0;

	m_collEventMap.clear();
	m_inputEventMap.clear();
	m_systemEventMap.clear();
}

unsigned int EventManager::AddEventListener(eEventListenerType type, const CollisionFunc &listener)
{
	if (type != EVENT_TYPE_COLLISION)
		return -1;

	m_collEventMap.insert(std::make_pair(m_idGenerator, listener));
	return m_idGenerator++;
}

unsigned int EventManager::AddEventListener(eEventListenerType type, const InputFunc &listener)
{
	if (type != EVENT_TYPE_INPUT)
		return -1;

	m_inputEventMap.insert(std::make_pair(m_idGenerator, listener));
	return m_idGenerator++;
}

unsigned int EventManager::AddEventListener(eEventListenerType type, const SystemFunc &listener)
{
	if (type != EVENT_TYPE_GAMEOVER && type != EVENT_TYPE_NEXT_STAGE && type != EVENT_TYPE_RESTART && type != EVENT_TYPE_OPPONENT_CREATE)
		return -1;

	m_systemEventMap.insert(std::make_pair(m_idGenerator, listener));
	return m_idGenerator++;
}

void EventManager::RemoveEventListener(eEventListenerType type, unsigned int listenerID)
{
	if (type == EVENT_TYPE_COLLISION)
	{
		CollisionListenerMap::iterator iter = m_collEventMap.find(listenerID);
		if (iter == m_collEventMap.end())
			return;

		m_collEventMap.erase(iter);
	}
	else if (type == EVENT_TYPE_INPUT)
	{
		InputListenerMap::iterator iter = m_inputEventMap.find(listenerID);
		if (iter == m_inputEventMap.end())
			return;

		m_inputEventMap.erase(iter);
	}
	else if (type == EVENT_TYPE_GAMEOVER || type == EVENT_TYPE_NEXT_STAGE || type == EVENT_TYPE_RESTART || type == EVENT_TYPE_OPPONENT_CREATE)
	{
		SystemListenerMap::iterator iter = m_systemEventMap.find(listenerID);
		if (iter == m_systemEventMap.end())
			return;

		m_systemEventMap.erase(iter);
	}
}

void EventManager::NotifyEvent(const EventCollisionMsg &msg)
{
	for (const std::pair<unsigned int, CollisionFunc> &listener : m_collEventMap)
	{
		listener.second(msg);
	}
}

void EventManager::NotifyEvent(const EventInputMsg &msg)
{
	for (const std::pair<unsigned int, InputFunc> &listener : m_inputEventMap)
	{
		listener.second(msg);
	}
}

void EventManager::NotifyEvent(const EventSystemMsg &msg)
{
	for (const std::pair<unsigned int, SystemFunc> &listener : m_systemEventMap)
	{
		listener.second(msg);
	}
}