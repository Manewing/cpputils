#ifndef UTILS_TUPLE_HH
#define UTILS_TUPLE_HH

#include <tuple>
#include <sstream>

namespace utils {

  template<typename T>
  T __read_type(std::istream& __in) {
    T __t; __in >> __t;
    return __t;
  }

  template<typename ... Args>
  std::tuple<Args...> read_tuple(std::istream& __in) {
    return std::tuple<Args...>{ __read_type<Args>(__in)... };
  }

  template<typename ... Args>
  std::tuple<Args...> read_tuple(std::string const& __str) {
    std::istringstream __ss(__str);
    return read_tuple<Args...>(__ss);
  }

  template<typename ... Args>
  std::tuple<Args...> typesafe_read_tuple(std::istream& __in) {
    __in.exceptions(std::ios::failbit);
    return read_tuple<Args...>(__in);
  }

  template<typename ... Args>
  std::tuple<Args...> typesafe_read_tuple(std::string const& __str) {
    std::istringstream __ss(__str);
    return typesafe_read_tuple<Args...>(__ss);
  }

  template<size_t __pos, typename ... Args>
  struct __write_tuple;

  template<typename ... Args>
  struct __write_tuple<0, Args...> {
    __write_tuple(std::ostream& __out, std::tuple<Args...> const& __t) {
      __out << std::get<0>(__t);
    }
  };

  template<size_t __pos, typename ... Args>
  struct __write_tuple : public __write_tuple<__pos-1, Args...> {
    __write_tuple(std::ostream& __out, std::tuple<Args...> const& __t):
      __write_tuple<__pos-1, Args...>(__out, __t) {
        __out << " " << std::get<__pos>(__t);
    }
  };

  template<typename ... Args>
  void write_tuple(std::ostream& __out, std::tuple<Args...> const& __t) {
    static const size_t __pos = sizeof...(Args) - 1;
    __write_tuple<__pos, Args...> __wt(__out, __t);
  }

  template<typename ... Args>
  void write_tuple(std::string& __str, std::tuple<Args...> const& __t) {
    std::ostringstream __ss;
    write_tuple(__ss, __t);
    __str = __ss.str();
  }


};

template<typename ... Args>
std::istream& operator >> (std::istream& __in, std::tuple<Args...>& __t) {
  __t = utils::read_tuple(__in);
  return __in;
}

template<typename ... Args>
std::ostream& operator << (std::ostream& __out, std::tuple<Args...> const& __t) {
  utils::write_tuple(__out, __t);
  return __out;
}

#endif // #ifndef UTILS_TUPLE_HH
