/**
 * 
 *  Copyright:  Copyright (c) 2020, ISSuh
 * 
 */

#include <iostream>
#include <vector>

#include <gtest/gtest.h>

#include <srpc/net/message/Message.hpp>

TEST(Message, Header) {
  srpc::Message msg("00000011-1100-0000-0000-000001100000",
                                 srpc::Command::ACCEPT);

  std::string serializedMsg(msg.serialize());

  srpc::Message deserializedMsg(serializedMsg);

  ASSERT_EQ(1, 1);
}

int main(int argc, char** argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}