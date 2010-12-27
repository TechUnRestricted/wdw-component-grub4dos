/* common.c - miscellaneous shared variables and routines */
/*
 *  GRUB  --  GRand Unified Bootloader
 *  Copyright (C) 1999,2000,2001,2002,2004  Free Software Foundation, Inc.
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program; if not, write to the Free Software
 *  Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
 */

#include <shared.h>
#include <iso9660.h>
#include "pxe.h"

#ifdef SUPPORT_DISKLESS
# define GRUB	1
# include <etherboot.h>
#endif

/*
 *  Shared BIOS/boot data.
 */

#ifdef GRUB_UTIL
struct multiboot_info mbi;
unsigned long saved_drive;
unsigned long saved_partition;
#endif
char saved_dir[256];
#ifdef GRUB_UTIL
unsigned long force_cdrom_as_boot_device = 1;
//unsigned long cdrom_drives[8] = {0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF};
unsigned long cdrom_drive = GRUB_INVALID_DRIVE;
unsigned long ram_drive = 0x7F;	/* default is a floppy. */
unsigned long long rd_base = 0;	/* Note the rd_base value of -1 invalidates the ram drive. */
unsigned long long rd_size = 0x100000000ULL;	/* default is 4G */
unsigned long long saved_mem_higher = 0;
unsigned long saved_mem_upper = 0;
unsigned long saved_mmap_addr;
unsigned long saved_mmap_length = 0;
#endif
unsigned long saved_mem_lower;

#ifndef STAGE1_5
/* This saves the maximum size of extended memory (in KB).  */
unsigned long extended_memory;
unsigned long init_free_mem_start;
#endif
#ifdef GRUB_UTIL
int is64bit = 0;
grub_error_t errnum = ERR_NONE;
#endif
int errorcheck = 1;

/*
 *  Error code stuff.
 */

#ifndef STAGE1_5

