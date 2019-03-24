#pragma once

#include <functional>

#include "CommonData.h"

typedef std::function<void(EventCollisionMsg)> CollisionFunc;
typedef std::map<unsigned int, CollisionFunc> CollisionListenerMap;

typedef std::function<void(EventInputMsg)> InputFunc;
typedef std::map<unsigned int, InputFunc> InputListenerMap;

typedef std::function<void(EventSystemMsg)> SystemFunc;
typedef std::map<unsigned int, SystemFunc> SystemListenerMap;

class EventManager
{
private:
	static EventManager *m_pEventManager;
public:
	static EventManager *GetInstance();

	void Init();

	unsigned int AddEventListener(eEventListenerType type, const CollisionFunc &listener);
	unsigned int AddEventListener(eEventListenerType type, const InputFunc &listener);
	unsigned int AddEventListener(eEventListenerType type, const SystemFunc &listener);


	void RemoveEventListener(eEventListenerType type, unsigned int listerID);

	void NotifyEvent(const EventCollisionMsg &msg);
	void NotifyEvent(const EventInputMsg &msg);
	void NotifyEvent(const EventSystemMsg &msg);
	
private:
	unsigned int m_idGenerator;

	CollisionListenerMap m_collEventMap;
	InputListenerMap	 m_inputEventMap;
	SystemListenerMap	 m_systemEventMap;
};