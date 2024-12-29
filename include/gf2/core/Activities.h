// SPDX-License-Identifier: Zlib
// Copyright (c) 2023 Julien Bernard
#ifndef GF_ACTIVITIES_H
#define GF_ACTIVITIES_H

#include <memory>
#include <tuple>

#include "Activity.h"
#include "Color.h"
#include "CoreApi.h"
#include "Easing.h"
#include "Tween.h"
#include "TypeTraits.h"
#include "Vec2.h"

namespace gf {

  class GF_CORE_API ValueActivity : public Activity {
  public:
    ValueActivity(float origin, float target, float* value, Time duration, Easing easing = ease_linear);

    void set_origin(float origin)
    {
      m_tween.set_origin(origin);
    }

    float origin() const noexcept
    {
      return m_tween.origin();
    }

    void set_target(float target)
    {
      m_tween.set_target(target);
    }

    float target() const noexcept
    {
      return m_tween.target();
    }

    void set_duration(Time duration)
    {
      m_tween.set_duration(duration);
    }

    Time duration() const noexcept
    {
      return m_tween.duration();
    }

    ActivityStatus run(Time time) override;
    void restart() override;

  private:
    Tween<float> m_tween;
  };

  class GF_CORE_API RotationActivity : public Activity {
  public:
    RotationActivity(float origin, float target, float* angle, Time duration, Easing easing = ease_linear);

    void set_origin(float origin)
    {
      m_tween.set_origin(origin);
      normalize();
    }

    float origin() const noexcept
    {
      return m_tween.origin();
    }

    void set_target(float target)
    {
      m_tween.set_target(target);
      normalize();
    }

    float target() const noexcept
    {
      return m_tween.target();
    }

    void set_duration(Time duration)
    {
      m_tween.set_duration(duration);
    }

    Time duration() const noexcept
    {
      return m_tween.duration();
    }

    ActivityStatus run(Time time) override;
    void restart() override;

  private:
    void normalize();

    Tween<float> m_tween;
  };

  class GF_CORE_API MotionActivity : public Activity {
  public:
    MotionActivity(Vec2F origin, Vec2F target, Vec2F* position, Time duration, Easing easing = ease_linear);

    void set_origin(Vec2F origin)
    {
      m_tween.set_origin(origin);
    }

    Vec2F origin() const noexcept
    {
      return m_tween.origin();
    }

    void set_target(Vec2F target)
    {
      m_tween.set_target(target);
    }

    Vec2F target() const noexcept
    {
      return m_tween.target();
    }

    void set_duration(Time duration)
    {
      m_tween.set_duration(duration);
    }

    Time duration() const noexcept
    {
      return m_tween.duration();
    }

    ActivityStatus run(Time time) override;
    void restart() override;

  private:
    Tween<Vec2F> m_tween;
  };

  class GF_CORE_API ColorActivity : public Activity {
  public:
    ColorActivity(Color origin, Color target, Color* color, Time duration, Easing easing = ease_linear);

    void set_origin(Color origin)
    {
      m_tween.set_origin(origin);
    }

    Color origin() const noexcept
    {
      return m_tween.origin();
    }

    void set_target(Color target)
    {
      m_tween.set_target(target);
    }

    Color target() const noexcept
    {
      return m_tween.target();
    }

    void set_duration(Time duration)
    {
      m_tween.set_duration(duration);
    }

    Time duration() const noexcept
    {
      return m_tween.duration();
    }

    ActivityStatus run(Time time) override;
    void restart() override;

  private:
    Tween<Color> m_tween;
  };

  class GF_CORE_API CallbackActivity : public Activity {
  public:
    template<typename Func, typename = PreventHiddenCopyMove<Func, CallbackActivity>>
    CallbackActivity(Func&& callback)
    : m_callback(std::forward<Func>(callback))
    {
    }

    ActivityStatus run(Time time) override;
    void restart() override;

  private:
    std::function<void()> m_callback;
    bool m_called = false;
  };

  class GF_CORE_API DelayActivity : public Activity {
  public:
    DelayActivity(Time duration);

    ActivityStatus run(Time time) override;
    void restart() override;

  private:
    Time m_elapsed;
    Time m_duration;
  };

  class GF_CORE_API SequenceActivity : public Activity {
  public:
    void add_activity(Activity* activity);
    void clear();

    ActivityStatus run(Time time) override;
    void restart() override;

  private:
    std::size_t m_current = 0;
    std::vector<Activity*> m_activities;
  };

  class GF_CORE_API RepeatActivity : public Activity {
  public:
    RepeatActivity(Activity* activity, int repeat = 0);

    ActivityStatus run(Time time) override;
    void restart() override;

  private:
    Activity* m_activity;
    int m_count = 0;
    int m_repeat = 0;
  };

  enum class ActivityFinish : uint8_t {
    Any,
    All,
  };

  class GF_CORE_API ParallelActivity : public Activity {
  public:
    ParallelActivity(ActivityFinish finish = ActivityFinish::Any);

    void add_activity(Activity* activity);
    void clear();

    ActivityStatus run(Time time) override;
    void restart() override;

  private:
    ActivityFinish m_finish = ActivityFinish::Any;
    ActivityStatus m_status = ActivityStatus::Running;
    std::vector<Activity*> m_activities;
  };

  namespace activity {

    inline ValueActivity value(float origin, float target, float* value, Time duration, Easing easing = ease_linear)
    {
      return { origin, target, value, duration, easing };
    }

    inline RotationActivity rotation(float origin, float target, float* value, Time duration, Easing easing = ease_linear)
    {
      return { origin, target, value, duration, easing };
    }