char *err_list[] =
{
  [ERR_NONE] = 0,
  [ERR_BAD_ARGUMENT] = "Invalid argument",
  [ERR_BAD_FILENAME] = "Filename must be either an absolute pathname or blocklist",
  [ERR_BAD_FILETYPE] = "Bad file or directory type",
  [ERR_BAD_GZIP_DATA] = "Bad or corrupt data while decompressing file",
  [ERR_BAD_GZIP_HEADER] = "Bad or incompatible header in compressed file",
  [ERR_BAD_PART_TABLE] = "Partition table invalid or corrupt",
  [ERR_BAD_VERSION] = "Mismatched or corrupt version of stage1/stage2",
  [ERR_BELOW_1MB] = "Loading below 1MB is not supported",
  [ERR_BOOT_COMMAND] = "Kernel must be loaded before booting",
  [ERR_BOOT_FAILURE] = "Unknown boot failure",
  [ERR_BOOT_FEATURES] = "Unsupported Multiboot features requested",
  [ERR_DEV_FORMAT] = "Unrecognized device string, or you omitted the required DEVICE part which should lead the filename.",
  [ERR_DEV_NEED_INIT] = "Device not initialized yet",
  [ERR_DEV_VALUES] = "Invalid device requested",
  [ERR_EXEC_FORMAT] = "Invalid or unsupported executable format",
  [ERR_FILELENGTH] = "Filesystem compatibility error, cannot read whole file",
  [ERR_FILENAME_FORMAT] = "The leading DEVICE of the filename to find must be stripped off,\n\tand DIR for set-root must begin in a slash(/).",
  [ERR_FILE_NOT_FOUND] = "File not found",
  [ERR_FSYS_CORRUPT] = "Inconsistent filesystem structure",
  [ERR_FSYS_MOUNT] = "Cannot mount selected partition",
  [ERR_GEOM] = "Selected cylinder exceeds maximum supported by BIOS",
  [ERR_HD_VOL_START_0] = "The BPB hidden_sectors should not be zero for a hard-disk partition boot sector",
  [ERR_IN_SITU_FLOPPY] = "Only hard drives could be mapped in situ.",
  [ERR_IN_SITU_MEM] = "Should not use --mem together with --in-situ.",
  [ERR_NEED_LX_KERNEL] = "Linux kernel must be loaded before initrd",
  [ERR_NEED_MB_KERNEL] = "Multiboot kernel must be loaded before modules",
  [ERR_NO_DISK] = "Selected disk does not exist",
  [ERR_NO_DISK_SPACE] = "No spare sectors on the disk",
  [ERR_NO_PART] = "No such partition",
  [ERR_NO_HEADS] = "The number of heads must be specified. The `--heads=0' option tells map to choose a value(but maybe unsuitable) for you",
  [ERR_NO_SECTORS] = "The number of sectors per track must be specified. The `--sectors-per-track=0' option tells map to choose a value(but maybe unsuitable) for you",
  [ERR_NON_CONTIGUOUS] = "File for drive emulation must be in one contiguous disk area",
  [ERR_NUMBER_OVERFLOW] = "Overflow while parsing number",
  [ERR_NUMBER_PARSING] = "Error while parsing number",
  [ERR_OUTSIDE_PART] = "Attempt to access block outside partition",
  [ERR_PRIVILEGED] = "Must be authenticated",
  [ERR_READ] = "Disk read error",
  [ERR_SYMLINK_LOOP] = "Too many symbolic links",
  [ERR_UNALIGNED] = "File is not sector aligned",
  [ERR_UNRECOGNIZED] = "Unrecognized command",
  [ERR_WONT_FIT] = "Selected item cannot fit into memory",
  [ERR_WRITE] = "Disk write error",
  [ERR_INT13_ON_HOOK] = "The int13 handler already on hook",
  [ERR_INT13_OFF_HOOK] = "The int13 handler not yet on hook",
  [ERR_NO_DRIVE_MAPPED] = "Refuse to hook int13 because of empty drive map table",
  [ERR_INVALID_HEADS] = "Invalid heads. Should be between 0 and 256(0 means auto)",
  [ERR_INVALID_SECTORS] = "Invalid sectors. Should be between 0 and 63(0 means auto)",
  [ERR_SPECIFY_GEOM] = "Should not specify geometry when mapping a whole drive or when emulating a hard disk with a logical partition",
  [ERR_EXTENDED_PARTITION] = "Extended partition table is invalid, or its CHS values conflict with the BPB in a logical partition",
  [ERR_DEL_MEM_DRIVE] = "You should delete other mem drive first, or use `--mem' option to force the deletion",
  [ERR_SPECIFY_MEM] = "Should not specify `--mem' when mapping a whole drive",
  [ERR_SPECIFY_RESTRICTION] = "Options --read-only, --fake-write and --unsafe-boot are mutually exclusive. Should not specify them repeatedly.",
  [ERR_INVALID_FLOPPIES] = "Invalid floppies. Should be between 0 and 2",
  [ERR_INVALID_HARDDRIVES] = "Invalid harddrives. Should be between 0 and 127",
  [ERR_INVALID_LOAD_SEGMENT] = "Invalid load segment. Should be between 0 and 0x9FFF",
  [ERR_INVALID_LOAD_OFFSET] = "Invalid load offset. Should be between 0 and 0xF800",
  [ERR_INVALID_LOAD_LENGTH] = "Invalid load length. Should be between 512 and 0xA0000",
  [ERR_INVALID_SKIP_LENGTH] = "Invalid skip length. Should be less than the file size",
  [ERR_INVALID_BOOT_CS] = "Invalid boot CS. Should be between 0 and 0xFFFF",
  [ERR_INVALID_BOOT_IP] = "Invalid boot IP. Should be between 0 and 0xFFFF",
  [ERR_INVALID_RAM_DRIVE] = "Invalid ram_drive. Should be between 0 and 254",
//  [ERR_INVALID_RD_BASE] = "Invalid rd_base. Should not be 0xffffffff",
//  [ERR_INVALID_RD_SIZE] = "Invalid rd_size. Should not be 0",
  [ERR_MD_BASE] = "When mapping whole mem device at a fixed location, you must specify --mem to a value > 0.",
  [ERR_RD_BASE] = "RD_BASE must be sector-aligned and non-zero for mapping at a fixed location",
  [ERR_DOS_BACKUP] = "GRUB was not booted from DOS, or the backup copy of DOS at physical\naddress 0x200000 is corrupt",
  [ERR_ENABLE_A20] = "Failed to turn on Gate A20!",
  [ERR_DISABLE_A20] = "Failed to turn off Gate A20!",
  [ERR_DEFAULT_FILE] = "Invalid DEFAULT file format. Please copy a valid DEFAULT file from the grub4dos release and try again. Also note that the DEFAULT file must be uncompressed.",
  [ERR_PARTITION_TABLE_FULL] = "Cannot use --in-situ because the partition table is full(i.e., all the 4 entries are in use).",
  [ERR_MD5_FORMAT] = "Unrecognized md5 string. You must create it using the MD5CRYPT command.",
  [ERR_WRITE_GZIP_FILE] = "Attempt to write a gzip file",
  [ERR_FUNC_CALL] = "Invalid function call",
  [ERR_INTERNAL_CHECK] = "Internal check failed. Please report this bug.",
  [ERR_KERNEL_WITH_PROGRAM] = "Kernel cannot load if there is an active process",
  [ERR_HALT] = "Halt failed.",
  [ERR_PARTITION_LOOP] = "Too many partitions.",
//  [ERR_WRITE_TO_NON_MEM_DRIVE] = "Only RAM drives can be written when running in a script",
  [ERR_NOT_ENOUGH_MEMORY] = "Not enough memory",
  [ERR_BAT_GOTO] = "The syntax of GOTO is incorrect.",
  [ERR_BAT_CALL] = "The syntax of CALL is incorrect.",

};


