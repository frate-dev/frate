#pragma once
#include <string>


namespace Utils::Validation {
  bool CppLanguageVersion(std::string version);
  bool CLanguageVersion(std::string version);
  bool Language(std::string lang);
  bool CCompiler(std::string compiler);
  bool CppCompiler(std::string compiler);
  bool ProjectName(std::string name);
  bool ProjectType(std::string type);
  bool SourceDir(std::string dir);
  bool BuildDir(std::string dir);
  bool IncludeDir(std::string dir);
  bool ProjectVersion(std::string version);
  bool CmakeVersion(std::string version);
}
