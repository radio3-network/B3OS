/* The following lines define the memory regions and their sizes */
MEMORY
{
  dram0_0_seg : org = 0x3F800000, len = 320K
  iram1_0_seg : org = 0x40100000, len = 96K
  irom0_0_seg : org = 0x40200000, len = 1M
}

/* The following lines define the sections of the program and the memory regions they belong to */
SECTIONS
{
  .dram0.bss :
  {
    /* The following line specifies the memory region for this section */
    *(.dram0.bss)
    /* The following line specifies that this section should be aligned to a 4-byte boundary */
    . = ALIGN(4)
  } > dram0_0_seg
}
