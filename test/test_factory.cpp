#include "utils_factory.h"

#include <iostream>
#include <string>

class test_base {
  public:
    typedef unsigned int                                   type_t;
    typedef utils::factory<test_base, type_t, std::string> factory_t;

  private:
    static factory_t factory;

  public:
    static test_base * create(type_t const& t, std::string const& nm) {
      return factory.create(t, nm);
    }

    template<class D>
    static bool add(type_t const& t) {
      return factory.add<D>(t);
    }

    static bool remove(type_t const& t) {
      return factory.remove(t);
    }

  public:
    test_base(std::string const& name):
      m_name(name) {}
    virtual ~test_base() {}

    virtual void print() const = 0;

  protected:
    std::string m_name;
};

test_base::factory_t test_base::factory;

class testA : public test_base {
  public:
    static const test_base::type_t type;

  public:
    testA(std::string const& name):
      test_base(name) {}
    virtual ~testA() {}

    virtual void print() const {
      std::cout << "testA[" << testA::type << "]: " << m_name << std::endl;
    }
};
const test_base::type_t testA::type = 0;

class testB : public test_base {
  public:
    static const test_base::type_t type;

  public:
    testB(std::string const& name):
      test_base(name) {}
    virtual ~testB() {}

    virtual void print() const {
      std::cout << "testB[" << testB::type << "]: " << m_name << std::endl;
    }
};
const test_base::type_t testB::type = 1;

class testC : public test_base {
  public:
    static const test_base::type_t type;

  public:
    testC(std::string const& name):
      test_base(name) {}
    virtual ~testC() {}

    virtual void print() const {
      std::cout << "testC[" << testC::type << "]: " << m_name << std::endl;
    }
};
const test_base::type_t testC::type = 2;

int main() {
  test_base::add<testA>(testA::type);
  test_base::add<testB>(testB::type);

  test_base * ta = test_base::create(testA::type, "A");
  std::cout << ta << std::endl;
  if (!dynamic_cast<testA*>(ta))
    return 1;
  ta->print();

  test_base * tb = test_base::create(testB::type, "B");
  std::cout << tb << std::endl;
  if (!dynamic_cast<testB*>(tb))
    return 2;
  tb->print();

  if (test_base::create(testC::type, "C"))
    return 3;


  test_base::add<testC>(testC::type);
  test_base * tc = test_base::create(testC::type, "C");
  std::cout << tc << std::endl;
  if (!dynamic_cast<testC*>(tc))
    return 3;
  tc->print();

  test_base::remove(testA::type);
  if (test_base::create(testA::type, "A"))
    return 4;

  test_base::remove(testB::type);
  if (test_base::create(testB::type, "B"))
    return 5;

  if (test_base::add<testC>(testC::type))
    return 6;

  if (test_base::remove(testA::type))
    return 7;

  delete ta;
  delete tb;
  delete tc;
  return 0;
}

