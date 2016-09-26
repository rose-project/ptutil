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
    char* path = NULL;
    int ret = EXIT_SUCCESS;

    struct gpt_device device;

    if(argc == 1)
    {
        logErr("Usage: %s TESTFILE", argv[0]);
        return EXIT_FAILURE;
    }

    path = argv[1];

    if( gpt_init(&device, path) )
    {
        logErr("Init failed");
        return EXIT_FAILURE;
    }

    if( gpt_validate(&device, GPT_PRIMARY, false) )
    {
        logErr("Primary gpt invalid");
        ret = EXIT_FAILURE;
        goto failed_action;
    }

    if( gpt_validate(&device, GPT_BACKUP, false) )
    {
        logErr("Backup gpt invalid");
        ret = EXIT_FAILURE;
        goto failed_action;
    }

    gpt_dump(&device, GPT_PRIMARY);
    gpt_dump(&device, GPT_BACKUP);

failed_action:

    if( gpt_deInit(&device) )
    {
        logErr("DeInit failed");
        ret = EXIT_FAILURE;
    }

    return ret;
}