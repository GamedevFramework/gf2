#include <gf2/graphics/Event.h>

void doc_Event() {

  // tag::loop[]
  while (auto event = gf::Event::poll()) {
    switch (event->type) {
      case gf::EventType::Quit:
        // close the window
        break;

      default:
        // do nothing
        break;
    }
  }
  // end::loop[]

}
