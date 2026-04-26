// SPDX-License-Identifier: Zlib
// Copyright (c) 2023-2025 Julien Bernard
#ifndef GF_BEHAVIOR_TREE_H
#define GF_BEHAVIOR_TREE_H

#include <cassert>
#include <cstdint>

#include <memory>
#include <tuple>

namespace gf {

  enum class BehaviorStatus : uint8_t {
    Success,
    Running,
    Failure,
  };

  template<typename Blackboard>
  class BehaviorNode {
  public:
    BehaviorNode() = default;
    BehaviorNode(const BehaviorNode&) = delete;
    BehaviorNode(BehaviorNode&&) noexcept = default;
    virtual ~BehaviorNode() = default;
    BehaviorNode& operator=(const BehaviorNode&) = delete;
    BehaviorNode& operator=(BehaviorNode&&) noexcept = default;

    virtual BehaviorStatus process(Blackboard& blackboard) const = 0;
  };

  namespace behavior {

    namespace details {
      // inspired by https://accu.org/journals/overload/25/139/williams_2382/

      template<typename Blackboard, typename Tuple, std::size_t I>
      const BehaviorNode<Blackboard>& behavior_tuple_get(Tuple& tuple)
      {
        return std::get<I>(tuple);
      }

      template<typename Blackboard, typename Tuple, typename Indices = std::make_index_sequence<std::tuple_size_v<Tuple>>>
      struct BehaviorTupleGetters;

      template<typename Blackboard, typename Tuple, std::size_t... Indices>
      struct BehaviorTupleGetters<Blackboard, Tuple, std::index_sequence<Indices...>> {
        using Getter = const BehaviorNode<Blackboard>& (*)(Tuple&);
        static constexpr Getter Table[std::tuple_size_v<Tuple>] = { &behavior_tuple_get<Blackboard, Tuple, Indices>... };
      };

      template<typename Blackboard, typename Tuple>
      const BehaviorNode<Blackboard>& behavior_tuple_get_ith(Tuple& tuple, std::size_t i)
      {
        assert(i < std::tuple_size_v<Tuple>);
        return BehaviorTupleGetters<Blackboard, Tuple>::Table[i](tuple);
      }

    }

    /*
     * Sequence
     */

    template<typename Blackboard, typename ... Args>
    class Sequence : public BehaviorNode<Blackboard> {
    public:
      Sequence(Args... children)
      : m_children(std::forward<Args>(children)...)
      {
      }

      BehaviorStatus process(Blackboard& blackboard) const override
      {
        for (std::size_t i = 0; i < sizeof...(Args); ++i) {
          const BehaviorNode<Blackboard>& behavior = details::behavior_tuple_get_ith<Blackboard>(m_children, i);
          const BehaviorStatus status = behavior.process(blackboard);

          if (status != BehaviorStatus::Success) {
            return status;
          }
        }

        return BehaviorStatus::Success;
      }

    private:
      std::tuple<Args...> m_children;
    };

    template<typename Blackboard, typename ... Args>
    Sequence<Blackboard, Args...> sequence(Args&&... behaviors)
    {
      return { std::forward<Args>(behaviors)... };
    }

    /*
     * Selector
     */

    template<typename Blackboard, typename ... Args>
    class Selector : public BehaviorNode<Blackboard> {
    public:
      Selector(Args... children)
      : m_children(std::forward<Args>(children)...)
      {
      }

      BehaviorStatus process(Blackboard& blackboard) const override
      {
        for (std::size_t i = 0; i < sizeof...(Args); ++i) {
          const BehaviorNode<Blackboard>& behavior = details::behavior_tuple_get_ith<Blackboard>(m_children, i);
          const BehaviorStatus status = behavior.process(blackboard);

          if (status != BehaviorStatus::Failure) {
            return status;
          }
        }

        return BehaviorStatus::Failure;
      }

    private:
      std::tuple<Args...> m_children;
    };

    template<typename Blackboard, typename ... Args>
    Selector<Blackboard, Args...> selector(Args&&... behaviors)
    {
      return { std::forward<Args>(behaviors)... };
    }

