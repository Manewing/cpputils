#include "utils_am.h"
using namespace utils;

class action : public action_base {
  public:
    action(action_manager& am, int prio):
      action_base(am, prio) {}

    virtual void execute() {
      if (m_priority != m_am.ticks())
        throw 1;
    }

    virtual action_base* copy() const {
      return new action(m_am, m_priority);
    }
};

class paction : public paction_base {
  public:
    paction(action_manager& am, int prio, tick_t period):
      paction_base(am, prio, period) {}

    virtual void execute() {
      if (m_priority != (m_am.ticks() % m_period))
        throw 2;
    }

    virtual action_base* copy() const {
      return new paction(m_am, m_priority, m_period);
    }
};

int main() {
  action_manager am;

  am.schedule(new action(am, 0), 0);
  am.schedule(new action(am, 1), 1);
  am.schedule(new action(am, 2), 2);
  am.schedule(new action(am, 3), 3);
  am.schedule(new action(am, 4), 4);
  am.schedule(new action(am, 5), 5);

  am.schedule(new paction(am, 0, 2), 0);
  am.schedule(new paction(am, 1, 4), 1);

  try {
    for (int l = 0; l < 20; l++) {
      am.tick();
    }
  } catch (int e) {
    return e;
  }

  return 0;
}
