# 《穿越操作系统迷雾》源码

经作者同意，上传到GitHub。仅供学习交流使用，分享转载请保留作者版权声明。

可以前往[豆瓣](https://book.douban.com/subject/36560814/)和作者讨论交流。

## 截图

![screenshot](screenshot.png)

## 仓库说明

Tags下面放的是完全没有任何改动的纯净版原始源码。

`main` 分支如无必要，不对原始代码进行任何改动。

## 改动

### 支持Linux/x86 Boot Protocol

kvmtool支持一种独特的加载方式，当检测不出文件格式时，会把内核文件内容直接加载到0x10000处。原始代码只支持这一种加载方式，离开了kvmtool就无法运行。这里对原始代码进行了一些的改动，以便支持Linux/x86 Boot Protocol。这样就不会报Warning: Kernel image is not a bzImage了，也方便后续支持在QEMU下运行。

因为Linux/x86 Boot Protocol会把保护模式代码加载到0x100000。因此需要把start32改到这个位置，同时因为在原始代码中这是start64所在位置，先把start64挪到0x200000。

原始代码中，跳往start64时，并没有跳到0xffffffff80100000这个地址，而是跳到了0x100000。因此，在0x0和0xffffffff80000000开始的地址空间内都映射了内核。不知道出于什么原因，0x0处仅映射了2MB，而0xffffffff80000000处映射了KERNEL_PAGE_NUM页。为方便起见，这里就把原始代码 .org 0x33000 处第一项直接指向 pml2 。

因为pml2内容会由前面的代码填充，故一开始全填零即可。原始代码在这里填了第一项。

因为vesa_mode_info固定在0x4000，且有256个字节了，因此setup_sects为0x20，(0x4200-0x200)/512。

### 支持QEMU

参考[INT 15H, E820H](https://uefi.org/htmlspecs/ACPI_Spec_6_4_html/15_System_Address_Map_Interfaces/int-15h-e820h---query-system-address-map.html)，查询系统地址区间时需要设置EDX为0x0534D4150。原始代码漏了这行，倒也不影响在kvmtool上运行。

因为QEMU不支持VESA，参考[VBE Display API](http://cvs.savannah.nongnu.org/viewvc/*checkout*/vgabios/vgabios/vbe_display_api.txt?revision=1.14)，改成[QEMU Standard VGA](https://www.qemu.org/docs/master/specs/standard-vga.html)获取framebuffer地址。并强制设置成640x480x32。

因为QEMU默认内存大小为128M，而原始代码app1和app2物理地址起始位置分别为0xc800000和0xd000000。分别改成0x1000000和0x2000000。
