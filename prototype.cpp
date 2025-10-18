//////////////////////////////////////////////////////////////////////
// SPDX-License-Identifier: MIT
// Author:  Giovanni Santini
// Mail:    giovanni.santini@proton.me
// Github:  @San7o
//
// prototype
// =========
//
// The Prototype pattern delegates the cloning process to the actual
// objects that are being cloned. The pattern declares a common
// interface for all objects that support cloning. This interface lets
// you clone an object without coupling your code to the class of that
// object. Usually, such an interface contains just a single clone
// method.
//
// An object that supports cloning is called a prototype.
//

#include <iostream>
#include <cassert>
#include <memory>

class Prototype {
public:
  virtual ~Prototype() = default;
  virtual std::unique_ptr<Prototype> clone() const = 0;

  // In C++, the virtual cloned method declared in the base class
  // can only return the Prototype type and not the derived classes,
  // meaning that we cannot implement something like:
  //
  //    std::unique_ptr<Button> clone() const override;
  //
  // Because it has a different signature than the virtual function
  // and this defeats the point of having the prototype interface in
  // the first place.
  //
  // The best we can do is to use this trick with clone_as.
  template <typename T>
  std::unique_ptr<T> clone_as() const {
    return std::unique_ptr<T>(static_cast<T*>(clone().release()));
  }
};

class Button : public Prototype {
public:
  int x;
  int y;
  std::string text;

  // This function will be used by the clone method, so it should
  // accepts all class variables 
  Button(int x, int y, std::string text)
    : x(x), y(y), text(text) {}
  
  void click() {
    std::cout << "Button clicked\n";
  }
  
  std::unique_ptr<Prototype> clone() const override {
    return std::make_unique<Button>(this->x, this->y, this->text);
  }
};

//
// Implementation with C++20 concepts
// ==================================
//
// A better option would be to use C++20 concepts. You can define a
// concept Copyable that ensures that a class can be copied. Then, you
// can static_assert to guarantee that the class meets the
// requirement.
//

#include <concepts>
#include <type_traits>

template <typename T>
concept Copyable =
std::copy_constructible<T> && std::is_copy_assignable<T>::value;

class Checkbox {
public:
  std::string text;
  bool checked;
  Checkbox(std::string text) {
    this->text = text;
    this->checked = false;
  }
  void check() { this->checked = true; }

  // Copy constructor
  Checkbox(const Checkbox&) = default;
  // Copy assignment operator
  Checkbox& operator=(const Checkbox&) = default;
};

static_assert(Copyable<Checkbox>);

int main(void)
{
  Button button = Button(10, 100, "test");
  assert(button.x == 10);
  assert(button.y == 100);
  assert(button.text == "test");
  std::unique_ptr<Button> button_cloned = button.clone_as<Button>();
  assert(button_cloned->x == 10);
  assert(button_cloned->y == 100);
  assert(button_cloned->text == "test");

  Checkbox cb = Checkbox("click me!");
  assert(cb.text == "click me!");
  assert(cb.checked == false);
  Checkbox cb_copy = cb;
  assert(cb_copy.text == "click me!");
  assert(cb_copy.checked == false);
  return 0;
}
