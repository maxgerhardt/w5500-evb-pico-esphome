#pragma once

#include "esphome/core/component.h"
#include "esphome/components/network/ip_address.h"

namespace esphome {
namespace w5500_ethernet {

class W5500EthernetComponent : public Component {
 public:
  W5500EthernetComponent();
  ~W5500EthernetComponent() = default;
  void setup() override;
  void loop() override;
  void dump_config() override;
  float get_setup_priority() const override;
  void set_use_address(const std::string &use_address) { this->use_address_ = use_address; }
  bool is_connected();
  network::IPAddress get_ip_address();

 protected:
   std::string use_address_;
};

extern W5500EthernetComponent* global_eth_component;

}  // namespace am2320
}  // namespace w5500_ethernet
