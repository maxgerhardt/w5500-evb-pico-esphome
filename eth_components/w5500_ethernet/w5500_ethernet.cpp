
#include "w5500_ethernet.h"
#include "esphome/core/hal.h"
#include "esphome/core/helpers.h"
#include "esphome/core/log.h"
#include <Arduino.h>
#include <W5500lwIP.h>
#define WIZNET_RSTn 20
#define WIZNET_CS   17
#define WIZNET_INT  21
Wiznet5500lwIP eth(WIZNET_CS /* chip select */, SPI, WIZNET_INT /* interrupt */);

namespace esphome {
namespace w5500_ethernet {

static const char *const TAG = "w5500_ethernet";
static unsigned long last_print_time = 0;

W5500EthernetComponent *global_eth_component;  // NOLINT(cppcoreguidelines-avoid-non-const-global-variables)

W5500EthernetComponent::W5500EthernetComponent() {
  global_eth_component = this;
}

float W5500EthernetComponent::get_setup_priority() const {
  return esphome::setup_priority::ETHERNET;
}

void W5500EthernetComponent::setup() {
  ESP_LOGCONFIG(TAG, "Setting up W5500 Ethernet...");

  // Set up SPI pinout to match your HW
  SPI.setRX(16);
  SPI.setCS(17);
  SPI.setSCK(18);
  SPI.setTX(19);

  // get the W5500 chip out of reset
  pinMode(WIZNET_RSTn, OUTPUT);
  digitalWrite(WIZNET_RSTn, LOW);
  delayMicroseconds(500);
  digitalWrite(WIZNET_RSTn, HIGH);
  delay(200);

  // Start the Ethernet port
  if (!eth.begin()) {
    ESP_LOGCONFIG(TAG,"No wired Ethernet hardware detected. Check pinouts, wiring.");
    this->mark_failed();
  }
}

void W5500EthernetComponent::loop() {
  if ((millis() - last_print_time) > 1000) {
    last_print_time = millis();
    ESP_LOGCONFIG(TAG, "Looping W5500 Ethernet...");
    ESP_LOGCONFIG(TAG, 
      "W5500 IP address: %s Has link: %d Free Heap %d", 
      eth.localIP().toString().c_str(),
      (int) eth.isLinked(),
      rp2040.getFreeHeap()
    );
  }
}

void W5500EthernetComponent::dump_config() {
  ESP_LOGCONFIG(TAG, "W5500 Ethernet config");
}

bool W5500EthernetComponent::is_connected() {
  return eth.connected() && eth.isLinked();
}

network::IPAddress W5500EthernetComponent::get_ip_address() {
  return network::IPAddress(eth.localIP());
}

}  // namespace w5500_ethernet
}  // namespace esphome
