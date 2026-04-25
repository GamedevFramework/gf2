#include <gf2/core/BehaviorTree.h>

#include "gtest/gtest.h"

namespace {

  struct Blackboard {
    int count = 0;
  };


}

namespace bt = gf::behavior;

TEST(BehaviorTreeTest, TaskSuccess) {
  Blackboard blackboard;
  auto tree = bt::task<Blackboard>([]([[maybe_unused]] Blackboard& blackboard) { return gf::BehaviorStatus::Success; });
  gf::BehaviorStatus status = tree.process(blackboard);
  EXPECT_EQ(status, gf::BehaviorStatus::Success);
}

TEST(BehaviorTreeTest, TaskFailure) {
  Blackboard blackboard;
  auto tree = bt::task<Blackboard>([]([[maybe_unused]] Blackboard& blackboard) { return gf::BehaviorStatus::Failure; });
  gf::BehaviorStatus status = tree.process(blackboard);
  EXPECT_EQ(status, gf::BehaviorStatus::Failure);
}

TEST(BehaviorTreeTest, TaskRunning) {
  Blackboard blackboard;
  auto tree = bt::task<Blackboard>([]([[maybe_unused]] Blackboard& blackboard) { return gf::BehaviorStatus::Running; });
  gf::BehaviorStatus status = tree.process(blackboard);
  EXPECT_EQ(status, gf::BehaviorStatus::Running);
}

TEST(BehaviorTreeTest, ConditionSuccess) {
  Blackboard blackboard;
  auto tree =  bt::condition<Blackboard>([]([[maybe_unused]] Blackboard& blackboard) { return true; });
  gf::BehaviorStatus status = tree.process(blackboard);
  EXPECT_EQ(status, gf::BehaviorStatus::Success);
}

TEST(BehaviorTreeTest, ConditionFailure) {
  Blackboard blackboard;
  auto tree =  bt::condition<Blackboard>([]([[maybe_unused]] Blackboard& blackboard) { return false; });
  gf::BehaviorStatus status = tree.process(blackboard);
  EXPECT_EQ(status, gf::BehaviorStatus::Failure);
}

TEST(BehaviorTreeTest, InvertSuccess) {
  Blackboard blackboard;
  auto tree =  bt::invert<Blackboard>(
    bt::task<Blackboard>([]([[maybe_unused]] Blackboard& blackboard) { return gf::BehaviorStatus::Failure; })
  );
  gf::BehaviorStatus status = tree.process(blackboard);
  EXPECT_EQ(status, gf::BehaviorStatus::Success);
}

TEST(BehaviorTreeTest, InvertFailure) {
  Blackboard blackboard;
  auto tree =  bt::invert<Blackboard>(
    bt::task<Blackboard>([]([[maybe_unused]] Blackboard& blackboard) { return gf::BehaviorStatus::Success; })
  );
  gf::BehaviorStatus status = tree.process(blackboard);
  EXPECT_EQ(status, gf::BehaviorStatus::Failure);
}

TEST(BehaviorTreeTest, SucceedSuccess) {
  Blackboard blackboard;
  auto tree =  bt::succeed<Blackboard>(
    bt::task<Blackboard>([]([[maybe_unused]] Blackboard& blackboard) { return gf::BehaviorStatus::Success; })
  );
  gf::BehaviorStatus status = tree.process(blackboard);
  EXPECT_EQ(status, gf::BehaviorStatus::Success);
}

TEST(BehaviorTreeTest, SucceedFailureSuccess) {
  Blackboard blackboard;
  auto tree =  bt::succeed<Blackboard>(
    bt::task<Blackboard>([]([[maybe_unused]] Blackboard& blackboard) { return gf::BehaviorStatus::Failure; })
  );
  gf::BehaviorStatus status = tree.process(blackboard);
  EXPECT_EQ(status, gf::BehaviorStatus::Success);
}

