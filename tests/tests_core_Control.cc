#include <gf2/core/Control.h>

#include <gtest/gtest.h>

#include <gf2/core/Event.h>

namespace {

  constexpr uint64_t DefaultTimestamp = 0xDEADBEEF;

}

TEST(ControlTest, SimpleScancode) {
  gf::ControlSettings settings(gf::Scancode::Q);
  gf::Control control(settings);

  EXPECT_EQ(control.settings().type(), gf::ControlType::Scancode);

  {
    // correct
    EXPECT_FALSE(control.active());

    gf::KeyPressedEvent event = {};
    event.keycode = gf::Keycode::A;
    event.scancode = gf::Scancode::Q;

    control.process_event({ DefaultTimestamp, event });
    EXPECT_TRUE(control.active());
    control.reset();
  }
  {
    // wrong scancode
    EXPECT_FALSE(control.active());

    gf::KeyPressedEvent event = {};
    event.keycode = gf::Keycode::Q;
    event.scancode = gf::Scancode::A;

    control.process_event({ DefaultTimestamp, event });
    EXPECT_FALSE(control.active());
    control.reset();
  }
  {
    // wrong modifier
    EXPECT_FALSE(control.active());

    gf::KeyPressedEvent event = {};
    event.keycode = gf::Keycode::A;
    event.scancode = gf::Scancode::Q;
    event.modifiers = gf::Modifier::Ctrl;

    control.process_event({ DefaultTimestamp, event });
    EXPECT_FALSE(control.active());
    control.reset();
  }
}

TEST(ControlTest, SimpleKeycode) {
  gf::ControlSettings settings(gf::Keycode::A);
  gf::Control control(settings);

  EXPECT_EQ(control.settings().type(), gf::ControlType::Keycode);

  {
    // correct
    EXPECT_FALSE(control.active());

    gf::KeyPressedEvent event = {};
    event.keycode = gf::Keycode::A;
    event.scancode = gf::Scancode::Q;

    control.process_event({ DefaultTimestamp, event });
    EXPECT_TRUE(control.active());
    control.reset();
  }
  {
    // wrong keycode
    EXPECT_FALSE(control.active());

    gf::KeyPressedEvent event = {};
    event.keycode = gf::Keycode::Q;
    event.scancode = gf::Scancode::A;

    control.process_event({ DefaultTimestamp, event });
    EXPECT_FALSE(control.active());
    control.reset();
  }
  {
    // wrong modifier
    EXPECT_FALSE(control.active());

    gf::KeyPressedEvent event = {};
    event.keycode = gf::Keycode::A;
    event.scancode = gf::Scancode::Q;
    event.modifiers = gf::Modifier::LeftShift;

    control.process_event({ DefaultTimestamp, event });
    EXPECT_FALSE(control.active());
    control.reset();
  }
}

TEST(ControlTest, ScancodeModifier) {
  gf::ControlSettings settings(gf::Scancode::Q, gf::Modifier::LeftCtrl);
  gf::Control control(settings);

  EXPECT_EQ(control.settings().type(), gf::ControlType::Scancode);

  {
    // correct
    EXPECT_FALSE(control.active());

    gf::KeyPressedEvent event = {};
    event.keycode = gf::Keycode::A;
    event.scancode = gf::Scancode::Q;
    event.modifiers = gf::Modifier::LeftCtrl;

    control.process_event({ DefaultTimestamp, event });
    EXPECT_TRUE(control.active());
    control.reset();
  }
  {
    // no modifier
    EXPECT_FALSE(control.active());

    gf::KeyPressedEvent event = {};
    event.keycode = gf::Keycode::A;
    event.scancode = gf::Scancode::Q;

    control.process_event({ DefaultTimestamp, event });
    EXPECT_FALSE(control.active());
    control.reset();
  }
  {
    // wrong modifier
    EXPECT_FALSE(control.active());

    gf::KeyPressedEvent event = {};
    event.keycode = gf::Keycode::A;
    event.scancode = gf::Scancode::Q;
    event.modifiers = gf::Modifier::LeftShift;

    control.process_event({ DefaultTimestamp, event });
    EXPECT_FALSE(control.active());
    control.reset();
  }
  {
    // wrong scancode
    EXPECT_FALSE(control.active());

    gf::KeyPressedEvent event = {};
    event.keycode = gf::Keycode::Q;
    event.scancode = gf::Scancode::A;
    event.modifiers = gf::Modifier::LeftCtrl;

    control.process_event({ DefaultTimestamp, event });
    EXPECT_FALSE(control.active());
    control.reset();
  }
}

TEST(ControlTest, KeycodeModifier) {
  gf::ControlSettings settings(gf::Keycode::A, gf::Modifier::LeftAlt);
  gf::Control control(settings);

  EXPECT_EQ(control.settings().type(), gf::ControlType::Keycode);

  {
    // correct
    EXPECT_FALSE(control.active());

    gf::KeyPressedEvent event = {};
    event.keycode = gf::Keycode::A;
    event.scancode = gf::Scancode::Q;
    event.modifiers = gf::Modifier::LeftAlt;

    control.process_event({ DefaultTimestamp, event });
    EXPECT_TRUE(control.active());
    control.reset();
  }
  {
    // no modifier
    EXPECT_FALSE(control.active());

    gf::KeyPressedEvent event = {};
    event.keycode = gf::Keycode::A;
    event.scancode = gf::Scancode::Q;

    control.process_event({ DefaultTimestamp, event });
    EXPECT_FALSE(control.active());
    control.reset();
  }
  {
    // correct
    EXPECT_FALSE(control.active());

    gf::KeyPressedEvent event = {};
    event.keycode = gf::Keycode::A;
    event.scancode = gf::Scancode::Q;
    event.modifiers = gf::Modifier::LeftCtrl;

    control.process_event({ DefaultTimestamp, event });
    EXPECT_FALSE(control.active());
    control.reset();
  }
  {
    // wrong keycode
    EXPECT_FALSE(control.active());

    gf::KeyPressedEvent event = {};
    event.keycode = gf::Keycode::Q;
    event.scancode = gf::Scancode::A;
    event.modifiers = gf::Modifier::LeftAlt;

    control.process_event({ DefaultTimestamp, event });
    EXPECT_FALSE(control.active());
    control.reset();
  }
}