    /*
     * Invert
     */

    template<typename Blackboard, typename Child>
    class Invert : public BehaviorNode<Blackboard> {
    public:
      Invert(Child child)
      : m_child(std::move(child))
      {
      }

      BehaviorStatus process(Blackboard& blackboard) const override
      {
        const BehaviorStatus status = m_child.process(blackboard);

        if (status == BehaviorStatus::Failure) {
          return BehaviorStatus::Success;
        }

        if (status == BehaviorStatus::Success) {
          return BehaviorStatus::Failure;
        }

        assert(false);
        return BehaviorStatus::Running;
      }

    private:
      Child m_child;
    };

    template<typename Blackboard, typename Child>
    Invert<Blackboard, Child> invert(Child&& behavior)
    {
      return { std::forward<Child>(behavior) };
    }

    /*
     * Succeed
     */

    template<typename Blackboard, typename Child>
    class Succeed : public BehaviorNode<Blackboard> {
    public:
      Succeed(Child child)
      : m_child(std::move(child))
      {
      }

      BehaviorStatus process(Blackboard& blackboard) const override
      {
        const BehaviorStatus status = m_child.process(blackboard);

        if (status == BehaviorStatus::Running) {
          return BehaviorStatus::Running;
        }

        return BehaviorStatus::Success;
      }

    private:
      Child m_child;
    };

    template<typename Blackboard, typename Child>
    Succeed<Blackboard, Child> succeed(Child&& behavior)
    {
      return { std::forward<Child>(behavior) };
    }

    /*
     * Repeat
     */

    template<typename Blackboard, typename Child>
    class Repeat : public BehaviorNode<Blackboard> {
    public:
      Repeat(Child child)
      : m_child(std::move(child))
      {
      }

      BehaviorStatus process(Blackboard& blackboard) const override
      {
        for (;;) {
          const BehaviorStatus status = m_child.process(blackboard);

          if (status == BehaviorStatus::Failure) {
            break;
          }
        }

        return BehaviorStatus::Success;
      }

    private:
      Child m_child;
    };

    template<typename Blackboard, typename Child>
    Repeat<Blackboard, Child> repeat(Child&& behavior)
    {
      return { std::forward<Child>(behavior) };
    }

    /*
     * Condition
     */

    template<typename Blackboard, typename Func>
    class Condition : public BehaviorNode<Blackboard> {
    public:
      Condition(Func func)
      : m_func(std::move(func))
      {
      }

      BehaviorStatus process(Blackboard& blackboard) const override
      {
        if (m_func(blackboard)) {
          return BehaviorStatus::Success;
        }

        return BehaviorStatus::Failure;
      }

    private:
      Func m_func;
    };

    template<typename Blackboard, typename Func>
    Condition<Blackboard, Func> condition(Func&& func)
    {
      return { std::forward<Func>(func) };
    }


    /*
     * Task
     */

    template<typename Blackboard, typename Func>
    class Task : public BehaviorNode<Blackboard> {
    public:
      Task(Func func)
      : m_func(std::move(func))
      {
      }

      BehaviorStatus process(Blackboard& blackboard) const override
      {
        return m_func(blackboard);
      }

    private:
      Func m_func;
    };

    template<typename Blackboard, typename Func>
    Task<Blackboard, Func> task(Func&& func)
    {
      return { std::forward<Func>(func) };
    }

    /*
     * AnyBehavior
     */

    template<typename Blackboard>
    class AnyBehavior : public BehaviorNode<Blackboard> {
    public:
      template<typename Child>
      explicit AnyBehavior(Child behavior)
      : m_child(std::make_unique<Child>(std::move(behavior)))
      {
      }

      BehaviorStatus process(Blackboard& blackboard) const override
      {
        return m_child->process(blackboard);
      }

    private:
      std::unique_ptr<BehaviorNode<Blackboard>> m_child;
    };


  }

}

#endif // GF_BEHAVIOR_TREE_H

