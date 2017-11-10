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

#include <stdio.h>
#include <stdarg.h>
#include <stdlib.h>
#include <unistd.h>
#include <getopt.h>

#include "pt/gpt.h"
#include "pt/helpers.h"

enum ACTIONS
{
    ACT_DUMP = 0,
    ACT_VALIDATE,
    ACT_INVALIDATE,
    ACT_MOVE,
    ACT_INVALID = 255
};

void usage(const char* name)
{
    printf("Usage: %s [options] <disk>\n", strrchr(name, '/') ? strrchr(name, '/') + 1 : name);
    printf("\n"
           "Selection:\n"
           "        --primary       select table from disk begin\n"
           "        --backup        select table from disk end\n"
           "General options:\n"
           "    -d  --dump          Dump gpt and partition informations\n"
           "Gpt options:\n"
           "        --validate      validates gpt header\n"
           "        --invalidate    invalidates gpt header (writes zeros in crc32 checksum section)\n"
           "    -f  --force         action will be forced, use with caution!\n"
           "                        This could lead to a broken system!\n"
           "    -r  --repair        gpt validate should try to repair an invalidated gpt table\n"
           "    -m  --move          move gpt table to it's default position or to another given by address parameter\n"
           "	-a                  absolute address in bytes"
           "Partition options:\n"
           "\n"
           "Others:\n"
           "   -h   --help          display this help message\n"
           "        --verbose       activate verbose logging\n"
           "\n");
}

extern bool gpt_main(int argc, char* argv[])
{
    int c;
    int option_index = 0;
    struct gpt_device device;
    const char* diskname = NULL;

    static int8_t selection = -1;
    static int action = ACT_INVALID;
    size_t address = 0;
    bool force  = false,
         repair = false;

    static struct option long_options[] = {
        {"verbose",    no_argument, &VERBOSE_LOGGING_FLAG, 1 },
        {"validate",   no_argument, &action, ACT_VALIDATE },
        {"invalidate", no_argument, &action, ACT_INVALIDATE },
        /* without flag setter */
        {"primary", no_argument, NULL,  0 },
        {"backup",  no_argument, NULL,  0 },
        {"dump",    no_argument, NULL, 'd'},
        {"force",   no_argument, NULL, 'f'},
        {"help",    no_argument, NULL, 'h'},
        {"repair",  no_argument, NULL, 'r'},
        {"move",    no_argument, NULL, 'm'},
        {NULL, 0, NULL, 0}
    };

    while(-1 != ( c = getopt_long (argc, argv, "dfhrma:",
                                   long_options, &option_index) ))
    {
        switch(c)
        {
        case 0:
            if(long_options[option_index].flag != 0)
                break;
            if(0 == strcmp(long_options[option_index].name, "primary") ||
               0 == strcmp(long_options[option_index].name, "backup"))
            {
                if(selection != -1)
                {
                    logErr("Primary and backup flag could not be set at same time\n");
                    usage(argv[0]);
                    return false;
                }
                else if(0 == strcmp(long_options[option_index].name, "primary"))
                    selection = GPT_PRIMARY;
                else
                    selection = GPT_BACKUP;
            }

                break;
        case 'd':
            action = ACT_DUMP;
            break;
        case 'f':
            force = true;
            break;
        case 'r':
            repair = true;
            break;
        case 'm':
            if(ACT_INVALID != action)
                logWarn("Overwrite previous selected action");
            action = ACT_MOVE;
            break;
        case 'a':
        {
            char *endptr;
            address = strtoull(optarg, &endptr, 0);
            if(endptr == optarg || *endptr != '\0')
            {
                logErr("Address must be an integer\n");
                return false;
            }
        }
            break;
        case 'h':
            /*fallthrough*/
        case '?':
            /*fallthrough*/
        default:
            usage(argv[0]);
            return false;
        }
    }

    if(optind >= argc || selection == -1)
    {
        usage(argv[0]);
        return false;
    }

    diskname = argv[optind];    // ignore further paths
    logDbg("Disk: %s", diskname);

    if(!gpt_init(&device, diskname))
    {
        return false;
    }

    switch(action)
    {
    case ACT_DUMP:
        gpt_dump(&device, (uint8_t)selection);
        break;
    case ACT_INVALIDATE:
        if( gpt_invalidate(&device, (uint8_t)selection, force) )
            logErr("%s-GPT could not be invalidated",
                   selection == GPT_PRIMARY ? "Primary" : "Backup");
        else
            printf("%s-GPT is invalid now\n",
                   selection == GPT_PRIMARY ? "Primary" : "Backup");
        break;
    case ACT_VALIDATE:
        if( gpt_validate(&device, (uint8_t)selection, repair) )
            logErr("%s-GPT is not valid",
                   selection == GPT_PRIMARY ? "Primary" : "Backup");
        else
            printf("%s-GPT is valid\n",
                   selection == GPT_PRIMARY ? "Primary" : "Backup");
        break;
    case ACT_MOVE:
        /* TODO move */
        (void) address;
        break;
    }

    gpt_deInit(&device);
    return true;
}
