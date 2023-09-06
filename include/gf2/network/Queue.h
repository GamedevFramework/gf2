// SPDX-License-Identifier: Zlib
// Copyright (c) 2023 Julien Bernard
#ifndef GF_QUEUE_H
#define GF_QUEUE_H

#include <condition_variable>
#include <deque>
#include <mutex>
#include <optional>

namespace gf {

  template<typename T>
  class Queue {
  public:
    std::optional<T> poll()
    {
      std::lock_guard<std::mutex> lock(m_mutex);

      if (m_queue.empty()) {
        return std::nullopt;
      }

      T value = m_queue.front();
      m_queue.pop_front();
      return value;
    }

    T wait()
    {
      std::unique_lock<std::mutex> lock(m_mutex);
      m_condition.wait(lock, [this]() { return !m_queue.empty(); });
      T value = m_queue.front();
      m_queue.pop_front();
      return value;
    }

    void push(const T& value)
    {
      std::lock_guard<std::mutex> lock(m_mutex);
      m_queue.push_back(value);
      m_condition.notify_one();
    }

    void push(T&& value)
    {
      std::lock_guard<std::mutex> lock(m_mutex);
      m_queue.push_back(std::move(value));
      m_condition.notify_one();
    }

    void clear()
    {
      std::lock_guard<std::mutex> lock(m_mutex);
      m_queue.clear();
    }

  private:
    std::mutex m_mutex;
    std::condition_variable m_condition;
    std::deque<T> m_queue;
  };

} // namespace gf

#endif // GF_QUEUE_H
