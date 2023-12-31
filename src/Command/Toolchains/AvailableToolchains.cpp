#include <vector>
#include <Frate/Command/AvailableTriples.hpp>
namespace Frate::Command::Toolchain {
  extern const std::vector<CompileTarget> CompileTargets = {
    {
      .os = "Darwin",
      .triple = "aarch64-apple-darwin"
    },
    {
      .os = "iOS",
      .triple = "aarch64-apple-ios"
    },
    {
      .os = "iOS", 
      .triple = "aarch64-apple-ios-macabi"
    },
    {
      .os = "iOS",
      .triple = "aarch64-apple-ios-sim"
    },
    {
      .os = "tvOS",
      .triple = "aarch64-apple-tvos"
    },
    {
      .os = "watchOS",
      .triple = "aarch64-apple-watchos-sim"
    },
    {
      .os = "Fuchsia",
      .triple = "aarch64-fuchsia"
    },
    {
      .os = "KMC Solid ASP3",
      .triple = "aarch64-kmc-solid_asp3"
    },
    {
      .os = "Android",
      .triple = "aarch64-linux-android"
    },
    {
      .os = "Nintendo Switch",
      .triple = "aarch64-nintendo-switch-freestanding"
    },
    {
      .os = "GnullVM",
      .triple = "aarch64-pc-windows-gnullvm"
    },
    {
      .os = "Windows",
      .triple = "aarch64-pc-windows-msvc"
    },
    {
      .os = "FreeBSD",
      .triple = "aarch64-unknown-freebsd"
    },
    {
      .os = "Fuchsia",
      .triple = "aarch64-unknown-fuchsia"
    },
    {
      .os = "Hermit",
      .triple = "aarch64-unknown-hermit"
    },
    {
      .os = "Linux",
      .triple = "aarch64-unknown-linux-gnu"
    },
    {
      .os = "Linux",
      .triple = "aarch64-unknown-linux-gnu_ilp32"
    },
    {
      .os = "Linux",
      .triple = "aarch64-unknown-linux-musl"
    },
    {
      .os = "HarmonyOS",
      .triple = "aarch64-unknown-linux-ohos"
    },
    {
      .os = "NetBSD",
      .triple = "aarch64-unknown-netbsd"
    },
    {
      .os = "None",
      .triple = "aarch64-unknown-none"
    },
    {
      .os = "None",
      .triple = "aarch64-unknown-none-softfloat"
    },
    {
      .os = "QNX",
      .triple = "aarch64-unknown-nto-qnx710"
    },
    {
      .os = "OpenBSD",
      .triple = "aarch64-unknown-openbsd"
    },
    {
      .os = "Redox",
      .triple = "aarch64-unknown-redox"
    },
    {
      .os = "TEEOS",
      .triple = "aarch64-unknown-teeos"
    },
    {
      .os = "UEFI",
      .triple = "aarch64-unknown-uefi"
    },
    {
      .os = "Windows",
      .triple = "aarch64-uwp-windows-msvc"
    },
    {
      .os = "VxWorks",
      .triple = "aarch64-wrs-vxworks"
    },
    {
      .os = "Linux",
      .triple = "aarch64_be-unknown-linux-gnu"
    },
    {
      .os = "Linux",
      .triple = "aarch64_be-unknown-linux-gnu_ilp32"
    },
    {
      .os = "NetBSD",
      .triple = "aarch64_be-unknown-netbsd"
    },
    {
      .os = "Android",
      .triple = "arm-linux-androideabi"
    },
    {
      .os = "Linux",
      .triple = "arm-unknown-linux-gnueabi"
    },
    {
      .os = "Linux",
      .triple = "arm-unknown-linux-gnueabihf"
    },
    {
      .os = "Linux",
      .triple = "arm-unknown-linux-musleabi"
    },
    {
      .os = "Linux",
      .triple = "arm-unknown-linux-musleabihf"
    },
    {
      .os = "watchOS",
      .triple = "arm64_32-apple-watchos"
    },
    {
      .os = "Linux",
      .triple = "armeb-unknown-linux-gnueabi"
    },
    {
      .os = "None",
      .triple = "armebv7r-none-eabi"
    },
    {
      .os = "None",
      .triple = "armebv7r-none-eabihf"
    },
    {
      .os = "None",
      .triple = "armv4t-none-eabi"
    },
    {
      .os = "Linux",
      .triple = "armv4t-unknown-linux-gnueabi"
    },
    {
      .os = "None",
      .triple = "armv5te-none-eabi"
    },
    {
      .os = "Linux",
      .triple = "armv5te-unknown-linux-gnueabi"
    },
    {
      .os = "Linux",
      .triple = "armv5te-unknown-linux-musleabi"
    },
    {
      .os = "Linux",
      .triple = "armv5te-unknown-linux-uclibceabi"
    },
    {
      .os = "FreeBSD",
      .triple = "armv6-unknown-freebsd"
    },
    {
      .os = "NetBSD",
      .triple = "armv6-unknown-netbsd-eabihf"
    },
    {
      .os = "Nintendo 3DS",
      .triple = "armv6k-nintendo-3ds"
    },
    {
      .os = "Android",
      .triple = "armv7-linux-androideabi"
    },
    {
      .os = "Sony Vita",
      .triple = "armv7-sony-vita-newlibeabihf"
    },
    {
      .os = "FreeBSD",
      .triple = "armv7-unknown-freebsd"
    },
    {
      .os = "Linux",
      .triple = "armv7-unknown-linux-gnueabi"
    },
    {
      .os = "Linux",
      .triple = "armv7-unknown-linux-gnueabihf"
    },
    {
      .os = "Linux",
      .triple = "armv7-unknown-linux-musleabi"
    },
    {
      .os = "Linux",
      .triple = "armv7-unknown-linux"
    },
    {
      .os = "HarmonyOS",
      .triple = "armv7-unknown-linux-ohos"
    },
    {
      .os = "Linux",
      .triple = "armv7-unknown-linux-uclibceabi"
    },
    {
      .os = "Linux",
      .triple = "armv7-unknown-linux-uclibceabihf"
    },
    {
      .os = "NetBSD",
      .triple = "armv7-unknown-netbsd-eabihf"
    },
    {
      .os = "VxWorks",
      .triple = "armv7-wrs-vxworks-eabihf"
    },
    {
      .os = "KMC Solid ASP3",
      .triple = "armv7a-kmc-solid_asp3-eabi"
    },
    {
      .os = "KMC Solid ASP3",
      .triple = "armv7a-kmc-solid_asp3-eabihf"
    },
    {
      .os = "None",
      .triple = "armv7a-none-eabi"
    },
    {
      .os = "None",
      .triple = "armv7a-none-eabihf"
    },
    {
      .os = "watchOS",
      .triple = "armv7k-apple-watchos"
    },
    {
      .os = "None",
      .triple = "armv7r-none-eabi"
    },
    {
      .os = "None",
      .triple = "armv7r-none-eabihf"
    },
    {
      .os = "iOS",
      .triple = "armv7s-apple-ios"
    },
    {
      .os = "Emscripten",
      .triple = "asmjs-unknown-emscripten"
    },
    {
      .os = "AVR",
      .triple = "avr-unknown-gnu-atmega328"
    },
    {
      .os = "None",
      .triple = "bpfeb-unknown-none"
    },
    {
      .os = "None",
      .triple = "bpfel-unknown-none"
    },
    {
      .os = "Linux",
      .triple = "csky-unknown-linux-gnuabiv2"
    },
    {
      .os = "Darwin",
      .triple = "x86_64-apple-darwin"
    },
    {
      .os = "iOS",
      .triple = "x86_64-apple-ios"
    },
    {
      .os = "iOS",
      .triple = "x86_64-apple-ios-macabi"
    },
    {
      .os = "tvOS",
      .triple = "x86_64-apple-tvos"
    },
    {
      .os = "watchOS",
      .triple = "x86_64-apple-watchos-sim"
    },
    {
      .os = "Fortanix SGX",
      .triple = "x86_64-fortanix-unknown-sgx"
    },
    {
      .os = "Fuchsia",
      .triple = "x86_64-fuchsia"
    },
    {
      .os = "Android",
      .triple = "x86_64-linux-android"
    },
    {
      .os = "QNX",
      .triple = "x86_64-pc-nto-qnx710"
    },
    {
      .os = "Solaris",
      .triple = "x86_64-pc-solaris"
    },
    {
      .os = "Windows",
      .triple = "x86_64-pc-windows-gnu"
    },
    {
      .os = "GnullVM",
      .triple = "x86_64-pc-windows-gnullvm"
    },
    {
      .os = "Windows",
      .triple = "x86_64-pc-windows-msvc"
    },
    {
      .os = "Solaris",
      .triple = "x86_64-sun-solaris"
    },
    {
      .os = "Unikraft",
      .triple = "x86_64-unikraft-linux-musl"
    },
    {
      .os = "DragonFly",
      .triple = "x86_64-unknown-dragonfly"
    },
    {
      .os = "FreeBSD",
      .triple = "x86_64-unknown-freebsd"
    },
    {
      .os = "Fuchsia",
      .triple = "x86_64-unknown-fuchsia"
    },
    {
      .os = "Haiku",
      .triple = "x86_64-unknown-haiku"
    },
    {
      .os = "Hermit",
      .triple = "x86_64-unknown-hermit"
    },
    {
      .os = "Illumos",
      .triple = "x86_64-unknown-illumos"
    },
    {
      .os = "L4Re",
      .triple = "x86_64-unknown-l4re-uclibc"
    },
    {
      .os = "Linux",
      .triple = "x86_64-unknown-linux-gnu"
    },
    {
      .os = "Linux",
      .triple = "x86_64-unknown-linux-gnux32"
    },
    {
      .os = "Linux",
      .triple = "x86_64-unknown-linux-musl"
    },
    {
      .os = "HarmonyOS",
      .triple = "x86_64-unknown-linux-ohos"
    },
    {
      .os = "NetBSD",
      .triple = "x86_64-unknown-netbsd"
    },
    {
      .os = "None",
      .triple = "x86_64-unknown-none"
    },
    {
      .os = "OpenBSD",
      .triple = "x86_64-unknown-openbsd"
    },
    {
      .os = "Redox",
      .triple = "x86_64-unknown-redox"
    },
    {
      .os = "UEFI",
      .triple = "x86_64-unknown-uefi"
    },
    {
      .os = "Windows",
      .triple = "x86_64-uwp-windows-gnu"
    },
    {
      .os = "Windows",
      .triple = "x86_64-uwp-windows-msvc"
    },
    {
      .os = "VxWorks",
      .triple = "x86_64-wrs-vxworks"
    },
    {
      .os = "Darwin",
      .triple = "x86_64-apple-darwin"
    }
  };
}
