//////////////////////////////////////////////////////////////////////
// SPDX-License-Identifier: MIT
// Author:  Giovanni Santini
// Mail:    giovanni.santini@proton.me
// Github:  @San7o
//
// adapter
// =======
//
// The Adapter pattern allows objects with incompatible interfaces to
// work together.  It acts as a bridge between a class that provides
// functionality (the Adaptee) and code that expects a different
// interface (the Target).
//
// In this example:
// - `CaptureDataFloat` is the Target interface expected by the client.
// - `CaptureDataInt` is the Adaptee providing data as an integer.
// - `CaptureDataIntAdapter` and `CaptureDataIntAdapter2` are Adapters
//   that convert integer data into float data, making `CaptureDataInt`
//   compatible with client code that expects `CaptureDataFloat`.
//

#include <iostream>
#include <memory>

// Target interface
// This is used by the client code, which expects a float number
class CaptureDataFloat {
public:
  virtual ~CaptureDataFloat() {}
  virtual float GetDataFloat() {
    return 123.5;
  }
};

// Adaptee, contains some sueful behaviour but its interface is
// incompatible with the existing client code
class CaptureDataInt {
public:
  virtual ~CaptureDataInt() {}
  virtual int GetDataInt() {
    return 1337;
  };
};

// Adapter
// Makes the Service interface compatible with the Target interface.
class CaptureDataIntAdapter : public CaptureDataFloat {
protected:
  
  // The adapter wraos the service
  std::unique_ptr<CaptureDataInt> adaptee;
  
public:
  CaptureDataIntAdapter(std::unique_ptr<CaptureDataInt> adaptee) {
    this->adaptee = std::move(adaptee);
  }
  
  float GetDataFloat() override {
    int x = adaptee->GetDataInt();
    float f = this->int2float(x);
    return f;
  }
  
private:
  float int2float(int x) {
    return (float) x;
  }
};

//
// Implementation with multiple inheritance
//

class CaptureDataIntAdapter2 : public CaptureDataInt, public CaptureDataFloat {
public:
  float GetDataFloat() override {
    int x = this->GetDataInt();
    float f = this->int2float(x);
    return f;
  }

private:
  float int2float(int x) {
    return (float) x;
  }
};

int main(void)
{
  // Here the cliend understands only float, we it cannot use
  // CaptureDataInt. We need an adapter.
  std::unique_ptr<CaptureDataInt> service = std::make_unique<CaptureDataInt>();
  CaptureDataIntAdapter adapter = CaptureDataIntAdapter(std::move(service));
  [[maybe_unused]] float x = adapter.GetDataFloat();
  std::cout << "x: " << x << "\n";
  
  return 0;
}
