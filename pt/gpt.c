/*********************************************************************************
 * MIT License
 *
 * Copyright (c) 2016 rose-project
 * Copyright (c) 2016 Christian Fries
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

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>

#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>

#include "pt/helpers.h"
#include "pt/crc32.h"

static const uint64_t GPT_SIGNATURE = 0x5452415020494645ULL;

#define LBA_SIZE 512LL

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


/**
 * @brief This gpt_header struct has a lenght of 92 bytes
 */
struct gpt_header
{
    uint64_t signature;	        /* constant 0x5452415020494645 btw. "EFI PART" */
    uint32_t revision;	        /* this revision is 0x00010000 */
    uint32_t header_size;       /* gpt header size should be >= 92bytes */
    uint32_t header_crc32;      /* header checksum (set to 0 during calculation) */
    uint32_t reserved;          /* must be zero */

    uint64_t my_lba;			/* lba location of this header */
    uint64_t alternate_lba;		/* lba position of backup header */
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

/**
 * @brief The gpt_entry struct has a length of 128 Bytes per partition entry
 */
struct gpt_entry {
    efi_guid_t	partition_type_guid;   /* Unique ID defines the purpose and type, zero value marks entry as unused */
    efi_guid_t	unique_partition_guid; /* partition GUID */
    uint64_t	starting_lba;          /* partition LBA start */
    uint64_t	ending_lba;	           /* partition LBA end */

    gpt_entry_attributes attributes;	/* uefi specific attribute flags*/

