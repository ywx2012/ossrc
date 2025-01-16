#pragma once

#include <stdint.h>

#define PCI_VENDOR_ID 0x00
#define PCI_DEVICE_ID 0x02
#define PCI_HEADER_TYPE 0x0e
#define PCI_BASE_ADDRESS_0 0x10
#define PCI_SUBSYSTEM_VENDOR_ID 0x2c
#define PCI_SUBSYSTEM_ID 0x2e

#define PCI_DEVFN(slot, func)   ((((slot) & 0x1f) << 3) | ((func) & 0x07))

#define PCI_ADDR(bus,slot,func) (((uint16_t)((bus)<<8))|(PCI_DEVFN((slot),(func))))
#define pci_readw(addr,offset) ((pci_readl((addr),(offset))>>(8*((offset)&2)))&0xFFFF)
#define pci_readb(addr,offset) ((pci_readl((addr),(offset))>>(8*((offset)&3)))&0xFF)
uint32_t pci_readl(uint16_t addr, uint8_t offset);
uint16_t pci_find_device(uint16_t vendor_id, uint16_t device_id, uint16_t subvendor, uint16_t subdev);
