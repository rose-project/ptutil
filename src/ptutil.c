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

#include "pt/gpt.h"
#include "pt/helpers.h"

int main(int argc, char* argv[])
{
    /* FIXME just a primitive first test app... */

    char* path = NULL;

    struct gpt_device device;

    if(argc < 1)
        return EXIT_FAILURE;

    path = argv[1];

    if( -1 == gpt_init(&device, path) )
    {
        logErr("Init failed");
        return EXIT_FAILURE;
    }

    if(-1 == gpt_validate())
    {
        logErr("Primary gpt invalid");
    }

    logDbg("Dump primary gpt");
    gpt_dump(&device, GPT_PRIMARY);

    if(-1 == gpt_validate())
    {
        logErr("Backup gpt invalid");
    }

    logDbg("Dump backup gpt");
    gpt_dump(&device, GPT_BACKUP);

    if(-1 == gpt_deInit(&device))
    {
        logErr("DeInit failed");
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}
