#include "utils_cc.h"
#include <set>

#ifdef UTILS_CC_THREAD_SAFE
#include <mutex>
#include <thread>
#endif

class simple_test : public utils::cc_if {
  public:

    simple_test(utils::cc_uint_t const& id):
      cc_if(id), m_id(id), m_called(false) {
      /* nothing todo */
    }

    virtual void cc_up() {
      m_called = true;
    }

    inline bool ok() const { return m_called; }

  private:
    unsigned int  m_id;
    bool          m_called;
};

class multi_test : public utils::cc_if {
  public:
    multi_test(utils::cc_uint_t const& count):
      cc_if(count), m_test_set(), m_called(false) {
      for (unsigned int l = 0; l < count; l++) {
        m_test_set.insert(new simple_test(l));
      }
    }

    virtual ~multi_test() {
      for (auto t : m_test_set)
        delete t;
    }

    virtual void cc_up() {
      m_called = true;
    }

    inline bool ok() const {
      bool status = m_called;
      for (auto t : m_test_set)
        status = status && t->ok();
      return status;
    }

  private:
    std::set<simple_test*> m_test_set;
    bool                   m_called;
};

class deep_test : public multi_test {
  public:
    deep_test(utils::cc_uint_t const& count):
      multi_test(count), m_called(false) {
    }

    virtual ~deep_test() {}

    virtual void cc_up() {
      multi_test::cc_up();
      m_called = true;
    }

    inline bool ok() const {
      return m_called && multi_test::ok();
    }

  public:
    bool m_called;
};

#ifdef UTILS_CC_THREAD_SAFE
std::mutex gbl_mtx;
std::set<multi_test*> gbl_test_set;

void creator_thread(unsigned int count) {
  std::set<multi_test*> tmp_set;
  for (unsigned int l = 0; l < count; l++)
    tmp_set.insert(new multi_test(count));

  gbl_mtx.lock();
  for (auto t : tmp_set)
    gbl_test_set.insert(t);
  gbl_mtx.unlock();
}
#endif

int main() {
  simple_test st(9);
  if (!st.ok())
    return 1;

  multi_test  mt(5);
  if (!mt.ok())
    return 2;

  deep_test   dt(5);
  if (!dt.ok())
    return 3;

#ifdef UTILS_CC_THREAD_SAFE
  std::thread t1(creator_thread, 50);
  std::thread t2(creator_thread, 50);
  std::thread t3(creator_thread, 50);
  std::thread t4(creator_thread, 50);

  t1.join();
  t2.join();
  t3.join();
  t4.join();

  bool cc_thread_ok = true;
  for (auto t : gbl_test_set) {
    cc_thread_ok = cc_thread_ok && t->ok();
    delete t;
  }

  if (!cc_thread_ok)
    return 4;

#endif

  return 0;
}
