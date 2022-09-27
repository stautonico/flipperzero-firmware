FORCE_COMMON_ALLOCATION

SECTIONS
{
	.text 0x00000000 : ALIGN(4)
	{
		*(.text)
		*(.stub)
		*(.text*)
		*(.text.*)
		*(.text._*)

		KEEP (*(.init))
		KEEP (*(.fini))
	}

	.rodata :
	{
		*(.rodata)
		*(.rodata1)
		*(.rodata.*)
	}

	.data :
	{
		*(.data)
		*(.data1)
		*(.data.*)
	}


	.bss :
	{
		*(.bss)
		*(.bss*)
		*(.sbss)
		*(.sbss*)
		*(COMMON)
	}

<<<<<<< HEAD
	.ARM.attributes : 
	{
		*(.ARM.attributes)
		*(.ARM.attributes.*)	
	}

||||||| [FL-2764] SubGhz: fix CAME, Chamberlain potocol (#1650)
=======
	.ARM.attributes :
	{
		*(.ARM.attributes)
		*(.ARM.attributes.*)
	}

>>>>>>> unleashed
	/DISCARD/ :
	{
		*(.comment)
		*(.comment.*)
		*(.llvmbc)
		*(.llvmcmd)
	}
}
