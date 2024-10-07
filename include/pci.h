#pragma once
#include <stdint.h>
#include <sys/pci.h>

#define PCI_ADDR(bus,slot,func) (((bus)<<8)|PCI_DEVFN((slot),(func)))

uint32_t pci_readl(uint16_t addr, uint8_t offset);
#define pci_readw(addr,offset) ((pci_readl((addr),(offset))>>(8*((offset)&2)))&0xFFFF)
#define pci_readb(addr,offset) ((pci_readl((addr),(offset))>>(8*((offset)&3)))&0xFF)
uint16_t pci_find_device(uint16_t vendor_id, uint16_t device_id, uint16_t subvendor, uint16_t subdev);