/* static for BIOS memory map fakery */
static struct AddrRangeDesc fakemap[3] =
{
  {20, 0, 0, MB_ARD_MEMORY},
  {20, 0x100000, 0, MB_ARD_MEMORY},
  {20, 0x1000000, 0, MB_ARD_MEMORY}
};

/* A big problem is that the memory areas aren't guaranteed to be:
   (1) contiguous, (2) sorted in ascending order, or (3) non-overlapping.
   Thus this kludge.  */
static unsigned long long
mmap_avail_at (unsigned long long bottom)
{
  unsigned long long top;
  unsigned long addr;
  int cont;
  
  top = bottom;
  do
    {
      for (cont = 0, addr = saved_mmap_addr;
	   addr < saved_mmap_addr + saved_mmap_length;
	   addr += *((unsigned long *) addr) + 4)
	{
	  struct AddrRangeDesc *desc = (struct AddrRangeDesc *) addr;
	  
	  if (desc->Type == MB_ARD_MEMORY
	      && desc->BaseAddr <= top
	      && desc->BaseAddr + desc->Length > top)
	    {
	      top = desc->BaseAddr + desc->Length;
	      cont++;
	    }
	}
    }
  while (cont);

  /* For now, GRUB assumes 32bits addresses, so...  */
  if (top > 0x100000000ULL && bottom < 0x100000000ULL)
      top = 0x100000000ULL;
  
  return top - bottom;
}
#endif /* ! STAGE1_5 */

#ifndef GRUB_UTIL
#ifndef STAGE1_5
unsigned int
grub_sleep (unsigned int seconds)
{
	unsigned long long j;
	int time1;
	int time2;

	/* Get current time.  */
	while ((time2 = getrtsecs ()) == 0xFF);

	for (j = 0; seconds; j++)
	{
	  if ((time1 = getrtsecs ()) != time2 && time1 != 0xFF)
	    {
		time2 = time1;
		seconds--;
		j = 0;
		continue;
	    }
	  if (j == 0x100000000ULL)
	    {
		seconds--;
		j = 0;
		continue;
	    }
	}

	return seconds;
}

// check 64bit and PAE
// return value bit0=PAE supported bit1=AMD64/Intel64 supported
int check_64bit_and_PAE ()
{
    unsigned int has_cpuid_instruction;
    // check for CPUID instruction
    asm ( "pushfl; popl %%eax;"	// get original EFLAGS
	  "movl %%eax, %%edx;"
	  "xorl $(1<<21), %%eax;"
	  "pushl %%eax; popfl;"	// try flip bit 21 of EFLAGS
	  "pushfl; popl %%eax;"	// get the modified EFLAGS
	  "pushl %%edx; popfl;"	// restore original EFLAGS
	  "xorl %%edx, %0; shrl $21, %%eax; and $1, %%eax;"	// check for bit 21 difference
	: "=a"(has_cpuid_instruction) : : "%edx" );
    if (!has_cpuid_instruction)
	return 0;
    unsigned int maxfn,feature;
    int x=0;
    asm ("cpuid;" : "=a"(maxfn): "0"(0x00000000) : "%ebx","%ecx","%edx");
    if (maxfn >= 0x00000001)
    {
	asm ("cpuid;" : "=d" (feature) : "a" (1) : "%ebx", "%ecx");
	if (feature & (1<<6)) // PAE
	    x |= IS64BIT_PAE;
    }
    asm ("cpuid;" : "=a"(maxfn): "0"(0x80000000) : "%ebx","%ecx","%edx");
    if (maxfn >= 0x80000001)
    {
	asm ("cpuid;" : "=d" (feature) : "a" (0x80000001) : "%ebx", "%ecx");
	if (feature & (1<<29)) // AMD64, EM64T/IA-32e/Intel64
	    x |= IS64BIT_AMD64;
    }
    return x;
}

#endif /* ! STAGE1_5 */
#endif /* ! GRUB_UTIL */

