#ifndef UTILS_AM_HH
#define UTILS_AM_HH

#include <map>
#include <vector>
#include <stdint.h>

namespace utils {

  class action_base;

  typedef uint64_t tick_t;

  class action_manager {
    public:
      typedef std::vector<action_base*>  queue_t;
      typedef std::map<tick_t, queue_t*> map_t;

    public:
      action_manager(tick_t ticks = 0);
      virtual ~action_manager();
      virtual void schedule(action_base* ab, tick_t ticks);
      virtual void tick();

      inline tick_t ticks() const { return m_total_ticks; }

    protected:
      bool has_queue(tick_t at);
      queue_t* get_queue(tick_t at);
      void free_queue(tick_t at);

    protected:
      tick_t m_total_ticks;
      map_t  m_action_map;
  };

  class action_base {
    friend class action_manager;

    public:
      action_base(action_manager& am, int priority = 0);
      virtual ~action_base();

      inline bool operator < (action_base const& ab) {
        return m_priority < ab.m_priority;
      }

      virtual void execute() = 0;

      virtual action_base* copy() const = 0;

    protected:
      virtual void execute_internal();

    protected:
      action_manager&  m_am;
      int const        m_priority;
  };

  class paction_base : public action_base {
    public:
      paction_base(action_manager& am, int prio, tick_t period);
      virtual ~paction_base();
      virtual void execute() = 0;

    protected:
      virtual void execute_internal();

    protected:
      tick_t m_period;
  };

}

#endif // #ifndef UTILS_AM_HH
