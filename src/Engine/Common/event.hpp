#pragma once

#include <functional>
#include <vector>
#include <unordered_map>

class CEvent {
public:
    ~CEvent() {
        callbacks.clear();
    }
    void addCallback(std::function<void(const CEvent&)> callback) {
        callbacks.push_back(callback);
    }

    // ִ�����лص�
    void trigger() const {
        for (const auto& callback : callbacks) {
            callback(*this);
        }
        return;
    }

    void* event_data;
private:
	std::vector<std::function<void(const CEvent&)>> callbacks; // �洢�¼��Ļص�����
};