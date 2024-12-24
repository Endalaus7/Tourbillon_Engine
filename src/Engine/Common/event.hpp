#pragma once

#include <functional>
#include <vector>
#include <list>
#include <unordered_map>

#define METHOD_LISTENER(Listener) std::bind(&Listener, this, std::placeholders::_1)
#define FUNCTION_LISTENER(Listener) std::bind(&Listener, std::placeholders::_1)

class CEvent {
public:
    ~CEvent() {
        callbacks.clear();
    }
    void addCallback(std::function<void(const CEvent&)> callback) {
        callbacks.push_back(callback);
    }

    // 执行所有回调
    void trigger() const {
        for (const auto& callback : callbacks) {
            callback(*this);
        }
        return;
    }

    void* event_data = NULL;
    size_t dataSize = 0;
private:
	std::list<std::function<void(const CEvent&)>> callbacks; // 存储事件的回调函数
};