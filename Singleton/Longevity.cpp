//
// Created by lee on 17/7/3.
//

#include "Longevity.h"
#include <stdexcept>

detail::TrackerArray detail::pTrackerArray = nullptr;
unsigned int detail::elements = 0;

#define Assert(e) do {if (!(e)) throw std::runtime_error("Assert failed: " #e);} while(0)

// AtExitFn
// destroy objects
void detail::AtExitFn()
{
    using namespace detail;
    Assert(elements > 0 && pTrackerArray != nullptr);
    // Pick the element at the top of the stack
    LifetimeTracker* pTop = pTrackerArray[elements - 1];
    // Remove that object off the stack
    // can't fail
    pTrackerArray = static_cast<TrackerArray>(std::realloc(pTrackerArray, sizeof(*pTrackerArray) * --elements));
    // Destroy the element
    delete pTop;
}

#undef Assert
