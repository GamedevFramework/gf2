#include <gf2/graphics/Events.h>

void doc_Event() {

  // tag::loop[]
  while (auto event = gf::Events::poll()) {
    switch (event->type()) {
      case gf::EventType::Quit:
        // close the window
        break;

      default:
        // do nothing
        break;
    }
  }
  // end::loop[]

  // tag::specific[]
  while (auto event = gf::Events::poll()) {
    switch (event->type()) {
      case gf::EventType::Quit:
        // close the window
        break;

      case gf::EventType::KeyPressed:
        {
          const auto& key_pressed_event = event->from<gf::EventType::KeyPressed>();
          // do something with key_pressed_event
        }
        break;

      default:
        // do nothing
        break;
    }
  }
  // end::specific[]

}
