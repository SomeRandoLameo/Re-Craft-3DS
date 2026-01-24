#pragma once

struct PadAngles {
    u8 up, down, left, right;

    bool operator==(u8 i) { return this->up == i && this->down == i && this->left == i && this->right == i; }
    bool operator!=(u8 i) { return !(*this == i); }
    bool operator==(const PadAngles& o) {
        return this->up == o.up && this->down == o.down && this->left == o.left && this->right == o.right;
    }
    bool operator!=(const PadAngles& o) { return !(*this == o); }
};

