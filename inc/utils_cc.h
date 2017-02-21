#ifndef UTILS_CC_HH
#define UTILS_CC_HH

#include <string>

namespace utils {

  class cc_if {
    public:
      cc_if() {
        cc_if::current = this;
      }

    public:
      virtual void cc_up() = 0;
      static cc_if * current;
  };

  template<typename T>
  class cc_param {
    friend class cc_if;
    public:
      cc_param(): m_t(T()) {}
      cc_param(T const &t): m_t(t) {}
      inline T operator=(T t) { return m_t = t; }
      inline operator T&() { return m_t; }
      inline operator T const&() { return m_t; }
      virtual ~cc_param() {
        if (cc_if::current) {
          cc_if::current->cc_up();
          cc_if::current = NULL;
        }
      }
    private:
      T m_t;
  };

}; // namespace utils

#endif // #ifndef UTILS_CC_HH
