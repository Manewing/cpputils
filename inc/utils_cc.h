#ifndef UTILS_CC_HH
#define UTILS_CC_HH

#include <map>
#include <string>

namespace utils {

  class cc_if;

  /*!
   * \class __cc_param
   * \brief Parameter (base), which is accepted by cc_if constructor.
   */
  class __cc_param {
    // Disabled
    __cc_param(__cc_param const& __cc_p) = delete;
    __cc_param(__cc_param const&& __cc_p) = delete;
    __cc_param& operator = (__cc_param const& __cc_p) = delete;

    public:
      //! Constructor
      __cc_param();

      //! Destructor (calls cc_if::cc_up)
      virtual ~__cc_param();
  };

  /*!
   * \class cc_if
   * \brief Constructor Cleanup Interface
   *
   *
   *    Calls cleanup function 'cc_up', after the constructor has
   *    finished. This allows calling (pure) virtual, through a
   *    parent class after the constructor of the child has finished.
   *
   */
  class cc_if {
    friend class __cc_param;

    public:
      typedef std::map<__cc_param const*, cc_if*> context_t;

    public:

      //! Constructor
      cc_if(__cc_param const& __cc_p);

      //! Abstract cleanup function.
      virtual void cc_up() = 0;

    private:
      static context_t  ctx;
  };


  /*!
   * \class cc_param<T>
   * \brief Parameter which is accepted by cc_if constructor.
   *
   * CC Parameter which allows readonly access to template type.
   */
  template<typename T>
  class cc_param : public __cc_param {
    public:
      cc_param(): m_t(T()) {}
      cc_param(T const &t): m_t(t) {}
      virtual ~cc_param() {}
      inline operator T const& () const { return m_t; }

    private:
      T m_t;
  };

  typedef cc_param<int>           cc_int_t;
  typedef cc_param<unsigned int>  cc_uint_t;
  typedef cc_param<float>         cc_float_t;
  typedef cc_param<double>        cc_double_t;
  typedef cc_param<char>          cc_char_t;
  typedef cc_param<unsigned char> cc_byte_t;
  typedef cc_param<std::string>   cc_string_t;
  typedef __cc_param              cc_dummy_t;

}; // namespace utils

#endif // #ifndef UTILS_CC_HH
