
Solutions for IDA Pro
=====================

Since we cannot assemble/disassemble code in IDA Pro itself, we have to use another tool. To make this easier, we just one an [online assembler/disassembler](https://defuse.ca/online-x86-assembler.htm)

Exercise 1
----------

Since there is no way to view in IDA Pro what processors/architectures are supported directly, we have to look at the [homepage](https://www.hex-rays.com/products/ida/processors.shtml):

```
IDA Starter Edition

IDA Starter Edition supports the following families (64-bit analysis is possible only with IDA Professional Edition)

AMD K6-2 3D-Now! extensions
32-bit ARM Architecture versions from v3 to v8 including Thumb, Thumb-2, DSP instructions and NEON Advanced SIMD instructions.
ARMv4/ARMv4T: ARM7 cores (ARM7TDMI/ARM710T/ARM720T/ARM740T), ARM9 cores (ARM9TDMI/ARM920T/ARM922T/ARM940T)
ARMv5/ARMv5TE/ARMv5TEJ: ARM9 cores (ARM946E-S/ARM966E-S/ARM968E-S/ARM926EJ-S/ARM996HS), ARM10E (ARM1020E/ARM1022E/ARM1026EJ-S)
ARMv6/ARMv6T2/ARMv6Z/ARMv6K: ARM11 cores (ARM1136J(F)-S/ARM1156T2(F)-S/ARM1176JZ(F)-S/ARM11 MPCore)
ARMv6-M: Cortex-M0/Cortex-M0+/Cortex-M1 (e.g. NXP LPC800/LPC1xxx, Freescale Kinetis L and M series, STM32 F0 series etc.)
ARMv7-M: Cortex-M3 (e.g. NXP LPC17xx/18xx/13xx, STM32 F1/F2/L1 series, TI Stellaris, Toshiba TX03/TMPM3xx etc.)
ARMv7E-M: Cortex-M4 (e.g. NXP LPC43xx, STM32 F3/F4 series, TI Stellaris LM4F, Freescale Kinetis K series and W series, Atmel AT91SAM4 etc.)
ARMv7-R: Cortex-R4(F)/Cortex-R5/Cortex-R7 (e.g. TI TMS570LS etc.)
ARMv7-A: Cortex-A5/Cortex-A7/Cortex-A8/Cortex-A9/Cortex-A12/Cortex-A15 (e.g. TI Sitara, TI OMAP series, Samsung S5PC100 and Exynos, Nvidia Tegra, Freescale i.MX, Allwinner A-Series and many others)
ARMv7 (custom): Apple A4/A5/A5X/A6/A6X (Swift microarchitecture, used in Apple's iPhone/iPod/iPad/AppleTV), Qualcomm Snapdragon
Note: this list is incomplete; code for any ARM-compliant core can be disassembled
ATMEL AVR (comes with source code)
DEC PDP-11(comes with source code)
Fujitsu FR (comes with source code)
GameBoy
Hitachi/Renesas H8/300, H8/300L, H8/300H, H8S/2000, H8S/2600, H8SX (comes with source code)
Hitachi H8/500(comes with source code)
Hitachi HD 6301, HD 6303, Hitachi HD 64180
INTEL 8080
INTEL 8085
INTEL 80196 (comes with source code)
INTEL 8051 (comes with source code)
INTEL 860XR (comes with source code)
INTEL 960 (comes with source code)
INTEL 80x86 and 80x87
INTEL Pentium family, including SSE, SSE2, SSE3, SSE4
Java Virtual Machine (comes with source code)
KR1878 (comes with source code)
Microsoft .NET (Common Language Infrastructure bytecode)
Mitsubishi MELPS740 or Renesas 740 (comes with source code)
Hitachi/Renesas M16C
MN102 (comes only with source code)
MOS Technologies 6502 (comes with source code)
Motorola/Freescale MC680xx, CPU32 (68330), MC6301, MC6303, MC6800, MC6801, MC6803, MC6805, MC6808, HCS08, MC6809, MC6811, M68H12C, ColdFire
Motorola MC6812/MC68HC12/CPU12
Freescale HCS12, HCS12X (including XGATE coprocessor)
NSC CR16 (comes only with source code)
NEC V850 and V850E1 (V850ES) (comes with source code)
EFI Byte Code (EBC) (comes with source code)
SPU (Synergistic Processing Unit of the Cell BE) (comes with source code)
MSP430, MSP430X (comes with source code)
PIC 12XX, PIC 14XX, PIC 18XX, PIC 16XXX (comes with source code)
Rockwell C39 (comes only with source code)
SAM8 (comes with source code)
SGS Thomson ST-7, and ST-20 (comes with source code)
TLCS900 (comes only with source code)
unSP from SunPlus
Philips XA series (51XA G3)(comes with source code)
Intel xScale
Z80, Zilog Z8, Zilog Z180, Zilog Z380 (comes with source code)
IDA Professional Edition

IDA Professional Edition supports all the Starter processors listed above plus the more complex ones listed below.

Analysis of 64 bit programs is possible with the IDA Professional.

x64 architecture (Intel x64 and AMD64)
ARM64 Architecture (aka AArch64)
ARMv8-A: Cortex-A50/Cortex-A53/Cortex-A57
ARMv8 (custom): Apple A7 (Cyclone microarchitecture, used in iPhone 5s)
Analog Devices AD218x series (ADSP-2181, ADSP-2183, ADSP-2184(L/N), ADSP-2185(L/M/N), ADSP-2186(L/M/N), ADSP-2187(L/N), ADSP-2188M/N, ADSP-2189M/N)
Dalvik (Android bytecode, DEX)
DEC Alpha
DSP563xx, DSP566xx, DSP561XX (comes with source code)
TI TMS320C2X, TMS320C5X, TMS320C6X, TMS320C64X, TMS 320C54xx, TMS320C55xx, TMS320C3 (comes with source code)
TI TMS320C27x/TMS320C28x
Hewlett-Packard HP-PA (comes with source code)
Hitachi/Renesas SuperH series: SH1, SH2, SH3, Hitachi SH4 (Dreamcast), SH-4A, SH-2A, SH2A-FPU
IBM/Motorola PowerPC/POWER architecture, including Power ISA extensions:
Book E (Embedded Controller Instructions)
Freescale ISA extentions (isel etc.)
SPE (Signal Processing Engine) instructions
AltiVec (SIMD) instructions
Hypervisor and virtualization instructions
All instructions from the Power ISA 2.06 specification (Vector, Decimal Floating Point, Integer Multiply-Accumulate, VSX etc.)
Cell BE (Broadband Engine) instructions (used in PlayStation 3)
VLE (Variable Length Encoding) compressed instruction set
Xenon (Xbox 360) instructions, including VMX128 extension
Paired Single SIMD instructions (PowerPC 750CL/Gekko/Broadway/Espresso, used in Nintendo Wii and WiiU)
Motorola/Freescale PowerPC-based cores and processors, including (but not limited to):
MPC5xx series: MPC533/MPC535/MPC555/MPC556/MPC561/MPC562/MPC563/MPC564/MPC566
Note: code compression features of MPC534/MPC564/MPC556/MPC566 (Burst Buffer Controller) are currently not supported
MPC8xx series (PowerQUICC): MPC821/MPC850/MPC860
MPC8xxx series (PowerQUICC II, PowerQUICC II Pro, PowerQUICC III): MPC82xx/MPC83xx/MPC85xx/MPC87xx
MPC5xxx series (Qorivva): MPC55xx, MPC56xx, MPC57xx
Power PC 4xx, 6xx, 74xx, e200 (including e200z0 with VLE), e500 (including e500v1, e500v2 and e500mc), e600, e700, e5500, e6500 cores
QorIQ series: P1, P2, P3, P4, P5 and T1, T2, T4 families
Infineon Tricore architecture (up to architecture v1.6)
Intel IA-64 Architecture - Itanium.
Motorola DSP 56K
Motorola MC6816
MIPS
MIPS Mark I (R2000)
MIPS Mark II (R3000)
MIPS Mark III: (R4000, R4200, R4300, R4400, and R4600)
MIPS Mark IV: R8000, R10000, R5900 (Playstation 2)
MIPS32, MIPS32r2, MIPS32r3 and MIPS64, MIPS64r2, MIPS64r3
Allegrex CPU (Playstation Portable), including VFPU instructions
Cavium Octeon ISA extensions
MIPS16 (MIPS16e) Application Specific Extension
MIPS-MT, MIPS-3D, smartMIPS Application Specific Extensions
Toshiba TX19/TX19A Family Application Specific Extension (MIPS16e+ aka MIPS16e-TX)
Mitsubishi M32R(comes with source code)
Mitsubishi M7700 (comes with source code)
Mitsubishi M7900 (comes with source code)
Nec 78K0 and Nec 78K0S (comes with source code)
STMicroelectronics ST9+, ST-10 (comes with source code)
SPARCII, ULTRASPARC
Siemens C166 (flow)
Fujitsu F2MC-16L, Fujitsu F2MC-LC (comes with source code)
```

Please note that the free version and the demo version are only supporting some of them.

Exercise 2
----------

This is a very simple task using the online tool, just input the code and select the wanted architecture, then press 'Assemble'.

```
Raw Hex (zero bytes in bold): B8EFBEADDE   

0:  b8 ef be ad de          mov    eax,0xdeadbeef
```

```
Raw Hex (zero bytes in bold): 5053585B   

0:  50                      push   eax
1:  53                      push   ebx
2:  58                      pop    eax
3:  5b                      pop    ebx
```

Exercise 3
----------

Similar to exercise 2, just input the hex-values and then disassemble it:

```
0:  b4 00                   mov    ah,0x0
2:  b0 13                   mov    al,0x13
4:  cd 10                   int    0x10
```