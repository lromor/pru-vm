
#include <iostream>
#include <gtest/gtest.h>
#include <gmock/gmock.h>


const unsigned int PRUcode[] =  {
     0x241234c1,
     0x24abcd81 };

int main(int argc, char *argv[]) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
