//////////////////////////////////////////////////////////////////////
// SPDX-License-Identifier: MIT
// Author:  Giovanni Santini
// Mail:    giovanni.santini@proton.me
// Github:  @San7o
//
// singleton
// =========
//
// A design pattern that ensures a class has only one instance and
// provides a global point of access to it.
//

#include <string>
#include <cassert>
#include <mutex>

class Singleton {
protected:
  static std::mutex mtx;
  std::string value;

  explicit Singleton() = default;

public:
  // Delete copy constructor
  Singleton(Singleton &other) = delete;
  // Delete copy assignment
  void operator=(const Singleton&) = delete;

  static void Init(const std::string &val) {
    std::lock_guard<std::mutex> lock(mtx);
    Singleton& s = GetInstance();
    s.value = val;
  }
  
  static Singleton &GetInstance() {
    static Singleton instance;
    return instance;
  }
  
  std::string GetValue() {
    return this->value;
  }
};

std::mutex Singleton::mtx;

int main()
{
  Singleton::Init("foo");
  Singleton& s1 = Singleton::GetInstance();
  Singleton& s2 = Singleton::GetInstance();
  assert(s1.GetValue() == "foo");
  assert(s2.GetValue() == "foo");
  assert(&s1 == &s2);
  
  return 0;
}