    efi_char16_t	partition_name[72 / sizeof(efi_char16_t)]; /* UTF-16LE string null terminated*/
} __attribute__ ((packed));


static int _gpt_read_header(const struct gpt_device *device, enum header_type type);


int gpt_init(struct gpt_device *device, char *device_path)
{
    ASSERT(device);
    ASSERT(device_path);
    int cnt = 0;

    device->fd = -1;
    device->primary = (struct gpt_header*) malloc(sizeof(struct gpt_header));
    device->backup  = (struct gpt_header*) malloc(sizeof(struct gpt_header));
    if( !device->primary || !device->backup)
    {
        logErr("Not enough memory");
        goto gpt_malloc_failed;
    }

    device->path = strdup( device_path );
    if(!device->path)
    {
        logErr("(%d) - %s", errno, strerror(errno));
        goto device_failed;
    }

    logDbg("Open %s...", device_path);
    device->fd = open( device_path, O_EXCL | O_SYNC | O_RDWR );
    if(-1 == device->fd)
    {
        logErr("(%d) - %s", errno, strerror(errno));
        goto device_failed;
    }

    /* Init primary / backup structure */
    if(-1 == _gpt_read_header(device, GPT_PRIMARY))
    {
        logWarn("Primary GPT header is invalid");
        cnt++;
    }
    if(-1 == _gpt_read_header(device, GPT_BACKUP))
    {
        logWarn("Backup GPT header is invalid");
        cnt++;
    }

    if(cnt > 1)
    {
        logErr("No gpt headers found!");
        goto device_failed;
    }

    return 0;

device_failed:
    if(device->fd >= 0)
        close(device->fd);
    free((char*)device->path);
    free(device->primary);
    free(device->backup);
    device->primary = NULL;
    device->backup  = NULL;
    device->fd = -1;
gpt_malloc_failed:
    return -1;
}

int gpt_deInit(struct gpt_device *device)
{
    ASSERT(device);
    if( -1 != device->fd )
        close( device->fd );
    free( (char*)device->path );
    free( device->primary );
    free( device->backup );
    device->fd = 0;
    device->path = NULL;
    device->primary = NULL;
    device->backup  = NULL;
    return 0;
}


int gpt_validate(const struct gpt_device *device, enum header_type type, bool repair)
{
    ASSERT(device);
    gpt_header *header;
    gpt_header tmpheader;
    uint32_t crc = 0;

    if(GPT_PRIMARY == type)
        header = device->primary;
    else
        header = device->backup;

    memcpy(&tmpheader, header, sizeof(gpt_header));
    memset(&tmpheader.header_crc32, 0x0, sizeof(tmpheader.header_crc32));

    crc = calculate_crc32((unsigned char*)&tmpheader,sizeof(gpt_header));

    if( tmpheader.signature != GPT_SIGNATURE ||
        crc == 0 || (crc != header->header_crc32 && !repair)
        )
    {
        logDbg("Calculated CRC: %ud != %ud", crc, header->header_crc32 );
        return -1;
    }

    // TODO check header->partition_entries_lba and header->partition_entry_array_crc32

    if(GPT_PRIMARY == type)
    {
        // TODO primary pt checks if AlternateLBA points to backup table
    }

    if(repair)
    {
        // TODO write back valid crc32 checksum
    }

    return 0;
}

int gpt_invalidate(const struct gpt_device *device, enum header_type type)
{
    ASSERT(device);
    return 0;
}

void gpt_dump(const struct gpt_device *device, enum header_type type)
{
    ASSERT(device);
    gpt_header *header;

    if(GPT_PRIMARY == type)
        header = device->primary;
    else
        header = device->backup;

    if(!header)
    {
        logErr("GPT header not initialized");
        return;
    }

    printf("==================================================\n");
    printf("Dump - %s - gpt header\n", GPT_PRIMARY == type ? "PRIMARY" : "BACKUP");
    printf("--------------------------------------------------\n");
    printf("GPT Header Informations\n");
    printf("--------------------------------------------------\n");
    printf("Signature   : %s (0x%016lx)\n",
               header->signature == GPT_SIGNATURE ? "valid" : "invalid",
               header->signature);
    printf("Revision    : %u\n", header->revision);
    printf("Header size : %u\n", header->header_size);
    printf("Header CRC32: 0x%08x\n", header->header_crc32);
    printf("Reserved    : 0x%08x\n", header->reserved);
    printf("--------------------------------------------------\n");
    printf("Partition informations\n");
    printf("--------------------------------------------------\n");
    printf("Current Header LBA    : 0x%016lx\n", header->my_lba);
    printf("Alternative Header LBA: 0x%016lx\n", header->alternate_lba);
    printf("First usable LBA      : 0x%016lx\n", header->first_usable_lba);
    printf("Last usable LBA       : 0x%016lx\n", header->last_usable_lba);
    printf("\n");
    printf("Disk GUID             : fakefake-fake-fake-fake-fakefakefake\n"/*, header->disk_guid*/);	// TODO formater for UUID
    printf("Start entry LBA       : 0x%016lx\n", header->partition_entries_lba);
    printf("Partition entry count : %u\n", header->num_partition_entries);
    printf("Partition entry size  : %d\n", header->sizeof_partition_entry);
    printf("Partition entry CRC32 : 0x%08x\n", header->partition_entry_array_crc32);
    printf("==================================================\n");
}

static int _gpt_read_header(const struct gpt_device *device, enum header_type type)
{
    ASSERT(device);
    ssize_t ret = -1;
    gpt_header* header = NULL;

    if(GPT_PRIMARY == type)
    {
        header = device->primary;
        if( -1 == lseek(device->fd, 1 * LBA_SIZE, SEEK_SET))
            return -1;
        ret = read(device->fd, header, sizeof(gpt_header));
    }
    else if(GPT_BACKUP == type)
    {
        header = device->backup;
        if(device->primary->signature == GPT_SIGNATURE)
        {
            if( -1 == lseek(device->fd, device->primary->alternate_lba * LBA_SIZE, SEEK_SET))
                return -1;
            ret = read(device->fd, header, sizeof(gpt_header));
        }
        /* T*/
        if(header->signature != GPT_SIGNATURE)
        {
            logWarn("No valid backup header at alternate LBA found!");
            if( -1 == lseek(device->fd, -1 * LBA_SIZE, SEEK_END))
                return -1;
            ret = read(device->fd, header, sizeof(gpt_header));
        }
    }
    else
    {
        logErr("Unknown HeaderType");
        return -1;
    }

    if(ret < (ssize_t) sizeof(gpt_header) || header->signature != GPT_SIGNATURE)
    {
        logErr("No valid signature found");
        return -1;
    }

    return 0;
}
