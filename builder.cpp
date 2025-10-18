//////////////////////////////////////////////////////////////////////
// SPDX-License-Identifier: MIT
// Author:  Giovanni Santini
// Mail:    giovanni.santini@proton.me
// Github:  @San7o
//
// builder
// =======
//
// The Builder Pattern separates the construction of a complex object
// from its representation, allowing the same construction process to
// create different object types or configurations via step-by-step
// construction.
//

#include <iostream>
#include <memory>
#include <cassert>

enum class Color {
  Red,
  Green,
  Blue,
  White,
  Black,
};

// Some objects we want to build
class Car {
public:
  int seats;
  int tires;
  Color color;
};
class Bike {
public:
  int seats;
  int tires;
  Color color;
};

// Builder interface
// This is used to abstract individual builders, but can be omitted
// if it is not required to have multiple products
class VehicleBuilder {
public:
  virtual ~VehicleBuilder(){};
  virtual void BuildSeats() = 0;
  virtual void BuildTires() = 0;
  virtual void BuildColor() = 0;
};

// Concrete builders
class CarBuilder : public VehicleBuilder {
private:
  Car car;
  
public:
  CarBuilder() {
    this->Reset();
  }

  void Reset() {
    this->car = Car{};
  }
  
  void BuildSeats() override {
    this->car.seats = 5;
  }
  
  void BuildTires() override {
    this->car.tires = 4;
  }

  void BuildColor() override {
    this->car.color = Color::Red;
  }

  Car Build() {
    Car result = std::move(this->car);
    this->Reset();
    return result;
  }
};

class BikeBuilder : public VehicleBuilder {
private:
  Bike bike;

public:
  BikeBuilder() {
    this->Reset();
  }

  void Reset() {
    this->bike = Bike{};
  }

  void BuildSeats() override {
    this->bike.seats = 1;
  }

  void BuildTires() override {
    this->bike.tires = 2;
  }

  void BuildColor() override {
    this->bike.color = Color::Red;
  }

  Bike Build() {
    Bike result = std::move(this->bike);
    this->Reset();
    return result;
  }
};

// Director, basically an hub for calling "default" builders
// This can also be omitted if unnecessary
class Director {
private:
  std::shared_ptr<VehicleBuilder> builder;
public:
  void BuildBuilder(std::shared_ptr<VehicleBuilder> builder) {
    this->builder = builder;
  }

  void BuildMinimalViableProduct() {
    this->builder->BuildSeats();
    this->builder->BuildTires();
  }

  void BuildFullFeaturedProduct() {
    this->builder->BuildSeats();
    this->builder->BuildTires();
    this->builder->BuildColor();
  }
};


//
// A simpler builder
// =================
//
// This is an alternative implementation that enables to set specific
// values for the object to build.
//
// This implementation does not work well if you want to generalize the
// builder with multiple products and it duplicates member variables,
// but it has the advantage that it constructs the product only once
// and it is simpler. If construction is expensive, this approach is
// better.
//

class Game {
public:
  int window_width;
  int window_height;
  bool game_ended;

  Game() = delete;
  Game(int window_width, int window_height, bool game_ended)
    : window_width(window_width), window_height(window_height), game_ended(game_ended)
  {
    // Tip: constructors and destructors should never have side
    // effects like IO, they should only bring the object into a valid
    // state. If the side-effect operation fails and throws and error,
    // the constructor will fail and the destructor will never be
    // called and potentially not clean resources.
    std::cout << "Game constructor called\n";
  }
  ~Game() {
    std::cout << "Game destructor called\n";
  }
  
  class Builder;
};


class Game::Builder {
private:
  int window_width;
  int window_height;
  bool game_ended;
  
public:
  Builder() {
    this->reset();
  }

  Builder& reset() {
    this->window_width = {};
    this->window_height = {};
    this->game_ended = {};
    return *this;
  }
    
  Builder& set_window_width(int window_width) {
    this->window_width = window_width;
    return *this;
  }

  Builder& set_window_height(int window_height) {
    this->window_height = window_height;
    return *this;
  }

  Builder& set_game_ended(bool game_ended) {
    this->game_ended = game_ended;
    return *this;
  }

  Game build() {
    Game g = Game(window_width, window_height, game_ended);
    this->reset();
    return g; // return with copy elision
  }
};

int main(void)
{
  // Build with director
  std::shared_ptr<Director> director = std::make_shared<Director>();
  std::shared_ptr<CarBuilder> builder = std::make_shared<CarBuilder>();
  director->BuildBuilder(builder);
  director->BuildMinimalViableProduct();
  [[maybe_unused]] Car car = builder->Build();
  assert(car.seats == 5);
  assert(car.tires == 4);

  // Simple builder
  BikeBuilder bb = BikeBuilder();
  bb.BuildSeats();
  bb.BuildTires();
  bb.BuildColor();
  [[maybe_unused]] Bike b = bb.Build();
  assert(b.seats == 1);
  assert(b.tires == 2);
  assert(b.color == Color::Red);

  // Alternative, simpler builder
  
  Game g = Game::Builder()
    .set_window_height(100)
    .set_window_width(200)
    .set_game_ended(false)
    .build();
  assert(g.window_height == 100);
  assert(g.window_width == 200);
  assert(g.game_ended == false);
  
  return 0;
}
