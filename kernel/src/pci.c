#include <stdbool.h>
#include <sys/io.h>
#include <pci.h>

#define CONFIG_ADDR_PORT 0xCF8
#define CONFIG_DATA_PORT 0xCFC

uint32_t
pci_readl(uint16_t addr, uint8_t offset) {
  outl((0x80<<24)|(addr<<8)|(offset&0xFC), CONFIG_ADDR_PORT);
  return inl(CONFIG_DATA_PORT);
}

uint16_t
pci_bus_find_device(uint8_t bus, uint16_t vendor_id, uint16_t device_id, uint16_t subvendor, uint16_t subdev) {
  for (uint8_t slot=0; slot<32; ++slot) {
    if (pci_readw(PCI_ADDR(bus, slot, 0), PCI_VENDOR_ID)==0xFFFF)
      continue;

    uint8_t max_fn = (pci_readb(PCI_ADDR(bus, slot, 0), PCI_HEADER_TYPE) & 0x80)?7:0;
    for (uint8_t fn=0; fn<=max_fn; ++fn) {
      if (pci_readw(PCI_ADDR(bus, slot, fn), PCI_VENDOR_ID)==0xFFFF)
        continue;

      uint16_t addr = PCI_ADDR(bus, slot, fn);
      uint8_t header_type = pci_readb(addr, PCI_HEADER_TYPE) & 0x7F;
      if (header_type == 1) {
        uint8_t subbus = pci_readb(addr, PCI_SECONDARY_BUS);
        addr = pci_bus_find_device(subbus, vendor_id, device_id, subvendor, subdev);
        if (addr != 0)
          return addr;
      }

      if (header_type != 0)
        continue;

      if (pci_readw(addr, PCI_VENDOR_ID) != vendor_id)
        continue;
      if (pci_readw(addr, PCI_DEVICE_ID) != device_id)
        continue;
      if (pci_readw(addr, PCI_SUBSYSTEM_VENDOR_ID) != subvendor)
        continue;
      if (pci_readw(addr, PCI_SUBSYSTEM_ID) != subdev)
        continue;

      return addr;
    }
  }
}

uint16_t
pci_find_device(uint16_t vendor_id, uint16_t device_id, uint16_t subvendor, uint16_t subdev) {
  return pci_bus_find_device(0, vendor_id, device_id, subvendor, subdev);
}
