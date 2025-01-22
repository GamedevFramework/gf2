// SPDX-License-Identifier: Zlib
// Copyright (c) 2023-2025 Julien Bernard
#ifndef GF_NAMEGEN_H
#define GF_NAMEGEN_H

#include <limits>
#include <map>
#include <optional>
#include <string>
#include <vector>

#include "CoreApi.h"
#include "Random.h"
#include "Time.h"

namespace gf {

  class GF_CORE_API NamegenModel {
  public:
    NamegenModel(const std::vector<std::string>& data, std::size_t order, double prior, std::string alphabet);

    std::optional<char> generate(const std::string& context, Random& random) const;
    void retrain(const std::vector<std::string>& data);

  private:
    void train(const std::vector<std::string>& data);
    void build_chains();

    std::size_t m_order = 1;
    double m_prior = 0.0;
    std::string m_alphabet;
    std::map<std::string, std::string> m_observations;
    std::map<std::string, std::vector<double>> m_chains;
  };

  class GF_CORE_API NamegenGenerator {
  public:
    NamegenGenerator(const std::vector<std::string>& data, std::size_t order, double prior, bool backoff);

    std::string generate(Random& random) const;

  private:
    std::optional<char> compute_letter(const std::string& word, Random& random) const;

    std::size_t m_order = 1;
    double m_prior = 0.0;
    bool m_backoff = true;
    std::vector<NamegenModel> m_models;

  };

  struct NamegenSettings {
    std::size_t min_length = 0;
    std::size_t max_length = std::numeric_limits<std::size_t>::max();
    std::string starts_with;
    std::string ends_with;
    std::string includes;
    std::string excludes;
  };

  class GF_CORE_API NamegenManager {
  public:
    NamegenManager(const std::vector<std::string>& data, std::size_t order, double prior, bool backoff);

    std::optional<std::string> generate_single(Random& random, const NamegenSettings& settings = {});
    std::vector<std::string> generate_multiple(Random& random, std::size_t count, Time max_time_per_name, const NamegenSettings& settings = {});

  private:
    NamegenGenerator m_generator;
  };

}

#endif // GF_NAMEGEN_H
