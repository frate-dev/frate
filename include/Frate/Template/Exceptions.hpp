#pragma once
#include "Frate/FrateException.hpp"
#include <string>

namespace Frate::Project {

  class TemplateException : public FrateException {
  public:
    TemplateException(const std::string &message) : FrateException(message) {}
  };

  class TemplateNotInstalled : public TemplateException {
  public:
    TemplateNotInstalled(const std::string &message)
        : TemplateException(message) {}
  };

  class TemplateNotFoundInIndex : public TemplateException {
  public:
    TemplateNotFoundInIndex(const std::string &message)
        : TemplateException(message) {}
  };

  class TemplateFailedToDownload : public TemplateException {
  public:
    TemplateFailedToDownload(const std::string &message)
        : TemplateException(message) {}
  };

  class TemplateIndexNotLoaded : public TemplateException {
  public:
    TemplateIndexNotLoaded(const std::string &message)
        : TemplateException(message) {}
  };

  class TemplateIndexFailedToLoad : public TemplateException {
  public:
    TemplateIndexFailedToLoad(const std::string &message)
        : TemplateException(message) {}
  };

  class TemplateConfigNotFound : public TemplateException {
  public:
    TemplateConfigNotFound(const std::string &message)
        : TemplateException(message) {}
  };

  class TemplateFileMapEmpty : public TemplateException {
  public:
    TemplateFileMapEmpty(const std::string &message)
        : TemplateException(message) {}
  };

  class TemplatePromptException : public TemplateException {
  public:
    TemplatePromptException(const std::string &message)
        : TemplateException(message) {}
  };

  class TemplateRenderException : public TemplateException {
  public:
    TemplateRenderException(const std::string &message)
        : TemplateException(message) {}
  };

} // namespace Frate::Project
