// SPDX-License-Identifier: Zlib
// Copyright (c) 2023 Julien Bernard
#ifndef GF_SHADER_DATA_H
#define GF_SHADER_DATA_H

#include <gf2/core/Mat3.h>
#include <gf2/core/Mat4.h>

namespace gf {

  template<typename T>
  struct ShaderData {
    using Type = T;
  };

  template<>
  struct ShaderData<Mat3F> {
    using Type = Mat4F;
  };

  template<typename T>
  using ShaderDataType = typename ShaderData<T>::Type;

}

#endif // GF_SHADER_DATA_H
