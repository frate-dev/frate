#include <Frate/Utils/Crypto/md5.hpp>
#include <format>

namespace Frate::Utils {
    MD5Encoder& MD5Encoder::intake(std::string &input){
      MD5((unsigned char *)input.c_str(), input.size(), md5_hash_buffer.data());
      digest = "";
      for (int i = 0; i < 16; i++) {
        digest += std::format("{:02x}", md5_hash_buffer[i]);
      }
      return *this;
    }
}