TEST(BehaviorTreeTest, SucceedRunning) {
  Blackboard blackboard;
  auto tree =  bt::succeed<Blackboard>(
    bt::task<Blackboard>([]([[maybe_unused]] Blackboard& blackboard) { return gf::BehaviorStatus::Running; })
  );
  gf::BehaviorStatus status = tree.process(blackboard);
  EXPECT_EQ(status, gf::BehaviorStatus::Running);
}

TEST(BehaviorTreeTest, RepeatSuccess) {
  Blackboard blackboard;
  auto tree =  bt::repeat<Blackboard>(
    bt::condition<Blackboard>([](Blackboard& blackboard) {
      ++blackboard.count;
      return blackboard.count < 4;
    })
  );
  gf::BehaviorStatus status = tree.process(blackboard);
  EXPECT_EQ(status, gf::BehaviorStatus::Success);
  EXPECT_EQ(blackboard.count, 4);
}

TEST(BehaviorTreeTest, SequenceSuccess) {
  Blackboard blackboard;
  auto tree =  bt::sequence<Blackboard>(
    bt::task<Blackboard>([]([[maybe_unused]] Blackboard& blackboard) {
      ++blackboard.count;
      return gf::BehaviorStatus::Success;
    }),
    bt::task<Blackboard>([]([[maybe_unused]] Blackboard& blackboard) {
      ++blackboard.count;
      return gf::BehaviorStatus::Success;
    })
  );
  gf::BehaviorStatus status = tree.process(blackboard);
  EXPECT_EQ(status, gf::BehaviorStatus::Success);
  EXPECT_EQ(blackboard.count, 2);
}

TEST(BehaviorTreeTest, SequenceFailure) {
  Blackboard blackboard;
  auto tree =  bt::sequence<Blackboard>(
    bt::task<Blackboard>([]([[maybe_unused]] Blackboard& blackboard) {
      ++blackboard.count;
      return gf::BehaviorStatus::Success;
    }),
    bt::task<Blackboard>([]([[maybe_unused]] Blackboard& blackboard) {
      ++blackboard.count;
      return gf::BehaviorStatus::Failure;
    })
  );
  gf::BehaviorStatus status = tree.process(blackboard);
  EXPECT_EQ(status, gf::BehaviorStatus::Failure);
  EXPECT_EQ(blackboard.count, 2);
}

TEST(BehaviorTreeTest, SequenceEarlyFailure) {
  Blackboard blackboard;
  auto tree =  bt::sequence<Blackboard>(
    bt::task<Blackboard>([]([[maybe_unused]] Blackboard& blackboard) {
      ++blackboard.count;
      return gf::BehaviorStatus::Failure;
    }),
    bt::task<Blackboard>([]([[maybe_unused]] Blackboard& blackboard) {
      ++blackboard.count;
      return gf::BehaviorStatus::Success;
    })
  );
  gf::BehaviorStatus status = tree.process(blackboard);
  EXPECT_EQ(status, gf::BehaviorStatus::Failure);
  EXPECT_EQ(blackboard.count, 1);
}

TEST(BehaviorTreeTest, SequenceRunning) {
  Blackboard blackboard;
  auto tree =  bt::sequence<Blackboard>(
    bt::task<Blackboard>([]([[maybe_unused]] Blackboard& blackboard) {
      ++blackboard.count;
      return gf::BehaviorStatus::Success;
    }),
    bt::task<Blackboard>([]([[maybe_unused]] Blackboard& blackboard) {
      ++blackboard.count;
      return gf::BehaviorStatus::Running;
    })
  );
  gf::BehaviorStatus status = tree.process(blackboard);
  EXPECT_EQ(status, gf::BehaviorStatus::Running);
  EXPECT_EQ(blackboard.count, 2);
}

