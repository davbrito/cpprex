#if !defined(CPPREX_H_)
#define CPPREX_H_

#include <algorithm>
#include <functional>
#include <utility>
#include <vector>

namespace rex {

template <typename Reducer, typename State>
class store {
 public:
  using state_type = State;

  using reducer_type = Reducer;
  using callback_type = std::function<void(const state_type&)>;

  template <typename ReducerT>
  store(ReducerT&&, const State&);

  template <typename ReducerT>
  store(ReducerT&&, State&&);

  template <typename ActionT>
  auto dispatch(ActionT&& a) -> std::enable_if_t<
      std::is_invocable_r_v<state_type, reducer_type, state_type, ActionT>>;

  template <typename Callback>
  auto subscribe(Callback&& a)
      -> std::enable_if_t<std::is_constructible_v<callback_type, Callback>>;

  const state_type& get_state() const;

  ~store();

 private:
  std::vector<callback_type> m_callbacks{};
  reducer_type m_reducer;
  state_type m_state;

  void notify() const;
};

/* Deduction Guides */

template <typename Reducer, typename State>
store(Reducer&& reducer, const State& initial_state)
    -> store<std::decay_t<std::remove_reference_t<Reducer>>, State>;

template <typename Reducer, typename State>
store(Reducer&& reducer, State&& initial_state)
    -> store<std::decay_t<std::remove_reference_t<Reducer>>, State>;

/* Constructors */

template <typename R, typename S>
template <typename Reducer>
store<R, S>::store(Reducer&& reducer, const S& initial_state)
    : m_reducer{std::forward<Reducer>(reducer)}, m_state{initial_state} {}

template <typename R, typename S>
template <typename Reducer>
store<R, S>::store(Reducer&& reducer, S&& initial_state)
    : m_reducer{std::forward<Reducer>(reducer)},
      m_state{std::move(initial_state)} {}

/* Destructor */

template <typename R, typename S>
store<R, S>::~store() = default;

/* Member Functions */

template <typename R, typename S>
template <typename ActionT>
auto store<R, S>::dispatch(ActionT&& a) -> std::enable_if_t<
    std::is_invocable_r_v<state_type, reducer_type, state_type, ActionT>> {
  m_state = m_reducer(std::move(m_state), std::forward<ActionT>(a));
  notify();
}

template <typename R, typename S>
template <typename Callback>
auto store<R, S>::subscribe(Callback&& a)
    -> std::enable_if_t<std::is_constructible_v<callback_type, Callback>> {
  m_callbacks.push_back(std::forward<Callback>(a));
}

template <typename R, typename S>
auto store<R, S>::get_state() const -> const state_type& {
  return m_state;
}

template <typename R, typename S>
void store<R, S>::notify() const {
  std::for_each(m_callbacks.begin(), m_callbacks.end(),
                [&](const auto& cb) { cb(m_state); });
}

}  // namespace rex

#endif  // CPPREX_H_
