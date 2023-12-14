#include <Frate/Command.hpp>
#include <nlohmann/json.hpp>

namespace Frate::Reflection {
  using nlohmann::json;
  using Command::Project;

  static void to_json(json &j, const Project &p) {
    j = json{
      {"name", p.name},
      {"description", p.description},
      {"type", p.type},
      {"version", p.version},
      {"authors", p.authors},
      {"license", p.license},
      {"dependencies", p.dependencies},
      {"keywords", p.keywords},
      {"git", p.git},
      {"homepage", p.homepage},
      {"bugs", p.bugs},
      {"compiler", p.compiler},
      {"lang", p.lang},
      {"lang_version", p.lang_version},
      {"build_dir", p.build_dir},
      {"include_dir", p.include_dir},
      {"src_dir", p.src_dir},
      {"toolchains", p.toolchains},
      {"flags", p.flags},
      {"libs", p.libs},
      {"default_mode", p.default_mode},
      {"modes", p.modes}
    };
  }
}
