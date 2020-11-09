#pragma once

namespace counter {

enum class action { increment, decrement };

inline int reducer(int state, action action) {
  switch (action) {
    case action::increment:
      return state + 1;
    case action::decrement:
      return state - 1;
    default:
      return state;
  }
}

}  // namespace counter