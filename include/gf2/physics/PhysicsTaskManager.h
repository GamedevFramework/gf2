// SPDX-License-Identifier: Zlib
// Copyright (c) 2023-2025 Julien Bernard
#ifndef GF_PHYSICS_TASK_MANAGER_H
#define GF_PHYSICS_TASK_MANAGER_H

#include <cstdint>

#include <array>
#include <thread>

#include <box2d/box2d.h>

#include <gf2/core/Queue.h>

namespace gf {

  class PhysicsTaskManager {
  public:
    PhysicsTaskManager(std::size_t thread_count);
    PhysicsTaskManager(const PhysicsTaskManager&) = delete;
    PhysicsTaskManager(PhysicsTaskManager&&) noexcept = delete;
    ~PhysicsTaskManager();

    PhysicsTaskManager& operator=(const PhysicsTaskManager&) = delete;
    PhysicsTaskManager& operator=(PhysicsTaskManager&&) noexcept = delete;

    int worker_count() const
    {
      return static_cast<int>(m_started_workers + 1);
    }

    static void* enqueue_task(b2TaskCallback* task, int item_count, int min_range, void* task_context, void* user_context);
    static void finish_task(void* user_task, void* user_context);

  private:
    void* enqueue(b2TaskCallback* task, int item_count, int min_range, void* task_context);
    void finish(void* user_task);
    void run(uint32_t worker_index);

    struct Work {
      b2TaskCallback* task;
      int start_index;
      int end_index;
      void* task_context;
    };

    struct Worker {
      Queue<Work> work;
      std::thread thread;
    };

    static constexpr std::size_t MaxWorkers = 16;

    std::size_t m_started_workers = 0;
    std::size_t m_current_workers = 0;
    std::array<Worker, MaxWorkers> m_workers;

    struct Token {};

    Queue<Token> m_finish;
  };

}

#endif // GF_PHYSICS_TASK_MANAGER_H
