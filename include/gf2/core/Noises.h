// SPDX-License-Identifier: Zlib
// Copyright (c) 2023-2025 Julien Bernard
#ifndef GF_NOISES_H
#define GF_NOISES_H

#include <cstdint>

#include <array>

#include "CoreApi.h"
#include "Math.h"
#include "Noise.h"
#include "Random.h"
#include "Vec2.h"
#include "Vec3.h"

namespace gf {

  class GF_CORE_API ValueNoise2D : public Noise2D {
  public:
    ValueNoise2D(Random* random, Step<float> step);

    float value(float x, float y) final;

  private:
    float at(uint8_t i, uint8_t j) const;

    Step<float> m_step;
    std::array<uint8_t, 256> m_permutation = {};
    std::array<float, 256> m_values = {};
  };

  class GF_CORE_API GradientNoise2D : public Noise2D {
  public:
    GradientNoise2D(Random* random, Step<float> step);

    float value(float x, float y) final;

  private:
    Vec2F at(uint8_t i, uint8_t j) const;

    Step<float> m_step;
    std::array<uint8_t, 256> m_permutation = {};
    std::array<Vec2F, 256> m_gradients = {};
  };

  class GF_CORE_API GradientNoise3D : public Noise3D {
  public:
    GradientNoise3D(Random* random, Step<float> step);

    float value(float x, float y, float z) final;

  private:
    Vec3F at(uint8_t i, uint8_t j, uint8_t k) const;

    Step<float> m_step;
    std::array<uint8_t, 256> m_permutation = {};
    std::array<Vec3F, 256> m_gradients = {};
  };

  class GF_CORE_API BetterGradientNoise2D : public Noise2D {
  public:
    BetterGradientNoise2D(Random* random);

    float value(float x, float y) final;

  private:
    Vec2F at(uint8_t i, uint8_t j) const;

    std::array<uint8_t, 256> m_permutation_x = {};
    std::array<uint8_t, 256> m_permutation_y = {};
    std::array<Vec2F, 256> m_gradients = {};
  };

  class GF_CORE_API FractalNoise2D : public Noise2D {
  public:
    FractalNoise2D(Noise2D* noise, float scale, int octaves = 8, float lacunarity = 2.0f, float persistence = 0.5f, float dimension = 1.0f);

    float value(float x, float y) final;

  private:
    Noise2D* m_noise = nullptr;
    float m_scale = 1.0f;
    int m_octaves = 8;
    float m_lacunarity = 2.0f;
    float m_persistence = 0.5f;
    float m_dimension = 1.0f;
  };

  class GF_CORE_API FractalNoise3D : public Noise3D {
  public:
    FractalNoise3D(Noise3D* noise, float scale, int octaves = 8, float lacunarity = 2.0f, float persistence = 0.5f, float dimension = 1.0f);

    float value(float x, float y, float z) final;

  private:
    Noise3D* m_noise = nullptr;
    float m_scale = 1.0f;
    int m_octaves = 8;
    float m_lacunarity = 2.0f;
    float m_persistence = 0.5f;
    float m_dimension = 1.0f;
  };

  class GF_CORE_API PerlinNoise2D : public Noise2D {
  public:
    PerlinNoise2D(Random* random, float scale, int octaves = 8);

    float value(float x, float y) final;

  private:
    GradientNoise2D m_gradient_noise;
    FractalNoise2D m_fractal_noise;
  };

  class GF_CORE_API PerlinNoise3D : public Noise3D {
  public:
    PerlinNoise3D(Random* random, float scale, int octaves = 8);

    float value(float x, float y, float z) final;

  private:
    GradientNoise3D m_gradient_noise;
    FractalNoise3D m_fractal_noise;
  };

  class GF_CORE_API SimplexNoise2D : public Noise2D {
  public:
    SimplexNoise2D(Random* random);

    float value(float x, float y) final;

  private:
    Vec2F at(uint8_t i, uint8_t j) const;

    std::array<uint8_t, 256> m_permutation{};
  };

  class GF_CORE_API WaveletNoise3D : public Noise3D {
  public:
    WaveletNoise3D(Random* random, std::ptrdiff_t wavelet_tile_size = 32);

    float value(float x, float y, float z) final;

  private:
    std::ptrdiff_t m_wavelet_tile_size;
    std::vector<float> m_data;
  };

  class GF_CORE_API WorleyNoise2D : public Noise2D {
  public:
    WorleyNoise2D(Random* random, std::size_t points_count, Distance2<float> distance, std::vector<float> coefficients);

    float value(float x, float y) final;

  private:
    std::size_t m_points_count;
    Distance2<float> m_distance;
    std::vector<float> m_coefficients;
    std::vector<Vec2F> m_cells;
  };

  class GF_CORE_API Multifractal2D : public Noise2D {
  public:
    Multifractal2D(Noise2D* noise, float scale, int octaves = 8, float lacunarity = 2.0f, float persistence = 0.5f, float dimension = 1.0f);

    float value(float x, float y) final;

  private:
    Noise2D* m_noise = nullptr;
    float m_scale = 1.0f;
    int m_octaves = 8;
    float m_lacunarity = 2.0f;
    float m_persistence = 0.5f;
    float m_dimension = 1.0f;
  };

  class GF_CORE_API HeteroTerrain2D : public Noise2D {
  public:
    HeteroTerrain2D(Noise2D* noise, float scale, float offset = 0.0f, int octaves = 8, float lacunarity = 2.0f, float persistence = 0.5f, float dimension = 1.0f);

    float value(float x, float y) final;

  private:
    Noise2D* m_noise = nullptr;
    float m_scale = 1.0f;
    float m_offset = 0.0f;
    int m_octaves = 8;
    float m_lacunarity = 2.0f;
    float m_persistence = 0.5f;
    float m_dimension = 1.0f;
  };

  class GF_CORE_API HybridMultifractal2D : public Noise2D {
  public:
    HybridMultifractal2D(Noise2D* noise, float scale, float offset = 0.0f, int octaves = 8, float lacunarity = 2.0f, float persistence = 0.5f, float dimension = 1.0f);

    float value(float x, float y) final;

  private:
    Noise2D* m_noise = nullptr;
    float m_scale = 1.0f;
    float m_offset = 0.0f;
    int m_octaves = 8;
    float m_lacunarity = 2.0f;
    float m_persistence = 0.5f;
    float m_dimension = 1.0f;
  };

  class GF_CORE_API RidgedMultifractal2D : public Noise2D {
  public:
    RidgedMultifractal2D(Noise2D* noise, float scale, float offset = 1.0f, float gain = 1.0f, int octaves = 8, float lacunarity = 2.0f, float persistence = 0.5f, float dimension = 1.0f);

    float value(float x, float y) final;

  private:
    Noise2D* m_noise = nullptr;
    float m_scale = 1.0f;
    float m_offset = 1.0f;
    float m_gain = 1.0f;
    int m_octaves = 8;
    float m_lacunarity = 2.0f;
    float m_persistence = 0.5f;
    float m_dimension = 1.0f;
  };

  class GF_CORE_API Noise3DTo2DAdapter : public Noise2D {
  public:
    Noise3DTo2DAdapter(Noise3D* noise, Vec3F normal = { 0.0f, 0.0f, 1.0f }, Vec3F point = { 0.0f, 0.0f, 0.0f });

    float value(float x, float y) final;

  private:
    Noise3D* m_noise;
    Vec3F m_normal;
    Vec3F m_point;
  };

}

#endif // GF_NOISES_H
