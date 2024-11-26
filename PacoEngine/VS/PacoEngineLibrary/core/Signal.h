#pragma once
#include <iostream>
#include <vector>
#include <algorithm>

template <typename ReturnType, typename... Args>
struct Signal {
public:
    using SlotType = ReturnType(*)(Args...);   

    void Connect(SlotType p_slot) {
        m_slots.push_back(p_slot);
    }

    void Emit(Args... args) {
        for (const auto& slot : m_slots) {
            slot(args...);
        }
    }

    void Disconnect(SlotType p_slot) {
        m_slots.erase(std::remove(m_slots.begin(), m_slots.end(), p_slot), m_slots.end());
    }

private:
    std::vector<SlotType> m_slots;
};