#pragma once
#include "../../../Cheat/Brand.hpp"
REVERS_STAMP(restore_h)

template <typename T>
class c_restore
{
private:
    T* stored;
    T backup;
public:
    __forceinline c_restore(T* orig)
    {
        stored = orig;
        backup = *orig;
    }

    __forceinline ~c_restore()
    {
        *stored = std::move(backup);
    }
};

#define CONCAT_IMPL( x, y ) x##y
#define MARCO_CONCAT( x, y ) CONCAT_IMPL( x, y )
#define RESTORE(original) auto MARCO_CONCAT( backup, __COUNTER__ ) = c_restore{ &original }