TEST(BehaviorTreeTest, SequenceEarlyRunning) {
  Blackboard blackboard;
  auto tree =  bt::sequence<Blackboard>(
    bt::task<Blackboard>([]([[maybe_unused]] Blackboard& blackboard) {
      ++blackboard.count;
      return gf::BehaviorStatus::Running;
    }),
    bt::task<Blackboard>([]([[maybe_unused]] Blackboard& blackboard) {
      ++blackboard.count;
      return gf::BehaviorStatus::Success;
    })
  );
  gf::BehaviorStatus status = tree.process(blackboard);
  EXPECT_EQ(status, gf::BehaviorStatus::Running);
  EXPECT_EQ(blackboard.count, 1);
}

TEST(BehaviorTreeTest, SelectorSuccess) {
  Blackboard blackboard;
  auto tree =  bt::selector<Blackboard>(
    bt::task<Blackboard>([]([[maybe_unused]] Blackboard& blackboard) {
      ++blackboard.count;
      return gf::BehaviorStatus::Failure;
    }),
    bt::task<Blackboard>([]([[maybe_unused]] Blackboard& blackboard) {
      ++blackboard.count;
      return gf::BehaviorStatus::Success;
    })
  );
  gf::BehaviorStatus status = tree.process(blackboard);
  EXPECT_EQ(status, gf::BehaviorStatus::Success);
  EXPECT_EQ(blackboard.count, 2);
}

TEST(BehaviorTreeTest, SelectorEarlySuccess) {
  Blackboard blackboard;
  auto tree =  bt::selector<Blackboard>(
    bt::task<Blackboard>([]([[maybe_unused]] Blackboard& blackboard) {
      ++blackboard.count;
      return gf::BehaviorStatus::Success;
    }),
    bt::task<Blackboard>([]([[maybe_unused]] Blackboard& blackboard) {
      ++blackboard.count;
      return gf::BehaviorStatus::Failure;
    })
  );
  gf::BehaviorStatus status = tree.process(blackboard);
  EXPECT_EQ(status, gf::BehaviorStatus::Success);
  EXPECT_EQ(blackboard.count, 1);
}

TEST(BehaviorTreeTest, SelectorFailure) {
  Blackboard blackboard;
  auto tree =  bt::selector<Blackboard>(
    bt::task<Blackboard>([]([[maybe_unused]] Blackboard& blackboard) {
      ++blackboard.count;
      return gf::BehaviorStatus::Failure;
    }),
    bt::task<Blackboard>([]([[maybe_unused]] Blackboard& blackboard) {
      ++blackboard.count;
      return gf::BehaviorStatus::Failure;
    })
  );
  gf::BehaviorStatus status = tree.process(blackboard);
  EXPECT_EQ(status, gf::BehaviorStatus::Failure);
  EXPECT_EQ(blackboard.count, 2);
}

TEST(BehaviorTreeTest, SelectorRunning) {
  Blackboard blackboard;
  auto tree =  bt::selector<Blackboard>(
    bt::task<Blackboard>([]([[maybe_unused]] Blackboard& blackboard) {
      ++blackboard.count;
      return gf::BehaviorStatus::Failure;
    }),
    bt::task<Blackboard>([]([[maybe_unused]] Blackboard& blackboard) {
      ++blackboard.count;
      return gf::BehaviorStatus::Running;
    })
  );
  gf::BehaviorStatus status = tree.process(blackboard);
  EXPECT_EQ(status, gf::BehaviorStatus::Running);
  EXPECT_EQ(blackboard.count, 2);
}

TEST(BehaviorTreeTest, SelectorEarlyRunning) {
  Blackboard blackboard;
  auto tree =  bt::selector<Blackboard>(
    bt::task<Blackboard>([]([[maybe_unused]] Blackboard& blackboard) {
      ++blackboard.count;
      return gf::BehaviorStatus::Running;
    }),
    bt::task<Blackboard>([]([[maybe_unused]] Blackboard& blackboard) {
      ++blackboard.count;
      return gf::BehaviorStatus::Success;
    })
  );
  gf::BehaviorStatus status = tree.process(blackboard);
  EXPECT_EQ(status, gf::BehaviorStatus::Running);
  EXPECT_EQ(blackboard.count, 1);
}
