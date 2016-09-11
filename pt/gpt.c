/*********************************************************************************
 * MIT License
 *
 * Copyright (c) 2016 rose-project
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 ********************************************************************************/

#include <pt/gpt.h>

#include <stdlib.h>

/*
 * GPT specification could be found here:
 * http://www.uefi.org/sites/default/files/resources/UEFI%20Spec%202_6.pdf
 * GPT S.165
 */

typedef uint16_t efi_char16_t;


/* UUID spec https://www.ietf.org/rfc/rfc4122.txt */
typedef struct {
    uint32_t time_low;
    uint16_t time_mid;
    uint16_t time_hi_and_version;
    uint8_t  clock_seq_hi;
    uint8_t  clock_seq_low;
    uint8_t  node[6];
} efi_guid_t;


typedef struct {
    uint64_t uefi_flags : 64;	/*all bits reserved by UEFI*/
} gpt_entry_attributes;


struct gpt_header
{
    uint64_t signature;	        /* constant 0x5452415020494645 btw. "EFI PART" */
    uint32_t revision;	        /* this revision is 0x00010000 */
    uint32_t header_size;       /* gpt header size should be >= 92bytes */
    uint32_t header_crc32;      /* header checksum (set to 0 during calculation) */
    uint32_t reserved;          /* must be zero */

    uint64_t my_lba;			/* lba location of this header */
    uint64_t alternate_lba;		/* lba position of recovery header */
    uint64_t first_usable_lba;	/* first usable LBA for partitions */
    uint64_t last_usable_lba;	/* last usable LBA for partitions */

    efi_guid_t disk_guid;		     /* disk GUID */
    uint64_t partition_entries_lba;	 /* starting LBA of the GUID partition entry array */
    uint32_t num_partition_entries;  /* number of partition entries */
    uint32_t sizeof_partition_entry; /* size in bytes of each the GUID Partition entry
                                      * value should be 128 x 2^n or zero */
    uint32_t partition_entry_array_crc32; /* Starts at PartitionEntryLBA and is computed
                                           * over a byte length of NumberOfPartitionEntries * SizeOfPartitionEntry */
    // reserved2 : 0;
} __attribute__ ((packed));


struct gpt_entry {
    efi_guid_t	partition_type_guid;   /* Unique ID defines the purpose and type, zero value marks entry as unused */
    efi_guid_t	unique_partition_guid; /* partition GUID */
    uint64_t	starting_lba;          /* partition LBA start */
    uint64_t	ending_lba;	           /* partition LBA end */

    gpt_entry_attributes attributes;	/* uefi specific attribute flags*/

    efi_char16_t	partition_name[72 / sizeof(efi_char16_t)]; /* UTF-16LE string null terminated*/
} __attribute__ ((packed));


int gpt_init(struct gpt_device *device)
{
//    device->primary = malloc(sizeof(struct gpt_header));

    return 0;
}

int gpt_deInit(struct gpt_device *device)
{

    return 0;
}

int gpt_validate()
{

    return 0;
}

int gpt_invalidate()
{

    return 0;
}

void gpt_dump()
{

}