    inline MotionActivity motion(Vec2F origin, Vec2F target, Vec2F* value, Time duration, Easing easing = ease_linear)
    {
      return { origin, target, value, duration, easing };
    }

    inline ColorActivity color(Color origin, Color target, Color* value, Time duration, Easing easing = ease_linear)
    {
      return { origin, target, value, duration, easing };
    }

    template<typename Func>
    CallbackActivity call(Func&& callback)
    {
      return { std::forward<Func>(callback) };
    }

    inline DelayActivity delay(Time duration)
    {
      return { duration };
    }

    namespace details {
      // inspired by https://accu.org/journals/overload/25/139/williams_2382/

      template<typename Tuple, std::size_t I>
      Activity& activity_tuple_get(Tuple& tuple)
      {
        return std::get<I>(tuple);
      }

      template<typename Tuple, typename Indices = std::make_index_sequence<std::tuple_size_v<Tuple>>>
      struct ActivityTupleGetters;

      template<typename Tuple, std::size_t... Indices>
      struct ActivityTupleGetters<Tuple, std::index_sequence<Indices...>> {
        using Getter = Activity& (*)(Tuple&);
        static constexpr Getter Table[std::tuple_size_v<Tuple>] = { &activity_tuple_get<Tuple, Indices>... };
      };

      template<typename Tuple>
      Activity& activity_tuple_get_ith(Tuple& tuple, std::size_t i)
      {
        assert(i < std::tuple_size_v<Tuple>);
        return ActivityTupleGetters<Tuple>::Table[i](tuple);
      }

    }

    template<typename... Args>
    class Sequence : public Activity {
    public:
      Sequence(Args... activities)
      : m_activities(std::forward<Args>(activities)...)
      {
      }

      ActivityStatus run(Time time) override
      {
        if (m_current == sizeof...(Args)) {
          return ActivityStatus::Finished;
        }

        Activity& activity = details::activity_tuple_get_ith(m_activities, m_current);
        ActivityStatus status = activity.run(time);

        if (status == ActivityStatus::Finished) {
          m_current++;
        }

        return m_current == sizeof...(Args) ? ActivityStatus::Finished : ActivityStatus::Running;
      }

      void restart() override
      {
        m_current = 0;

        for (std::size_t i = 0; i < sizeof...(Args); ++i) {
          details::activity_tuple_get_ith(m_activities, i).restart();
        }
      }

    private:
      std::size_t m_current = 0;
      std::tuple<Args...> m_activities;
    };

    template<typename... Args>
    Sequence<Args...> sequence(Args... activities)
    {
      return { std::forward<Args>(activities)... };
    }

    template<typename Other>
    class Repeat : public Activity {
    public:
      Repeat(Other activity, int repeat)
      : m_activity(std::move(activity))
      , m_repeat(repeat)
      {
      }

      ActivityStatus run(Time time) override
      {
        if (m_repeat > 0 && m_repeat == m_count) {
          return ActivityStatus::Finished;
        }

        const ActivityStatus status = m_activity.run(time);

        if (status == ActivityStatus::Finished) {
          m_activity.restart();
          m_count++;
        }

        return (m_repeat > 0 && m_repeat == m_count) ? ActivityStatus::Finished : ActivityStatus::Running;
      }

      void restart() override
      {
        m_count = 0;
        m_activity.restart();
      }

    private:
      Other m_activity;
      int m_count = 0;
      int m_repeat;
    };

    template<typename Other>
    Repeat<Other> repeat(Other activity, int repeat = 0)
    {
      return { std::move(activity), repeat };
    }

    template<typename... Args>
    class Parallel : public Activity {
    public:
      Parallel(ActivityFinish finish, Args... activities)
      : m_finish(finish)
      , m_activities(std::forward<Args>(activities)...)
      {
      }

      ActivityStatus run(Time time) override
      {
        if (m_status == ActivityStatus::Finished) {
          return ActivityStatus::Finished;
        }

        std::size_t finished = 0;

        for (std::size_t i = 0; i < sizeof...(Args); ++i) {
          const ActivityStatus status = details::activity_tuple_get_ith(m_activities, i).run(time);

          if (status == ActivityStatus::Finished) {
            ++finished;
          }
        }

        switch (m_finish) {
          case ActivityFinish::Any:
            if (finished > 0) {
              m_status = ActivityStatus::Finished;
            }
            break;

          case ActivityFinish::All:
            if (finished == sizeof...(Args)) {
              m_status = ActivityStatus::Finished;
            }
            break;
        }

        return m_status;
      }

      void restart() override
      {
        m_status = ActivityStatus::Running;

        for (std::size_t i = 0; i < sizeof...(Args); ++i) {
          details::activity_tuple_get_ith(m_activities, i).restart();
        }
      }

    private:
      ActivityFinish m_finish = ActivityFinish::Any;
      ActivityStatus m_status = ActivityStatus::Running;
      std::tuple<Args...> m_activities;
    };

    template<typename... Args>
    Parallel<Args...> parallel_any(Args... activities)
    {
      return { ActivityFinish::Any, std::forward<Args>(activities)... };
    }

    template<typename... Args>
    Parallel<Args...> parallel_all(Args... activities)
    {
      return { ActivityFinish::All, std::forward<Args>(activities)... };
    }

    class AnyActivity : public Activity {
    public:
      template<typename Other>
      explicit AnyActivity(Other other)
      : m_activity(std::make_unique<Other>(std::move(other)))
      {
      }

      ActivityStatus run(Time time) override
      {
        return m_activity->run(time);
      }

      void restart() override
      {
        m_activity->restart();
      }

    private:
      std::unique_ptr<Activity> m_activity;
    };

  }

}

#endif // GF_ACTIVITIES_H
