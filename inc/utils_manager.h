#ifndef UTILS_MANAGER_HH
#define UTILS_MANAGER_HH

#include <map>
#include <vector>
#include <stdexcept>

namespace utils {

  template<class T, class CT>
  class manager_base {
    public:
      typedef typename CT::iterator iterator;
      typedef typename CT::const_iterator const_iterator;

      virtual ~manager_base() {}

      inline iterator begin() {
        return m_buf.begin();
      }

      inline iterator end() {
        return m_buf.end();
      }

      inline const_iterator begin() const {
        return m_buf.begin();
      }

      inline const_iterator end() const {
        return m_buf.end();
      }

      inline size_t size() const {
        return m_buf.size();
      }

    protected:
      CT m_buf;

  };

  template<class T>
  class mem_manager : public manager_base<T, std::vector<T*> > {

    public:

      typedef typename mem_manager::iterator iterator;
      typedef typename mem_manager::const_iterator const_iterator;

      virtual ~mem_manager() {
        for (iterator it = this->begin(); it != this->end(); it++)
          delete *it;
      }

      virtual T* create() {
        if (this->m_buf.empty())
          return new T;
        T * t = this->m_buf.back();
        this->m_buf.pop_back();
        return t;
      }

      virtual void free(T* t) {
        if (t)
          this->m_buf.push_back(t);
      }
  };

  template<typename ID, class T>
  class manager : public manager_base<T, std::map<ID, T*> > {

    public:

      typedef typename manager::iterator iterator;
      typedef typename manager::const_iterator const_iterator;

      virtual ~manager() {
        for (iterator it = this->begin(); it != this->end(); it++)
          delete it->second;
      }

      inline iterator find(ID const& id) {
        return this->m_buf.find(id);
      }

      inline const_iterator find(ID const& id) const {
        return this->m_buf.find(id);
      }

      inline bool exists(ID const& id) const {
        return this->find(id) != this->end();
      }

      virtual T* create(ID const& id) {
        if (this->exists(id))
          throw std::runtime_error("cannot allocate new element");
        T * t = new T(id);
        this->m_buf[id] = t;
        return t;
      }

      virtual T* get(ID const& id) {
        if (!this->exists(id))
          throw std::out_of_range("cannot get element");
        return this->m_buf[id];
      }

      virtual void free(ID const& id) {
        iterator it = this->find(id);
        if (it != this->end()) {
          delete it->second;
          this->m_buf.erase(it);
        }
      }
  };

}; // namespace utils

#endif // #ifndef UTILS_MANAGER_HH
