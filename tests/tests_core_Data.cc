#include <gf2/core/ActionSettings.h>
#include <gf2/core/AnimationData.h>
#include <gf2/core/ControlSettings.h>
#include <gf2/core/Property.h>
#include <gf2/core/SerializationOps.h>
#include <gf2/core/SerializationContainer.h>
#include <gf2/core/SpriteData.h>
#include <gf2/core/Stream.h>
#include <gf2/core/TextData.h>

#include "gtest/gtest.h"

namespace {

  class NullInputStream : public gf::InputStream {
  public:
    std::size_t read(gf::Span<uint8_t> buffer) override
    {
      if (buffer.size() >= 2) {
        buffer[0] = 'g';
        buffer[1] = 'f';
      }

      return buffer.size();
    }

    void seek([[maybe_unused]] std::ptrdiff_t position) override
    {
    }

    void skip([[maybe_unused]] std::ptrdiff_t position) override
    {
    }

    bool finished() override
    {
      return false;
    }
  };

  template<typename Data>
  void deserialize(Data& data)
  {
    NullInputStream stream;
    gf::Deserializer archive(&stream);

    archive | data;
  }

  class NullOutputStream : public gf::OutputStream {
  public:
    std::size_t write(gf::Span<const uint8_t> buffer) override
    {
      m_written += buffer.size();
      return buffer.size();
    }

    std::size_t written_bytes() const override
    {
      return m_written;
    }

  private:
    std::size_t m_written = 0;
  };

  template<typename Data>
  void serialize(const Data& data)
  {
    NullOutputStream stream;
    gf::Serializer archive(&stream);

    archive | data;
  }

}

TEST(DataTest, AnimatedSpriteData) {
  gf::AnimationData data;

  serialize(data);
  deserialize(data);

  gf::SpriteResource resource;
  serialize(resource);
  deserialize(resource);
}

TEST(DataTest, SpriteData) {
  gf::SpriteData data;

  serialize(data);
  deserialize(data);

  gf::SpriteResource resource;
  serialize(resource);
  deserialize(resource);
}

TEST(DataTest, TextData) {
  gf::TextData data;

  serialize(data);
  deserialize(data);

  gf::TextResource resource;
  serialize(resource);
  deserialize(resource);
}

TEST(DataTest, ControlData) {
  gf::ControlSettings data = {};

  serialize(data);
  deserialize(data);
}

TEST(DataTest, ActionData) {
  gf::ActionGroupSettings data = {};

  serialize(data);
  deserialize(data);
}
