#include "Input.h"

bool Input::getKey(uint16_t sKey) const noexcept
{
    return (keyState_[sKey] & 0x80) != 0;
}

bool Input::isTriggerKey(uint16_t sKey) const noexcept
{
    return false;
}

void Input::updateKeyState(void* pState) noexcept
{
    memcpy_s(keyState_, sizeof(keyState_), pState, sizeof(keyState_));
}
