#include "Frate/Utils/General.hpp"
#include <Frate/Generators.hpp>
#include <Frate/Constants.hpp>
#include <fstream>
#include <memory>
namespace Frate::Generators::Readme {
    bool create(std::shared_ptr<Command::Interface> inter){
      std::string readme = R"(
# {project_name} with {name}
- {description}
- {author}
- {date}

## Getting started

### Adding dependencies
To add dependencies to your project, you can use the following command:
```bash
frate add p <package_name>
```
This will add the package to the project and will automatically add it to the CMakeLists.txt file.

### Building the project
To build the project, you can use the following command:
```bash
frate build
```
This will build the project and will create the executable in the build directory.

### Running the project
To run the project, you can use the following command:
```bash
frate run
```
This will run the project and will create the executable in the build directory.

Find more information at the [Frate Github]({help_url})

)";

      Utils::replaceKey(readme, "{project_name}", inter->pro->name + Constants::VERSION);
      Utils::replaceKey(readme, "{name}", Constants::NAME);
      Utils::replaceKey(readme, "{description}", inter->pro->description);
      Utils::replaceKey(readme, "{author}", inter->pro->authors.size() > 0 ? inter->pro->authors[0] : "");

      std::time_t t = std::time(nullptr);
      std::tm tm = *std::localtime(&t);
      std::stringstream ss;
      ss << std::put_time(&tm, "%d/%m/%Y");
      Utils::replaceKey(readme, "{date}", ss.str());
      Utils::replaceKey(readme, "{help_url}", Constants::PROJECT_URL);
      std::ofstream readme_file(inter->pro->path / "README.md");

      try{
        readme_file << readme;
      }catch(...){
        Frate::error << "Failed to create README.md file" << std::endl;
        return false;
      }
      return true;
    }
}

