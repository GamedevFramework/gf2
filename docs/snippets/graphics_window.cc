#include <gf2/graphics/Window.h>
#include <gf2/graphics/Event.h>

void doc_Window() {

  // tag::loop[]
  gf::Window window("Super RPG 3000", { 1920, 1080 }, ~gf::WindowHints::Resizable);

  while (!window.should_close()) {

    while (auto event = gf::Event::poll()) {
      switch (event->type()) {
        case gf::EventType::Quit:
          window.close(); // don't forget this!
          break;

        default:
          // do nothing
          break;
      }
    }

    // ...

  }
  // end::loop[]

}
