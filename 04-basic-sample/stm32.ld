_stack_base = 0x10002000;
_boot_checksum = 0 - (_stack_base + main + 1 + (hang + 1) * 5);

MEMORY
{
   rom(RX)   : ORIGIN = 0x00000000, LENGTH = 128K
   ram(WAIL) : ORIGIN = 0x20000000, LENGTH = 16K
}

SECTIONS
{
   .text : { *(.text*) } > rom
   .bss  : { *(.bss*) } > ram
}
