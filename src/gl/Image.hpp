#pragma once
#include <stb/stb_image.h>
#include <cassert>
#include <vector>

namespace l3d {
namespace gl {
class Image {
 public:
  inline Image();
  inline Image(const char* filename, int format);
  inline ~Image();
  inline bool Load(const char* filename, int format);
  inline int Width() const;
  inline int Height() const;
  inline int Channels() const;
  inline std::vector<unsigned char> BytesCopy() const;
  inline unsigned char* Bytes();

 private:
  unsigned char* bytes;
  int width;
  int height;
  int channels;
};

Image::Image() : bytes(nullptr), width(-1), height(-1), channels(-1) {}

Image::Image(const char* filename, int format) : Image() {
  Load(filename, format);
}

Image::~Image() {
  if (bytes != nullptr) stbi_image_free(bytes);
}

bool Image::Load(const char* filename, int format) {
  if (bytes != nullptr) {
    // unloads the current image and resets all attributes
    stbi_image_free(bytes);
    bytes = nullptr;
    width = -1;
    height = -1;
    channels = -1;
  }
  // loads the new image
  bytes = stbi_load(filename, &width, &height, &channels, format);
  return bytes != nullptr;
}

int Image::Width() const { return width; }

int Image::Height() const { return height; }

int Image::Channels() const { return channels; }

std::vector<unsigned char> Image::BytesCopy() const {
  assert(bytes != nullptr && "No image loaded!");
  return std::vector<unsigned char>(bytes, bytes + width * height * channels);
}

unsigned char* Image::Bytes() { return bytes; }

}  // namespace gl
}  // namespace l3d
