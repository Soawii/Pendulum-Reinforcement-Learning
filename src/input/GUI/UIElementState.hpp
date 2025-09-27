#pragma once

enum class UIElementState {
    HIDDEN = 0,
    DISABLED = 1,
    NORMAL = 2,
    HOVER = 3,
    ACTIVE = 4,
    PRESSED = 5,
    STATE_AMOUNT = 6
};

enum class UISizeMode {
    FIXED = 0,
    FIT_CONTENT = 1
};