void *grub_malloc(unsigned long size)
{
	struct malloc_array *p_memalloc_array = malloc_array_start;
	unsigned long alloc_mem = 0;
	size = (size + 0x1F) & ~0xf; //分配内存,16字节对齐,额外分配16字节.也就是说最少的内存分配是32字节.

	for ( ; p_memalloc_array->addr != free_mem_end; p_memalloc_array = p_memalloc_array->next)//find free mem array;
	{
		if (p_memalloc_array->addr & 1)//used mem
			continue;
		alloc_mem = p_memalloc_array->next->addr - p_memalloc_array->addr;
		if (alloc_mem < size)
		{
			continue;
		}

		if (alloc_mem > size) //add new array
		{
			struct malloc_array *P = malloc_array_start;
			for ( ; P->addr; P++)
			{
				if (P == (struct malloc_array *)mem_alloc_array_end)
				{
					errnum = ERR_WONT_FIT;
					return NULL;
				}
			}

			P->addr = p_memalloc_array->addr + size;
			P->next = p_memalloc_array->next;
			p_memalloc_array->next = P;
		}

		alloc_mem =  p_memalloc_array->addr;
		p_memalloc_array->addr |= 1;//set mem used

		return (void *)alloc_mem;
	}
	errnum = ERR_WONT_FIT;
	return NULL;
}

void grub_free(void *ptr)
{
	if (ptr == NULL)
		return;
	struct malloc_array *P = malloc_array_start;
	struct malloc_array *P1 = malloc_array_start;

	for (;P->addr != free_mem_end;P1 = P,P = P->next)
	{
		if ((P->addr & ~0xfUL) == (unsigned long)ptr)
		{
			P->addr &= ~0xfUL;//unused memory

			if (P1 != P && (P1->addr & 1) == 0)
			{//向前合并可用内存块.
				P1->next = P->next;
				P->addr = 0;
				P = P1;
			}

			P1 = P->next;
			if (P1->addr != free_mem_end && (P1->addr & 1) == 0)
			{//向后合并可用内存块.
				P1->addr = 0;
				P->next = P1->next;
			}
			return;
		}
	}
	return;
}

