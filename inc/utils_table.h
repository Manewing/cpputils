#ifndef UTILS_TABLE_HH
#define UTILS_TABLE_HH

#include <vector>
#include <algorithm>

#include "utils_tuple.h"

namespace utils {

  struct parse_exception {
    parse_exception(size_t __line, std::string const& __err_symb):
      line(__line), err_symb(__err_symb) {}
    size_t line;
    std::string err_symb;
  };

  template <typename Arg, typename ... Args>
  class table {
    public:
      typedef std::tuple<Arg, Args...>           row_t;
      typedef std::vector<row_t>                 row_buf_t;
      typedef typename row_buf_t::iterator       iterator;
      typedef typename row_buf_t::const_iterator const_iterator;

      typedef enum { PARSE = 0x1 } exception_t;

    public:
      table():
        m_row_buf(),
        m_exception(PARSE) {}

      table(std::istream& in):
        m_row_buf(),
        m_exception(PARSE) {
        read(in);
      }

      virtual ~table() {}

      inline void exceptions(exception_t __e) const {m_exception = __e;}

      inline size_t columns() const {return 1 + sizeof...(Args);}
      inline size_t rows()    const {return m_row_buf.size();}
      inline size_t size()    const {return m_row_buf.size();}

      inline iterator begin() {return m_row_buf.begin();}
      inline iterator end()   {return m_row_buf.end();}

      inline const_iterator begin() const {return m_row_buf.begin();}
      inline const_iterator end()   const {return m_row_buf.end();}

      inline row_t& operator[] (size_t row) {
        return m_row_buf[row];
      }

      inline row_t const& operator[] (size_t row) const {
        return m_row_buf[row];
      }

      inline void push_back(row_t const& r) {
        m_row_buf.push_back(r);
      }

      inline iterator insert(iterator pos, row_t const& r) {
        return m_row_buf.insert(pos, r);
      }

      inline iterator erase(iterator pos) {
        return m_row_buf.erase(pos);
      }

      inline iterator erase(iterator first, iterator last) {
        return m_row_buf.erase(first, last);
      }

      std::istream& read(std::istream& in) {
        while (!in.eof()) {
          std::string line;
          std::getline(in, line);

          if (filter(line))
            continue;

          std::istringstream ss(line);
          try {
            m_row_buf.push_back(utils::typesafe_read_tuple<Arg, Args...>(ss));
          } catch (std::istream::failure const& e) {
            ss.clear();
            std::string err_symb; ss >> err_symb;

            if (m_exception & PARSE)
              throw parse_exception(rows()+1, err_symb);
          }

          if (!filter(__read_type<std::string>(ss)) && m_exception & PARSE)
            throw parse_exception(rows(), ss.str());

        }

        return in;
      }

      std::ostream& write(std::ostream& out) const {
        for (auto& r : m_row_buf) {
          out << r << std::endl;
        }
        return out;
      }

    protected:
      virtual bool filter(std::string const& l) const {
        std::string tmp = l;
        tmp.erase(std::remove_if(tmp.begin(), tmp.end(), isspace), tmp.end());

        if (tmp[0] == '#' || tmp.size() == 0)
          return true;
        return false;
      }

    protected:
      row_buf_t           m_row_buf;
      mutable exception_t m_exception;
  };

};

template<typename Arg, typename ... Args>
std::istream& operator >> (std::istream& in, utils::table<Arg, Args...>& t) {
  return t.read(in);
}

template<typename Arg, typename ... Args>
std::ostream& operator << (std::ostream& out, utils::table<Arg, Args...> const& t) {
  return t.write(out);
}

#endif // #ifndef UTILS_TABLE_HH
