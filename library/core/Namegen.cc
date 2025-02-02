// SPDX-License-Identifier: Zlib
// Copyright (c) 2023-2025 Julien Bernard

#include <gf2/core/Namegen.h>

#include <cassert>

#include <algorithm>
#include <iterator>
#include <random>
#include <set>

#include <gf2/core/Clock.h>
#include <gf2/core/StringUtils.h>

namespace gf {

  namespace {

    constexpr char32_t WordLimit = '#';

  }

  /*
   * NameGeneratorModel
   */

  NamegenModel::NamegenModel(const std::vector<std::u32string>& data, std::size_t order, double prior, std::u32string alphabet)
  : m_order(order)
  , m_prior(prior)
  , m_alphabet(std::move(alphabet))
  {
    assert(0.0 <= prior && prior <= 1.0);
    train(data);
    build_chains();
  }

  std::optional<char32_t> NamegenModel::generate(const std::u32string& context, Random& random) const
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

  void NamegenModel::retrain(const std::vector<std::u32string>& data)
  {
    train(data);
    build_chains();
  }

  void NamegenModel::train(const std::vector<std::u32string>& data)
  {
    for (const std::u32string& item : data) {
      const std::u32string sequence = std::u32string(m_order, WordLimit) + item + WordLimit;

      for (std::size_t i = 0; i < sequence.size() - m_order; ++i) {
        const std::u32string key = sequence.substr(i, m_order);
        assert(i + m_order < sequence.size());
        m_observations[key].push_back(sequence[i + m_order]);
      }
    }
  }

  void NamegenModel::build_chains()
  {
    m_chains.clear();

    for (auto& [ context, observation ] : m_observations) {
      std::vector<double>& values = m_chains[context];

      for (const char32_t prediction : m_alphabet) {
        values.push_back(m_prior + static_cast<double>(std::count(observation.begin(), observation.end(), prediction)));
      }
    }
  }

  /*
   * NamegenGenerator
   */

  NamegenGenerator::NamegenGenerator(const std::vector<std::u32string>& data, std::size_t order, double prior, bool backoff)
  : m_order(order)
  , m_prior(prior)
  , m_backoff(backoff)
  {
    std::set<char32_t> letters;

    for (const std::u32string& item : data) {
      for (const char32_t c : item) {
        letters.insert(c);
      }
    }

    std::u32string alphabet(letters.begin(), letters.end());
    alphabet.push_back(WordLimit);

    if (backoff) {
      for (std::size_t i = 0; i < m_order; ++i) {
        m_models.emplace_back(data, order - i, prior, alphabet);
      }
    } else {
      m_models.emplace_back(data, order, prior, alphabet);
    }
  }

  std::u32string NamegenGenerator::generate(Random& random) const
  {
    std::u32string word(m_order, WordLimit);

    auto maybe_letter = compute_letter(word, random);

    while (maybe_letter && maybe_letter.value() != WordLimit) {
      word.push_back(maybe_letter.value());
      maybe_letter = compute_letter(word, random);
    }

    return word;
  }

  std::optional<char32_t> NamegenGenerator::compute_letter(const std::u32string& word, Random& random) const
  {
    assert(word.size() >= m_order);

    std::u32string context = word.substr(word.size() - m_order);
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
    std::vector<std::u32string> to_utf32_strings(const std::vector<std::string>& data)
    {
      std::vector<std::u32string> utf32_data;
      std::transform(data.begin(), data.end(), std::back_inserter(utf32_data), to_utf32);
      return utf32_data;
    }

    bool satisfy_size_settings(const std::u32string& word, const NamegenSettings& settings)
    {
      return settings.min_length <= word.size() &&  word.size() <= settings.max_length;
    }

    bool satisfy_settings(const std::string& word, const NamegenSettings& settings)
    {
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
  : m_generator(to_utf32_strings(data), order, prior, backoff)
  {
  }

  std::optional<std::string> NamegenManager::generate_single(Random& random, const NamegenSettings& settings) const
  {
    std::u32string name = m_generator.generate(random);
    name.erase(std::remove(name.begin(), name.end(), WordLimit), name.end());

    if (!satisfy_size_settings(name, settings)) {
      return std::nullopt;
    }

    std::string utf8_name = to_utf8(name);

    if (satisfy_settings(utf8_name, settings)) {
      return utf8_name;
    }

    return std::nullopt;
  }

  std::vector<std::string> NamegenManager::generate_multiple(Random& random, std::size_t count, Time max_time_per_name, const NamegenSettings& settings) const
  {
    std::vector<std::string> names;
    const Time max_time = count * max_time_per_name;
    const Clock clock;

    while (names.size() < count && clock.elapsed_time() < max_time) {
      if (auto maybe_name = generate_single(random, settings); maybe_name) {
        names.push_back(std::move(maybe_name).value());
      }
    }

    return names;
  }

}

