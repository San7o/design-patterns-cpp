//////////////////////////////////////////////////////////////////////
// SPDX-License-Identifier: MIT
// Author:  Giovanni Santini
// Mail:    giovanni.santini@proton.me
// Github:  @San7o
//
// bridge
// ======
//
// The Bridge pattern decouples an abstraction from its implementation
// so that the two can vary independently. In this example, the
// `Remote` class (abstraction) controls a `DeviceI` implementation
// (such as `Radio`) through a common interface. This allows new
// remotes and devices to be developed and extended without modifying
// each other.
//

#include <iostream>
#include <memory>
#include <cassert>

// Implementation
class DeviceI {
public:
  virtual ~DeviceI() {}
  virtual bool IsEnabled() = 0;
  virtual void Enable() = 0;
  virtual void Disable() = 0;
  virtual float GetVolume() = 0;
  virtual void SetVolume(float percent) = 0;
  virtual int GetChannel() = 0;
  virtual void SetChannel(int channel) = 0;
};

class Radio : public DeviceI {
protected:
  bool enabled;
  float volume;
  int channel;
  
public:
  bool IsEnabled() { return this->enabled; }
  void Enable() { this->enabled = true; }
  void Disable() { this->enabled = false; }
  float GetVolume() { return this->volume; }
  void SetVolume(float percent) { this->volume = percent; };
  int GetChannel() { return this->channel; }
  void SetChannel(int channel) { this->channel = channel; }
};

// Abstraction
class Remote {
protected:
  // Wraps the implementation
  std::unique_ptr<DeviceI> device;
  
public:
  Remote(std::unique_ptr<DeviceI> device) {
    this->device = std::move(device);
  }
  ~Remote() {}
  void togglePower() {
    if (device->IsEnabled())
      device->Disable();
    else
      device->Enable();
  }

  void VolumeDown() {
    float volume = this->device->GetVolume();
    this->device->SetVolume(volume - 1.0f);
  }

  void VolumeUp() {
    float volume = this->device->GetVolume();
    this->device->SetVolume(volume + 1.0f);
  }

  void ChannelDown() {
    int channel = this->device->GetChannel();
    this->device->SetChannel(channel - 1);
  }

  void ChannelUp() {
    int channel = this->device->GetChannel();
    this->device->SetChannel(channel + 1);
  }
};

int main(void)
{
  std::unique_ptr<DeviceI> radio = std::make_unique<Radio>();
  Remote remote = Remote(std::move(radio));
  remote.VolumeUp();
  remote.ChannelUp();
  return 0;
}
