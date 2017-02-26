#ifndef UTILS_FACTORY_HH
#define UTILS_FACTORY_HH

#include <map>

namespace utils {

  template<class C, class D, typename ... Args>
  class f_line;

  template<class C, typename ... Args>
  class __f_line {
    template<class _C, class _D, typename ... _Args>
    friend class f_line;
    private:
      __f_line() {}
    public:
      virtual ~__f_line() {}
      virtual C* create(Args... args) const = 0;
  };

  template<class C, class D, typename ... Args>
  class f_line : public __f_line<C, Args...> {
    public:
      f_line() {}
      virtual ~f_line() {}
      virtual C* create(Args ... args) const {
        return new D(args...);
      }
  };

  template<class C, typename T, typename ... Args>
  class factory {
    public:
      typedef __f_line<C, Args...>                  line_t;
      typedef std::map<T, line_t*>                  line_map_t;
      typedef typename line_map_t::iterator         iterator;
      typedef typename line_map_t::const_iterator   const_iterator;

    public:
      factory(): m_line_map() {}

      virtual ~factory() {
        for (auto it : m_line_map)
          delete it.second;
      }

      inline iterator begin() { return m_line_map.begin(); }
      inline iterator end() { return m_line_map.end(); }
      inline const_iterator begin() const { return m_line_map.begin(); }
      inline const_iterator end() const { return m_line_map.end(); }

      inline bool exists(T const& t) const {
        return m_line_map.find(t) != m_line_map.end();
      }

      C* create(T const& t, Args ... args) {
        if (!exists(t))
          return NULL;
        return m_line_map[t]->create(args...);
      }

      template<class D>
      bool add(T const& t) {
        if (exists(t))
          return false;

        m_line_map[t] = new f_line<C, D, Args...>;;
        return true;
      }

      bool remove(T const& t) {
        iterator it = m_line_map.find(t);
        if (it != m_line_map.end()) {
          delete it->second;
          m_line_map.erase(it);
          return true;
        }

        return false;
      }

    protected:
      line_map_t m_line_map;
  };

}; // namespace utils

#endif // #ifndef UTILS_FACTORY_HH
