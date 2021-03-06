/*
 * The MIT License (MIT)
 * 
 * Copyright (c) 2014 Boone "Bea" Adkins
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
 */
/**
 * @file
 *
 * Tests for pomdp::Pretty.
 *
 * @date Jul 20, 2013
 * @author Bea Adkins
 */

#include <gtest/gtest.h>

#include "pretty.h"

using namespace pomdp;

const int n = 6;
const char* names_c[] = {"Error", "Stop", "Go", "Left", "Right", "Grab"};
const int numbers_c[] = {-1, 0, 1, 2, 3, 5};

class TestPrettyPrint : public ::testing::Test
{
protected:
  virtual void SetUp()
  {
    names.assign(names_c, names_c + n);
    numbers.assign(numbers_c, numbers_c + n);
  }

  virtual void TearDown()
  {
  }
  std::vector<std::string> names;
  std::vector<int> numbers;
};

TEST_F(TestPrettyPrint, testConstructor)
{
  Pretty* prettyall = new Pretty(names, numbers);
  for(int i = 0; i < n; i++)
  {
    EXPECT_EQ(names[i], prettyall->toString(numbers[i]));
  }

  Pretty* pretty6 = new Pretty(names);
  for(int i = 0; i < n; i++)
  {
    EXPECT_EQ(names[i], pretty6->toString(i));
  }

  std::vector<std::string> names5;
  names5.assign(names.begin(), names.end() - 1);
  std::vector<int> numbers5;
  numbers5.assign(numbers.begin(), numbers.end() - 1);
  Pretty* pretty5 = new Pretty(names5, numbers);
  for(int i = 0; i < n - 5; i++)
  {
    EXPECT_EQ(names[i], pretty5->toString(numbers[i]));
  }
  EXPECT_EQ("6", pretty5->toString(6));

  delete pretty5;
  pretty5 = new Pretty(names, numbers5);
  for(int i = 0; i < n - 1; i++)
  {
    EXPECT_EQ(names[i], pretty5->toString(numbers5[i]));
  }
  EXPECT_EQ("6", pretty5->toString(6));
}

TEST_F(TestPrettyPrint, testAdd)
{
  Pretty prettyadd = Pretty();

  // Add new member.
  ASSERT_TRUE(prettyadd.addMapping("Derp", 14));
  EXPECT_EQ("Derp", prettyadd.toString(14));

  // Duplicate add.
  ASSERT_FALSE(prettyadd.addMapping("Duplicate", 14));

}

TEST_F(TestPrettyPrint, testGetNames)
{
  Pretty out_of_order(names, numbers);
  std::vector<std::string> names_v = out_of_order.getNames();
  ASSERT_EQ(0, names_v.size()) << "getNames() did not return empty vector for out-of-order number mapping.";

  Pretty all(names);
  names_v = all.getNames();
  ASSERT_EQ(names.size(), names_v.size()) << "Differing lengths for original and getNames() vectors.";
  for(int i = 0; i < n; i++)
  {
    EXPECT_EQ(names[i], names_v[i]);
  }
}

int main(int argc, char* argv[])
{
  testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
