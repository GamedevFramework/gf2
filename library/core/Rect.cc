#include <gf2/Rect.h>

namespace gf {

#ifndef _MSC_VER
  template struct Rect<float>;
  template struct Rect<double>;
  template struct Rect<int>;
  template struct Rect<unsigned>;
#endif

}
