// SPDX-License-Identifier: MIT
// Author:  Giovanni Santini
// Mail:    giovanni.santini@proton.me
// Github:  @San7o
//
// factory-method
// ==============
//
// Defines an interface for creating a product, but lets subclasses
// decide which concrete class to instantiate. This allows a class to
// defer instantiation to its subclasses while working with abstract
// interfaces.
//

#include <iostream>
#include <memory>

// Product interface
class Button {
public:
  // Tip: base class destructors should be virtual
  virtual ~Button() = default;
  virtual void click() = 0;
};

// Product implementations
class WinButton : public Button {
public:
  void click() override {
    std::cout << "WinButton click\n";
  }
};
class LinuxButton : public Button {
public:
  void click() override {
    std::cout << "LinuxButton click\n";
  }
};

// Factory interface
class Dialog {
public:
  virtual ~Dialog() = default;
  // Returns a button, instantiated depending on the specific factory
  // implementation
  virtual std::unique_ptr<Button> createButton() = 0;
};

// Factory implementations
class WinDialog : public Dialog {
public:
  std::unique_ptr<Button> createButton() override {
    return std::make_unique<WinButton>();
  }
};
class LinuxDialog : public Dialog {
public:
  std::unique_ptr<Button> createButton() override {
    return std::make_unique<LinuxButton>();
  }
};

int main(void)
{
  {
    WinDialog win_dialog;

    // Use a base dialog and button classes, regardless of the
    // implementation. From this point, the code is the same for all
    // implementations.
    Dialog* dialog = &win_dialog;
    std::unique_ptr<Button> button = dialog->createButton();
    button->click();
  }

  {
    LinuxDialog linux_dialog;
    Dialog* dialog = &linux_dialog;
    std::unique_ptr<Button> button = dialog->createButton();
    button->click();
  }
  return 0;
}
