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

#ifndef GPTTEST_HPP
#define GPTTEST_HPP

#include <gtest/gtest.h>
#include <pt/gpt.h>

static const char* DISK_IMAGE_PATH = NULL;


TEST(GptTest, DeInit)
{
    struct gpt_device device;
    ASSERT_EQ(true, gpt_init(&device, DISK_IMAGE_PATH));
    ASSERT_EQ(true, gpt_deInit(&device));
}

TEST(GptTest, Validation)
{
    struct gpt_device device;
    ASSERT_EQ(true, gpt_init(&device, DISK_IMAGE_PATH));
    EXPECT_EQ(0, gpt_validate(&device, GPT_PRIMARY, false));
    EXPECT_EQ(0, gpt_validate(&device, GPT_BACKUP, false));
    ASSERT_EQ(true, gpt_deInit(&device));
}

TEST(GptTest, InValidation)
{
    struct gpt_device device;
    ASSERT_EQ(true, gpt_init(&device, DISK_IMAGE_PATH));
    EXPECT_EQ(0, gpt_validate(&device, GPT_PRIMARY, false));
    EXPECT_EQ(0, gpt_invalidate(&device, GPT_PRIMARY, false));
    EXPECT_EQ(1, gpt_validate(&device, GPT_PRIMARY, false));
    EXPECT_EQ(0, gpt_validate(&device, GPT_PRIMARY, true));
    ASSERT_EQ(true, gpt_deInit(&device));
}

TEST(GptTest, ForceInValidation)
{
    struct gpt_device device;
    ASSERT_EQ(true, gpt_init(&device, DISK_IMAGE_PATH));
    EXPECT_EQ( 0,   gpt_validate(&device,   GPT_PRIMARY, false));
    EXPECT_EQ( 0,   gpt_invalidate(&device, GPT_PRIMARY, false));
    EXPECT_EQ( 1,   gpt_invalidate(&device, GPT_BACKUP,  false));
    EXPECT_EQ( 0,   gpt_invalidate(&device, GPT_BACKUP,  true));
    EXPECT_EQ( 0,   gpt_validate(&device,   GPT_PRIMARY, true));
    EXPECT_EQ( 0,   gpt_validate(&device,   GPT_BACKUP,  true));
    ASSERT_EQ(true, gpt_deInit(&device));
}

#endif // GPTTEST_HPP
