#pragma once
#include <functional>

template<typename... Args>
class Signal {
public:
    using SlotType = std::function<void(Args...)>;

    void Connect(const SlotType& slot) {
        slots.push_back(slot);
    }

    void Emit(Args... args) {
        for (auto& slot : slots) {
            slot(std::forward<Args>(args)...);
        }
    }

private:
    std::vector<SlotType> slots;
};
