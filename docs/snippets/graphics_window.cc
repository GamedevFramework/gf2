#include <gf2/Window.h>
#include <gf2/Event.h>

void doc_Window() {

  // tag::loop[]
  gf::Window window("", { 1920, 1080 }, ~gf::WindowHints::Resizable);

  while (!window.should_close()) {

    while (auto event = gf::Event::poll()) {
      switch (event->type) {
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