/* This queries for BIOS information.  */
void
init_bios_info (void)
{
#ifndef STAGE1_5
  unsigned long cont, memtmp, addr;
  int drive;
#endif
#ifndef GRUB_UTIL
#ifndef STAGE1_5
  unsigned long force_pxe_as_boot_device;
#endif /* ! STAGE1_5 */
#endif /* ! GRUB_UTIL */

#ifndef GRUB_UTIL
#ifndef STAGE1_5
  is64bit = check_64bit_and_PAE ();
#endif /* ! STAGE1_5 */
#endif /* ! GRUB_UTIL */

#ifndef GRUB_UTIL
  /* initialize mem alloc array */
  grub_memset(mem_alloc_array_start,0,(int)(mem_alloc_array_end - mem_alloc_array_start));
  mem_alloc_array_start[0].addr = free_mem_start;
  mem_alloc_array_start[1].addr = 0;	/* end the array */
  malloc_array_start = (struct malloc_array *)mem_alloc_array_start + 10;
  malloc_array_start->addr = free_mem_start + 0x400000;
  malloc_array_start->next = (struct malloc_array *)&free_mem_end;
#endif /* ! GRUB_UTIL */

  /*
   *  Get information from BIOS on installed RAM.
   */
#ifndef STAGE1_5
  DEBUG_SLEEP
  printf("\rGet lower memory... ");
#endif
  saved_mem_lower = get_memsize (0);	/* int12 --------safe enough */
#ifndef STAGE1_5
  printf("\rGet upper memory... ");
#endif
  saved_mem_upper = get_memsize (1);	/* int15/88 -----safe enough */

#ifndef STAGE1_5
  /*
   *  We need to call this somewhere before trying to put data
   *  above 1 MB, since without calling it, address line 20 will be wired
   *  to 0.  Not too desirable.
   */

#ifndef GRUB_UTIL
  debug = debug_boot + 1;
  DEBUG_SLEEP
  printf("\rTurning on gate A20...                          ");
#if 1
    {
	if (gateA20 (1))			/* int15/24 -----safe enough */
	{
		/* wipe out the messages on success */
		printf("\r                                                                \r");
	} else {
		printf("Failure! Report bug, please!\n");
		grub_sleep (5);	/* sleep 5 second on failure */
	}
    }
#else
  extern void grub2_gate_a20 (int on);
  grub2_gate_a20 (1);
  printf("\r                        \r");	/* wipe out the messages */
#endif
  DEBUG_SLEEP
#endif

  /* Store the size of extended memory in EXTENDED_MEMORY, in order to
     tell it to non-Multiboot OSes.  */
  extended_memory = saved_mem_upper;
  
  /*
   *  The MBI.MEM_UPPER variable only recognizes upper memory in the
   *  first memory region.  If there are multiple memory regions,
   *  the rest are reported to a Multiboot-compliant OS, but otherwise
   *  unused by GRUB.
   */

#ifdef GRUB_UTIL
  saved_mmap_addr = get_code_end ();
#endif
  addr = saved_mmap_addr;
  cont = 0;

  printf("\rGet E820 memory...           ");
  do
    {
      cont = get_mmap_entry ((void *) addr, cont);	/* int15/e820 ------ will write memory! */

      /* If the returned buffer's length is zero, quit. */
      if (! *((unsigned long *) addr))
	break;

      saved_mmap_length += *((unsigned long *) addr) + 4;
      addr += *((unsigned long *) addr) + 4;
    }
  while (cont);

  if (! (saved_mmap_length))
	printf("\rGet E801 memory...           ");

  if (saved_mmap_length)
    {
      unsigned long long max_addr;
      
      /*
       *  This is to get the lower memory, and upper memory (up to the
       *  first memory hole), into the MBI.MEM_{LOWER,UPPER}
       *  elements.  This is for OS's that don't care about the memory
       *  map, but might care about total RAM available.
       */
      saved_mem_lower = mmap_avail_at (0) >> 10;
      saved_mem_upper = mmap_avail_at (0x100000) >> 10;
      saved_mem_higher = mmap_avail_at (0x100000000ULL) >> 10;

      /* Find the maximum available address. Ignore any memory holes.  */
      for (max_addr = 0, addr = saved_mmap_addr;
	   addr < saved_mmap_addr + saved_mmap_length;
	   addr += *((unsigned long *) addr) + 4)
	{
	  struct AddrRangeDesc *desc = (struct AddrRangeDesc *) addr;
	  
	  if (desc->Type == MB_ARD_MEMORY && desc->Length > 0
	      && desc->BaseAddr + desc->Length > max_addr)
	    max_addr = desc->BaseAddr + desc->Length;
	}

      extended_memory = (max_addr - 0x100000) >> 10;
    }
  else if ((memtmp = get_eisamemsize ()) != -1)		/* int15/e801 ------safe enough */
    {
      cont = memtmp & ~0xFFFF;
      memtmp = memtmp & 0xFFFF;

      if (cont != 0)
	extended_memory = (cont >> 10) + 0x3c00;
      else
	extended_memory = memtmp;
      
      saved_mem_upper = memtmp;

      if (!cont || (memtmp == 0x3c00))
	{
	  saved_mem_upper += (cont >> 10);

//	  /* XXX should I do this at all ??? */
//
//	  saved_mmap_addr = (unsigned long) fakemap;
//	  saved_mmap_length = sizeof (struct AddrRangeDesc) * 2;
//	  fakemap[0].Length = (saved_mem_lower << 10);
//	  fakemap[1].Length = (saved_mem_upper << 10);
//	  fakemap[2].Length = 0;
	}
      //else
	{
	  /* XXX should I do this at all ??? */

	  saved_mmap_addr = (unsigned long) fakemap;
	  saved_mmap_length = sizeof (fakemap);
	  fakemap[0].Length = (saved_mem_lower << 10);
	  fakemap[1].Length = (memtmp << 10);
	  fakemap[2].Length = cont;
	}
    }

  printf("\r                        \r");	/* wipe out the messages */

  mbi.mem_upper = saved_mem_upper;
  mbi.mem_lower = saved_mem_lower;
  mbi.mmap_addr = saved_mmap_addr;
  mbi.mmap_length = saved_mmap_length;

#if 0
  /* Get the drive info.  */
  /* FIXME: This should be postponed until a Multiboot kernel actually
     requires it, because this could slow down the start-up
     unreasonably.  */
  mbi.drives_length = 0;
  mbi.drives_addr = addr;

  /* For now, GRUB doesn't probe floppies, since it is trivial to map
     floppy drives to BIOS drives.  */
#ifdef GRUB_UTIL
#define FIND_DRIVES 8
#else
#define FIND_DRIVES (*((char *)0x475))
#endif
  if (debug > 1)
	grub_printf ("hard drives: %d, int13: %X, int15: %X\n", FIND_DRIVES, *(unsigned long *)0x4C, *(unsigned long *)0x54);
  DEBUG_SLEEP

  for (drive = 0x80; drive < 0x80 + FIND_DRIVES; drive++)
    {
      struct drive_info *info = (struct drive_info *) addr;
//      unsigned short *port;
      
      /* Get the geometry. This ensures that the drive is present.  */

      if (debug > 1)
	grub_printf ("get_diskinfo(%X), ", drive);

      if (get_diskinfo (drive, &tmp_geom))
	continue;//break;

      if (debug > 1)
	grub_printf (" %sC/H/S=%d/%d/%d, Sector Count/Size=%ld/%d\n",
		((tmp_geom.flags & BIOSDISK_FLAG_LBA_EXTENSION) ? "LBA, " : ""),
		tmp_geom.cylinders, tmp_geom.heads, tmp_geom.sectors,
		(unsigned long long)tmp_geom.total_sectors, tmp_geom.sector_size);
      
      /* Set the information.  */
      info->drive_number = drive;
      info->drive_mode = ((tmp_geom.flags & BIOSDISK_FLAG_LBA_EXTENSION)
			  ? MB_DI_LBA_MODE : MB_DI_CHS_MODE);
      info->drive_cylinders = tmp_geom.cylinders;
      info->drive_heads = tmp_geom.heads;
      info->drive_sectors = tmp_geom.sectors;

      addr += sizeof (struct drive_info);

      info->size = addr - (unsigned long) info;
      mbi.drives_length += info->size;
    }
#endif

#ifdef GRUB_UTIL
  init_free_mem_start = addr;
#else
  init_free_mem_start = get_code_end ();
#endif

  //DEBUG_SLEEP

  /*
   *  Initialize other Multiboot Info flags.
   */

  mbi.flags = (MB_INFO_MEMORY | MB_INFO_CMDLINE | MB_INFO_BOOTDEV | MB_INFO_DRIVE_INFO | MB_INFO_CONFIG_TABLE | MB_INFO_BOOT_LOADER_NAME);
  if (saved_mmap_length)
    mbi.flags |= MB_INFO_MEM_MAP;
  
#endif /* STAGE1_5 */

#ifndef GRUB_UTIL
#ifndef STAGE1_5
#ifdef FSYS_PXE
    force_pxe_as_boot_device = 0;
    if (! ((*(char *)0x8205) & 0x01))	/* if it is not disable pxe */
    {
	//pxe_detect();
	if (! pxe_entry)
	{
	    PXENV_GET_CACHED_INFO_t get_cached_info;

	    printf("\rbegin pxe scan...            ");
	    pxe_scan ();
	    DEBUG_SLEEP

	    if (pxe_entry)
	    {
		//pxe_basemem = *((unsigned short*)0x413);

		get_cached_info.PacketType = PXENV_PACKET_TYPE_DHCP_ACK;
		get_cached_info.Buffer = get_cached_info.BufferSize = 0;

		printf("\rbegin pxe call(type=DHCP_ACK)...            ");
		pxe_call (PXENV_GET_CACHED_INFO, &get_cached_info);
		DEBUG_SLEEP

		if (get_cached_info.Status)
		{
			grub_printf ("\nFatal: DHCP_ACK failure!\n");
			goto pxe_init_fail;
		}

		discover_reply = LINEAR(get_cached_info.Buffer);

		pxe_yip = discover_reply->yip;
		pxe_sip = discover_reply->sip;
		pxe_gip = discover_reply->gip;

		pxe_mac_type = discover_reply->Hardware;
		pxe_mac_len = discover_reply->Hardlen;
		grub_memmove (&pxe_mac, &discover_reply->CAddr, pxe_mac_len);

		get_cached_info.PacketType = PXENV_PACKET_TYPE_CACHED_REPLY;
		get_cached_info.Buffer = get_cached_info.BufferSize = 0;

		printf("\rbegin pxe call(type=CACHED_REPLY)...            ");
		pxe_call (PXENV_GET_CACHED_INFO, &get_cached_info);
		DEBUG_SLEEP

		if (get_cached_info.Status)
		{
			grub_printf ("\nFatal: CACHED_REPLY failure!\n");
pxe_init_fail:
			pxe_keep = 0;
			pxe_unload ();
			DEBUG_SLEEP
			pxe_entry = 0;
			discover_reply = 0;
			goto pxe_init_done;
		}

		discover_reply = LINEAR(get_cached_info.Buffer);

		/* on pxe boot, we only use preset_menu */
		if (preset_menu != (char*)0x800)
			preset_menu = (char*)0x800;
		if (*config_file)
			*config_file = 0;
		force_pxe_as_boot_device = 1;
		if (bios_id != 1)		/* if it is not Bochs ... */
			goto set_root;;		/* ... skip cdrom check. */
	    }
	}
    }
pxe_init_done:
#endif /* FSYS_PXE */
#endif /* ! STAGE1_5 */
#endif /* ! GRUB_UTIL */

#if !defined(STAGE1_5) && !defined(GRUB_UTIL)
  /* Set cdrom drive.  */
    
#ifdef GRUB_UTIL
#define FIND_DRIVES 8
#else
#define FIND_DRIVES (*((char *)0x475))
#endif
    /* Get the geometry.  */
    if (debug > 1)
	printf("\rboot drive=%X, ", boot_drive);
    if ((((unsigned char)boot_drive) >= 0x80 + FIND_DRIVES)
	&& ! ((*(char *)0x8205) & 0x10))	/* if it is not disable startup cdrom drive look-up. */
    {
	cdrom_drive = get_cdinfo (boot_drive, &tmp_geom);
	if (! cdrom_drive || cdrom_drive != boot_drive)
		cdrom_drive = GRUB_INVALID_DRIVE;

	if (cdrom_drive == GRUB_INVALID_DRIVE)
	{
		/* read sector 16 - ISO9660 Primary Volume Descriptor
		 *	sector 17 - EL Torito Volume Descriptor
		 */
		struct disk_address_packet
		{
			unsigned char length;
			unsigned char reserved;
			unsigned short blocks;
			unsigned long buffer;
			unsigned long long block;

			unsigned char dummy[16];
		} __attribute__ ((packed)) *dap;

		dap = (struct disk_address_packet *)0x580;

		dap->length = 0x10;
		dap->reserved = 0;
		dap->blocks = 2;
		dap->buffer = 0x5F80/*SCRATCHSEG*/ << 16;
		dap->block = 16;

		/* set a known value */
		grub_memset ((char *)0x5F800, 0xEC, 0x1000);
		biosdisk_int13_extensions (0x4200, (unsigned char)boot_drive, dap);
		/* check ISO9660 Primary Volume Descriptor */
		if (! memcmp ((char *)0x5F800, "\1CD001\1\0", 8))
		/* check the EL Torito Volume Descriptor */
		if (! memcmp ((char *)0x60000, "\0CD001\1EL TORITO SPECIFICATION\0", 31))
		/* see if it is a big sector */
		{
			//char *p;
			//for (p = (char *)0x5FA00; p < (char *)0x60000; p++)
			//{
			//	if ((*p) != (char)0xEC)
			//	{
					cdrom_drive = boot_drive;
			//		break;
			//	}
			//}
		}

	}
    }
#undef FIND_DRIVES

    if (debug > 1)
	printf("%s\n", (cdrom_drive == GRUB_INVALID_DRIVE ? "Not CD":"Is CD"));
  DEBUG_SLEEP
#endif
  
#if !defined(STAGE1_5) && !defined(GRUB_UTIL)

  if (cdrom_drive == GRUB_INVALID_DRIVE
     && ! ((*(char *)0x8205) & 0x10))	/* if it is not disable startup cdrom drive look-up. */
  {
    int err;
    int version;
    struct drive_parameters *drp = (struct drive_parameters *)0x600;

#ifdef GRUB_UTIL
#define FIND_DRIVES 8
#else
#define FIND_DRIVES (*((char *)0x475))
#endif
	/* Drive 7F causes hang on motherboard "jetway 694AS/TAS", as reported
	 * by renfeide112@126.com, 2009-09-30. So no more play with 7F. */
    for (drive = 0xFF; drive > 0x7F; drive--)
    {
      if (drive >= 0x80 && drive < 0x80 + FIND_DRIVES)
	continue;	/* skip hard drives */
      
      /* Get the geometry.  */
      if (debug > 1)
	grub_printf ("\rget_cdinfo(%X),", drive);
      cdrom_drive = get_cdinfo (drive, &tmp_geom);
      if (cdrom_drive)
      {
	if (drive != 0x7F)
	  break;
	drive = cdrom_drive;
	cdrom_drive = get_cdinfo (drive, &tmp_geom);
	if (cdrom_drive == drive)
	  break;
	drive = 0x7F;
      }
    
      cdrom_drive = GRUB_INVALID_DRIVE;
    
      /* Some buggy BIOSes will hang at EBIOS `Get Drive Parameters' call
       * (INT 13h function 48h). So we only do further checks for Bochs.
       */
      
      if (bios_id != 1)		/* if it is not Bochs ... */
	continue;		/* ... skip and try next drive. */
      
      /* When qemu has a cdrom attached but not booted from cdrom, its
       * `get bootable cdrom status call', the int13/ax=4B01, returns CF=0
       * but with a wrong `Bootable CD-ROM Specification Packet' as follows:
       *
       * The first byte(packet size) is 0x13, all the rest bytes are 0's.
       * 
       * So we need to call get_diskinfo() here as a workaround.
       *
       *		(The bug was reported by Jacopo Lazzari. Thanks!)
       */

      if (drive >= 0x80)
      {
	version = check_int13_extensions (drive);

	if (! (version & 1)) /* not support functions 42h-44h, 47h-48h */
	    continue;	/* failure, try next drive. */
	
        /* It is safe to clear out DRP.  */
        grub_memset (drp, 0, sizeof (struct drive_parameters));
	  
	drp->size = sizeof (struct drive_parameters) - 16;
	  
	err = biosdisk_int13_extensions (0x4800, drive, drp);
	if (! err && drp->bytes_per_sector == ISO_SECTOR_SIZE)
	{
	    /* mount the drive, confirm the media exists. */
	    current_drive = drive;
	    current_partition = 0x00FFFFFF;
	    if (open_device ())
	    {
		/* Assume it is CDROM.  */
		cdrom_drive = drive;
		break;
	    }
	}
      } /* if (drive >= 0x80) */
    
    } /* for (drive = 0x7F; drive < 0xff; drive++) */
  } /* if (cdrom_drive == GRUB_INVALID_DRIVE) */
#undef FIND_DRIVES
  
//  if (cdrom_drive != GRUB_INVALID_DRIVE)  
//  {
//	/* skip pxe init if booting from cdrom */
//	*(char *)0x8205 |= 0x01;
//  }

  if (debug > 1)
    grub_printf(" cdrom_drive == %X.\n", cdrom_drive);
  DEBUG_SLEEP
#endif /* ! STAGE1_5 && ! GRUB_UTIL */

  /* check if the no-emulation-mode bootable cdrom exists. */
  
  /* if cdrom_drive is active, we assume it is the boot device */
  if (saved_entryno == 0 && force_cdrom_as_boot_device)
	force_cdrom_as_boot_device = 0;
  if (cdrom_drive != GRUB_INVALID_DRIVE && force_cdrom_as_boot_device)
  {
    boot_drive = cdrom_drive;	/* force it to be the boot drive */
  }

  if (boot_drive == cdrom_drive)
	/* force it to be "whole drive" without partition table */
	install_partition = 0xFFFFFF;

#ifndef GRUB_UTIL
#ifndef STAGE1_5
set_root:

  if (force_pxe_as_boot_device)
  {
	boot_drive = PXE_DRIVE;
	install_partition = 0xFFFFFF;
  }
#endif /* ! STAGE1_5 */
#endif /* ! GRUB_UTIL */

  /* Set root drive and partition.  */
  saved_drive = boot_drive;
  saved_partition = install_partition;
  force_cdrom_as_boot_device = 0;

#ifndef GRUB_UTIL
#ifndef STAGE1_5
  if (fb_status)//set boot_drive to fb_drive when boot from fbinst.
     boot_drive = FB_DRIVE;

  debug = 1;
  if (! atapi_dev_count)
    min_cdrom_id = (cdrom_drive < 0xE0 && cdrom_drive >= 0xC0) ? 0xE0 : 0xC0;
  
  /* if grub.exe is booted as a Linux kernel, check the initrd disk. */

  /* the real mode zero page(only the beginning 2 sectors, the boot params) is loaded at 0xA00 */

  /* check the header signature "HdrS" (0x53726448) */

  if (*(unsigned long*)(int*)(0xA00 + 0x202) == 0x53726448)
  {
	unsigned long initrd_addr;
	unsigned long initrd_size;
	
	initrd_addr = *(unsigned long*)(int*)(0xA00 + 0x218);
	initrd_size = *(unsigned long*)(int*)(0xA00 + 0x21c);
	if (initrd_addr && initrd_size)
	{
	    rd_base = initrd_addr;
	    rd_size = initrd_size;

	    /* check if there is a partition table */
	    if (*(unsigned short *)(initrd_addr + 0x40) == 0xAA55)
	    {
		if (! probe_mbr ((struct master_and_dos_boot_sector *)initrd_addr, 0, initrd_size, 0))
		    ram_drive = 0xfe;	/* partition table is valid, so let it be a harddrive */
		else
		    grub_printf ("\nUnrecognized partition table for RAM DRIVE; assuming floppy. Please rebuild\nit using a Microsoft-compatible FDISK tool, if the INITRD is a hard-disk image.\n");
	    }
	}
  }
#endif /* ! STAGE1_5 */
#endif /* ! GRUB_UTIL */

  /* Start main routine here.  */
  
  grub_printf("Starting cmain() ... ");
  
#if !defined(STAGE1_5) && !defined(GRUB_UTIL)
  DEBUG_SLEEP
#endif /* ! STAGE1_5 && ! GRUB_UTIL */
//  cmain ();	/* moved into asm.S and asmstub.c */
}
