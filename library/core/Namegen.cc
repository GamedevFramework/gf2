// SPDX-License-Identifier: Zlib
// Copyright (c) 2023-2025 Julien Bernard

#include <gf2/core/Namegen.h>

#include <cassert>

#include <algorithm>
#include <random>
#include <set>

#include <gf2/core/Clock.h>

namespace gf {

  namespace {

    constexpr char WordLimit = '#';

  }

  /*
   * NameGeneratorModel
   */

  NamegenModel::NamegenModel(const std::vector<std::string>& data, std::size_t order, double prior, std::string alphabet)
  : m_order(order)
  , m_prior(prior)
  , m_alphabet(std::move(alphabet))
  {
    assert(0.0 <= prior && prior <= 1.0);
    train(data);
    build_chains();
  }

  std::optional<char> NamegenModel::generate(const std::string& context, Random& random) const
  {
    assert(context.size() == m_order);
    auto iterator = m_chains.find(context);

    if (iterator == m_chains.end()) {
      return std::nullopt;
    }

    const std::vector<double>& weights = iterator->second;

    std::discrete_distribution<std::size_t> distribution(weights.begin(), weights.end());
    auto index = distribution(random.engine());
    assert(index < m_alphabet.size());
    return m_alphabet[index];
  }

  void NamegenModel::retrain(const std::vector<std::string>& data)
  {
    train(data);
    build_chains();
  }

  void NamegenModel::train(const std::vector<std::string>& data)
  {
    for (const std::string& item : data) {
      std::string d = std::string(m_order, WordLimit) + item + WordLimit;

      for (std::size_t i = 0; i < d.size() - m_order; ++i) {
        const std::string key = d.substr(i, m_order);
        assert(i + m_order < d.size());
        m_observations[key].push_back(d[i + m_order]);
      }
    }
  }

  void NamegenModel::build_chains()
  {
    m_chains.clear();

    for (auto& [ context, observation ] : m_observations) {
      std::vector<double>& values = m_chains[context];

      for (char prediction : m_alphabet) {
        values.push_back(m_prior + static_cast<double>(std::count(observation.begin(), observation.end(), prediction)));
      }
    }
  }

  /*
   * NamegenGenerator
   */

  NamegenGenerator::NamegenGenerator(const std::vector<std::string>& data, std::size_t order, double prior, bool backoff)
  : m_order(order)
  , m_prior(prior)
  , m_backoff(backoff)
  {
    std::set<char> letters;

    for (const std::string& item : data) {
      for (const char c : item) {
        letters.insert(c);
      }
    }

    std::string alphabet(letters.begin(), letters.end());
    alphabet.push_back(WordLimit);

    if (backoff) {
      for (std::size_t i = 0; i < m_order; ++i) {
        m_models.emplace_back(data, order - i, prior, alphabet);
      }
    } else {
      m_models.emplace_back(data, order, prior, alphabet);
    }
  }

  std::string NamegenGenerator::generate(Random& random) const
  {
    std::string word(m_order, WordLimit);

    auto maybe_letter = compute_letter(word, random);

    while (maybe_letter && maybe_letter.value() != WordLimit) {
      word.push_back(maybe_letter.value());
      maybe_letter = compute_letter(word, random);
    }

    return word;
  }

  std::optional<char> NamegenGenerator::compute_letter(const std::string& word, Random& random) const
  {
    assert(word.size() >= m_order);

    std::string context = word.substr(word.size() - m_order);
    assert(context.size() == m_order);

    for (const NamegenModel& model : m_models) {
      auto maybe_letter = model.generate(context, random);

      if (maybe_letter && maybe_letter.value() != WordLimit) {
        return maybe_letter;
      }

      context = context.substr(1);
    }

    return std::nullopt;
  }

  /*
   * NamegenManager
   */

  namespace {

    bool satisfy_settings(const std::string& word, const NamegenSettings& settings)
    {
      if (word.size() < settings.min_length || word.size() > settings.max_length) {
        return false;
      }

      if (settings.starts_with.size() > word.size() || word.substr(0, settings.starts_with.size()) != settings.starts_with) {
        return false;
      }

      if (settings.ends_with.size() > word.size() || word.substr(word.size() - settings.ends_with.size()) != settings.ends_with) {
        return false;
      }

      if (!settings.includes.empty() && word.find(settings.includes) == std::string::npos) {
        return false;
      }

      if (!settings.excludes.empty() && word.find(settings.excludes) != std::string::npos) {
        return false;
      }

      return true;
    }

  }

  NamegenManager::NamegenManager(const std::vector<std::string>& data, std::size_t order, double prior, bool backoff)
  : m_generator(data, order, prior, backoff)
  {
  }

  std::optional<std::string> NamegenManager::generate_single(Random& random, const NamegenSettings& settings) const
  {
    std::string name = m_generator.generate(random);
    name.erase(std::remove(name.begin(), name.end(), WordLimit), name.end());

    if (satisfy_settings(name, settings)) {
      return name;
    }

    return std::nullopt;
  }

  std::vector<std::string> NamegenManager::generate_multiple(Random& random, std::size_t count, Time max_time_per_name, const NamegenSettings& settings) const
  {
    std::vector<std::string> names;
    Clock clock;

    while (names.size() < count && clock.restart() < max_time_per_name) {
      if (auto maybe_name = generate_single(random, settings); maybe_name) {
        names.push_back(std::move(maybe_name).value());
      }
    }

    return names;
  }

}

