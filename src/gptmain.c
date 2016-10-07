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
#include <unistd.h>
#include <getopt.h>

#include "pt/gpt.h"
#include "pt/helpers.h"

void usage(const char* name)
{
    printf("Usage: %s [options] <disk>\n", strrchr(name, '/') ? strrchr(name, '/') + 1 : name);
    printf("\n"
           "Options:\n"
           "   -h   --help           display this help message\n"
           "        --verbose        activate verbose logging\n"
           "\n");
}

extern bool gpt_main(int argc, char* argv[])
{
    int c;
    int option_index = 0;
    const char* diskname = NULL;

    static struct option long_options[] = {
        {"help",    no_argument, NULL, 'h'},
        {"verbose", no_argument, &VERBOSE_LOGGING_FLAG, 1 },
        {NULL, 0, NULL, 0}
    };

    while(-1 != ( c = getopt_long (argc, argv, "h",
                                   long_options, &option_index) ))
    {
        switch(c)
        {
        case 0:
            if(long_options[option_index].flag != 0)
                break;
            /*
             * TODO if long_options[option_index].name
             *      has optarg and is only long opt then handle it here
             */
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

    if(optind >= argc)
    {
        usage(argv[0]);
        return false;
    }

    diskname = argv[optind];    // ignore further paths
    logDbg("Disk: %s", diskname);

    /* Do something powerfull here! */


    return true;
}
