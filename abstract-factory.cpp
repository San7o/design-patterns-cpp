//////////////////////////////////////////////////////////////////////
// SPDX-License-Identifier: MIT
// Author:  Giovanni Santini
// Mail:    giovanni.santini@proton.me
// Github:  @San7o
//
// abstract-factory
// ================
//
// An Abstract Factory is a design pattern that provides an interface
// for creating families of related objects — without specifying their
// concrete classes.
// 
// It lets you create objects that belong together (e.g. Windows
// buttons and checkboxes vs. Mac buttons and checkboxes) — without
// hardcoding which “family” (Windows/Mac) you’re using.
//
// It is similar to the factory-method design pattern, but the factory
// can create a family of products.
//
#include <iostream>
#include <memory>

// Button base class
class Button {
public:
  // The destructor needs to be virtual too in order to destroy
  // derived classes
  //
  // Tip: never call virtual functions from constructors or
  // destructors, this is because the virtual function in a base class
  // constructor will use the base class since derived classes haven't
  // been created yet. Similarly, destructors will always resolve to
  // the Base class version of the function which may not be the
  // correct destructor.
  virtual ~Button(){};
  virtual void click(void) = 0;
};

// Button variants
class WinButton : public Button {
  void click(void) override {
    std::cout << "WinClick\n";
  }
};
class MacButton : public Button {
  void click(void) override {
    std::cout << "MacClick\n";
  }
};

// Checkbox base class
class Checkbox {
public:
  virtual ~Checkbox(){};
  virtual void check(void) = 0;
};

// Checkbox variants
class WinCheckbox : public Checkbox {
  void check(void) override {
    std::cout << "WinCheck\n";
  }
};
class MacCheckbox : public Checkbox {
  void check(void) override {
    std::cout << "MacCheck\n";
  }
};

// Factory interface, used to create buttons and checkboxes
//
// The user will only interact with this base class, and with the
// base classes of the products (buttons and checkboxes) using a
// variant interface.
class GUIFactory {
public:
  virtual ~GUIFactory(){};
  virtual std::unique_ptr<Button> createButton(void) = 0;
  virtual std::unique_ptr<Checkbox> createCheckbox(void) = 0;
};

// Factory implementations / variants
class WinFactory : public GUIFactory {
  std::unique_ptr<Button> createButton(void) override {
    return std::make_unique<WinButton>();
  }
  std::unique_ptr<Checkbox> createCheckbox(void) override {
    return std::make_unique<WinCheckbox>();
  }
};
class MacFactory : public GUIFactory {
  std::unique_ptr<Button> createButton(void) override {
    return std::make_unique<MacButton>();
  }
  std::unique_ptr<Checkbox> createCheckbox(void) override {
    return std::make_unique<MacCheckbox>();
  }
};

int main(void)
{
  {
    // Use the windows instance of the factory to get windows products
    WinFactory win;

    // Here we want the user to be abstracted away from the
    // implementation details of the products (Button,
    // Checkbox...). Each type of factory will create different
    // products, but the user will interact only with the base
    // classes of these.
    GUIFactory *factory = &win;
    std::unique_ptr<Button> b = factory->createButton();
    std::unique_ptr<Checkbox> c = factory->createCheckbox();

    b->click();
    c->check();
  }
  
  {
    MacFactory mac;
    
    // Word with the base class
    GUIFactory *factory = &mac;

    // Here is exactly the same as before
    std::unique_ptr<Button> b = factory->createButton();
    std::unique_ptr<Checkbox> c = factory->createCheckbox();

    b->click();
    c->check();
  }
  return 0;
}
