#include <gtest/gtest.h>
#include "../include/forward_list.h"
#include <std::string>

class ForwardListTest : public testing::Test {
 protected:
  ForwardList() {
     q0_.Enqueue(1);
     q1_.Enqueue(1.1);
     q2_.Enqueue("2");
  }

  // ~QueueTest() override = default;

  htp::forward_list<int> q0_;
  htp::forward_list<float> q1_;
  htp::forward_list<std::string> q2_;
};
