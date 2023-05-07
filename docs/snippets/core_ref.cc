#include <gf2/Ref.h>

// tag::ref_func[]
void function_with_ref(gf::Ref<int> ref);
// end::ref_func[]

void doc_Ref() {

// tag::ref[]
  int data = 42;
  function_with_ref(gf::ref(data));
// end::ref[]

}
