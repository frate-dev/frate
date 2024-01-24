#pragma once
#include <Frate/FrateException.hpp>
#include <array>
#include <openssl/md5.h>
#include <string>

namespace Frate::Utils {
  class MD5FailedToEncode : public FrateException {
  public:
    MD5FailedToEncode(const std::string &message) : FrateException(message) {}
  };

  class MD5Encoder {
  private:
    std::array<unsigned char, MD5_DIGEST_LENGTH> md5_hash_buffer = {0};
    std::string digest;

  public:
    MD5Encoder() = default;
    MD5Encoder &intake(std::string &input);
    [[nodiscard]] std::string &getDigest(){ return digest;}
  };


} // namespace Frate::Utils
