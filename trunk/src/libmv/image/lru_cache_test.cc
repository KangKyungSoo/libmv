// Copyright (c) 2007, 2008 libmv authors.
// 
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to
// deal in the Software without restriction, including without limitation the
// rights to use, copy, modify, merge, publish, distribute, sublicense, and/or
// sell copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
// 
// The above copyright notice and this permission notice shall be included in
// all copies or substantial portions of the Software.
// 
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
// FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS
// IN THE SOFTWARE.

#include "libmv/image/lru_cache.h"
#include "testing/testing.h"

namespace {

typedef libmv::LRUCache<int, int> TestCache;

TEST(LRUCache, NullOnEmptyKey) {
  TestCache cache(10);
  int *ptr;
  bool succeeded = cache.FetchAndPin(4, &ptr);
  EXPECT_FALSE(succeeded);
}

TEST(LRUCache, StoreAndRetreiveOneItem) {
  TestCache cache(10);
  int *ptr;
  cache.StoreAndPin(4, new int(40));
  bool succeeded = cache.FetchAndPin(4, &ptr);
  EXPECT_EQ(succeeded, true);
  EXPECT_EQ(*ptr, 40);
}

TEST(LRUCache, SizeIncreasesWithAddedItems) {
  TestCache cache(10);
  EXPECT_EQ(cache.Size(), 0);
  cache.StoreAndPin(4, new int(40));
  EXPECT_EQ(cache.Size(), 1);
  cache.StoreAndPin(5, new int(40));
  EXPECT_EQ(cache.Size(), 2);
}

TEST(LRUCache, MaxSizeExceededWhenItemsPinned) {
  TestCache cache(3);
  cache.StoreAndPin(4, new int(40));
  cache.StoreAndPin(5, new int(50));
  cache.StoreAndPin(6, new int(60));
  EXPECT_EQ(cache.Size(), 3);
  cache.StoreAndPin(7, new int(70));
  EXPECT_EQ(cache.Size(), 4);
}

TEST(LRUCache, MaxSizeNotExceededWhenItemsUnpinned) {
  TestCache cache(3);
  cache.StoreAndPin(4, new int(40));
  cache.StoreAndPin(5, new int(50));
  cache.StoreAndPin(6, new int(60));
  EXPECT_EQ(cache.Size(), 3);
  cache.MassUnpin();
  cache.StoreAndPin(7, new int(70));
  EXPECT_EQ(cache.Size(), 3);
  cache.StoreAndPin(8, new int(80));
  EXPECT_EQ(cache.Size(), 3);
  cache.StoreAndPin(9, new int(90));
  EXPECT_EQ(cache.Size(), 3);
}

TEST(LRUCache, HarderCase) {
  TestCache cache(3);
  cache.StoreAndPin(4, new int(40));
  cache.StoreAndPin(5, new int(50));
  cache.StoreAndPin(6, new int(60));
  EXPECT_EQ(cache.Size(), 3);
  cache.MassUnpin();
  cache.StoreAndPin(7, new int(70));
  EXPECT_EQ(cache.Size(), 3);
  cache.StoreAndPin(8, new int(80));
  EXPECT_EQ(cache.Size(), 3);
  cache.StoreAndPin(9, new int(90));
  EXPECT_EQ(cache.Size(), 3);
}

TEST(LRUCache, SizeDecreaseWhenMaxSizeChanged) {
  TestCache cache(3);
  cache.StoreAndPin(4, new int(40));
  cache.StoreAndPin(5, new int(50));
  cache.StoreAndPin(6, new int(60));
  cache.MassUnpin();
  EXPECT_EQ(cache.Size(), 3);
  cache.SetMaxSize(2);
  EXPECT_EQ(cache.Size(), 2);
  cache.SetMaxSize(1);
  EXPECT_EQ(cache.Size(), 1);
  cache.SetMaxSize(10);
  EXPECT_EQ(cache.Size(), 1);
}

}  // namespace
