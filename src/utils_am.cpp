#include "utils_am.h"

namespace utils {

    template<typename T>
    void free_vector(std::vector<T*>* __v) {
      for (auto __t : *__v)
        delete __t;
      delete __v;
    }

    action_manager::action_manager(tick_t ticks):
      m_total_ticks(ticks),
      m_action_map() {}

    action_manager::~action_manager() {
      for (auto it : m_action_map)
        free_vector(it.second);
    }

    void action_manager::schedule(action_base* ab, tick_t ticks) {
      get_queue(m_total_ticks + ticks)->push_back(ab);
    }

    void action_manager::tick() {
      if (has_queue(m_total_ticks)) {
        queue_t * q = get_queue(m_total_ticks);

        for (auto ab : *q) {
          ab->execute_internal();
        }

        free_queue(m_total_ticks);
      }

      m_total_ticks++;
    }

    bool action_manager::has_queue(tick_t at) {
      return m_action_map.find(at) != m_action_map.end();
    }

    action_manager::queue_t* action_manager::get_queue(tick_t at) {
      if (!has_queue(at)) {
        m_action_map[at] = new queue_t();
      }
      return m_action_map[at];
    }

    void action_manager::free_queue(tick_t at) {
      free_vector(m_action_map[at]);
      m_action_map.erase(at);
    }

    action_base::action_base(action_manager& am, int priority):
      m_am(am), m_priority(priority) {
    }

    action_base::~action_base() {
    }

    void action_base::execute_internal() {
      execute();
    }

    paction_base::paction_base(action_manager& am, int prio, tick_t period):
      action_base(am, prio), m_period(period) {
    }

    paction_base::~paction_base() {}

    void paction_base::execute_internal() {
      action_base::execute_internal();
      m_am.schedule(copy(), m_period);
    }
}
