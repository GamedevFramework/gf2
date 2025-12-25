// SPDX-License-Identifier: Zlib
// Copyright (c) 2023-2025 Julien Bernard

#include <gf2/physics/PhysicsTaskManager.h>

#include <cassert>

#include <gf2/core/Log.h>

namespace gf {

  PhysicsTaskManager::PhysicsTaskManager(std::size_t thread_count)
  : m_started_workers(thread_count)
  {
    m_started_workers = std::min(m_started_workers, MaxWorkers);

    for (std::size_t i = 0; i < m_started_workers; ++i) {
      std::thread thread(&PhysicsTaskManager::run, this, i);
      m_workers[i].thread = std::move(thread);
    }

    Log::info("{} physics thread started.", m_started_workers);
  }

  PhysicsTaskManager::~PhysicsTaskManager() {
    for (std::size_t i = 0; i < m_started_workers; ++i) {
      m_workers[i].work.push({ nullptr, 0, 0, nullptr });
    }

    for (std::size_t i = 0; i < m_started_workers; ++i) {
      m_workers[i].thread.join();
    }
  }

  void* PhysicsTaskManager::enqueue_task(b2TaskCallback* task, int item_count, int min_range, void* task_context, void* user_context)
  {
    if (item_count <= min_range) {
      // do it alone
      task(0, item_count, 0, task_context);
      return nullptr;
    }

    auto* task_manager = static_cast<PhysicsTaskManager*>(user_context);
    return task_manager->enqueue(task, item_count, min_range, task_context);
  }

  void PhysicsTaskManager::finish_task(void* user_task, void* user_context)
  {
    auto* task_manager = static_cast<PhysicsTaskManager*>(user_context);
    task_manager->finish(user_task);
  }

  void* PhysicsTaskManager::enqueue(b2TaskCallback* task, int item_count, int min_range, void* task_context)
  {
    // determine the number of workers and the range of work

    // Log::info("item_count: {}, min_range: {}", item_count, min_range);

    const std::size_t min_workers = item_count / min_range;
    assert(min_workers >= 1);
    m_current_workers = std::min(m_started_workers, min_workers);

    // Log::info("current_workers: {}", m_current_workers);

    int range = item_count / static_cast<int>(m_current_workers + 1);
    range = std::max(range, min_range);

    // Log::info("range: {}", range);

    // distribute the work

    int start = 0;

    for (std::size_t i = 0; i < m_current_workers; ++i) {
      // Log::info("> {}, {}", start, start + range);
      assert(range <= item_count);
      m_workers[i].work.push({ task, start, start + range, task_context });
      start += range;
      item_count -= range;
    }

    // take the rest
    // Log::info("take the rest: {}", item_count);

    if (item_count > 0) {
      // Log::info("* {}, {}", start, start + item_count);
      task(start, start + item_count, 0, task_context);
    }

    return this;
  }

  void PhysicsTaskManager::finish([[maybe_unused]] void* user_task)
  {
    for (std::size_t i = 0; i < m_current_workers; ++i) {
      // Log::info("Waiting for worker {}", i);
      m_finish.wait();
    }

    m_current_workers = 0;
  }

  void PhysicsTaskManager::run(uint32_t worker_index)
  {
    for (;;) {
      Work work = m_workers[worker_index].work.wait();

      if (work.task == nullptr) {
        return;
      }

      work.task(work.start_index, work.end_index, worker_index + 1, work.task_context);
      // Log::info("Finished {}", worker_index);
      m_finish.push({});
    }
  }

}
