//////////////////////////////////////////////////////////////////////
// SPDX-License-Identifier: MIT
// Author:  Giovanni Santini
// Mail:    giovanni.santini@proton.me
// Github:  @San7o
//
// composite
// =========
//
// The Composite pattern is a structural design pattern that allows
// you to treat individual objects and compositions of objects
// uniformly. It represents part–whole hierarchies where both simple
// (Leaf) and complex (Composite) elements share a common interface.
//
// Sadly, I cannot make this work with a decent API without using raw
// pointers. A big problem is that operators and polymorphism do not
// fit together nicely: I cannot really compare two different classes
// that inherit a certain class unless I have something to identify
// them in the base class. This is obvious because `operator==(Base&
// other)` accepts the base class type, and even if I make it virtual
// it cannot access the child classes.
//
// In this example:
//  - The `Graphic` class defines a common interface for all drawable
//    objects.
//  - `Leaf` classes like `Dot` and `Circle` represent simple graphics.
//  - `CompoundGraphic` (the Composite) contains and manages child
//     `Graphic` objects, allowing complex graphics to be built from
//     simpler ones.
//
//

#include <iostream>
#include <memory>
#include <variant>
#include <list>

class Leaf;
class Compound;

// Abstract class
class Graphic {
public:
  virtual ~Graphic() {}
  virtual void move(int x, int y) = 0;
  virtual void draw() = 0;
  virtual std::variant<Leaf*, Compound*> Type() = 0;
};

class Leaf : public Graphic {
public:
  virtual ~Leaf() {}
  std::variant<Leaf*, Compound*> Type() override {
    return this;
  };
};
  
class Compound : public Graphic {
public:
  virtual ~Compound() {}
  virtual void add(std::unique_ptr<Graphic> child) = 0;
  virtual void remove(Graphic* child) = 0;
  std::variant<Leaf*, Compound*> Type() override {
    return this;
  };

protected:
  std::list<std::unique_ptr<Graphic>> children;
};

class Dot : public Leaf {
public:
  Dot(int x, int y) {
    this->x = x;
    this->y = y;
  }

  void move(int x, int y) override {
    this->x = x;
    this->y = y;
  }

  void draw() override {
    std::cout << "Drawing Dot, x: " << this->x
              << ", y: " << this->y << "\n";
  }
  
protected:
  int x;
  int y;
};

class Circle: public Dot {
public:
  Circle(int x, int y, int radius) : Dot(x, y) {
    this->radius = radius;
  }

  void draw() override {
    std::cout << "Drawing Circle, x: " << this->x
              << ", y: " << this->y << "\n";
  }

protected:
  int radius;
};

class CompoundGraphic : public Compound {
public:
  CompoundGraphic() = default;
  
  void add(std::unique_ptr<Graphic> child) override {
    this->children.push_back(std::move(child));
  }

  void remove(Graphic* child) override {
    for (auto it = children.begin(); it != children.end(); ++it) {
      if (it->get() == child) {
        children.erase(it);
        return;
      }
    }
  }
  
  void move(int x, int y) override {
    for (auto& c : this->children) {
      c->move(x, y);
    }
  }

  void draw() override {
    for (auto& c : this->children) {
      c->draw();
    }
  }
};

int main(void)
{
  CompoundGraphic root;

  auto d = std::make_unique<Dot>(10, 20);

  Dot* raw_d = d.get();
  root.add(std::move(d));
  root.add(std::make_unique<Circle>(5, 5, 3));

  std::cout << "Before draw:\n";
  root.draw();

  root.remove(raw_d);

  std::cout << "After removal:\n";
  root.draw();

  return 0;
}
