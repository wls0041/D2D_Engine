#pragma once
#include "Framework.h"

enum class EventType : uint { Event_Update, Event_Render, };

#define EVENT_HANDLER_STATIC(func) []() { func(); } //�����Լ�
#define EVENT_HANDLER(func)		   [this]() { func(); } //� ��ü �ȿ� �ִ� �Լ�. �ڵ����� ĳ���õǵ��� ĸ�ĸ� ����

class EventSystem final
{
public:
	typedef std::function<void()> Subscriber;

public:
	static EventSystem &Get() {
		static EventSystem instance;
		return instance;
	}

	void Subscribe(const EventType &type, Subscriber &&func) { 
		subscribers[type].push_back(std::forward<Subscriber>(func)); //forward : lvalue�� rvalue�� ĳ����. (move�Լ��� ����) 
	}
	void Fire(const EventType &type) {
		if (subscribers.find(type) == subscribers.end()) return;
		for (auto subscriber : subscribers[type]) subscriber();
	}
	void Clear() {
		for (auto subscriber : subscribers) {
			subscriber.second.clear();
			subscriber.second.shrink_to_fit();
		}
		subscribers.clear();
	}


private:
	EventSystem() = default;

private:
	std::map<EventType, std::vector<Subscriber>> subscribers;

};