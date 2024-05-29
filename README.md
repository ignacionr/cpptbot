# cppTBot

cppTBot is a C++ library for creating and managing bot applications. This repository provides the necessary files and instructions to integrate cppTBot into your CMake-based project using CMake's FetchContent module.

## Getting Started

### Prerequisites

- CMake 3.14 or higher
- C++17 or higher compiler

### Using cppTBot with CMake FetchContent

To use cppTBot in your project, you can utilize CMake's FetchContent module. Below is a full sample `CMakeLists.txt` file to demonstrate how to integrate cppTBot into your project.

```cmake
cmake_minimum_required(VERSION 3.14)
project(MyBotApp VERSION 1.0 LANGUAGES CXX)

# Specify the C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Include FetchContent module
include(FetchContent)

# Fetch cppTBot from GitHub
FetchContent_Declare(
  cppTBot
  GIT_REPOSITORY https://github.com/ignacionr/cpptbot.git
  GIT_TAG main  # You can specify a particular commit, tag, or branch here
)

# Make cppTBot available
FetchContent_MakeAvailable(cppTBot)

# Add your executable
add_executable(MyBotApp main.cpp)

# Link cppTBot to your executable
target_link_libraries(MyBotApp PRIVATE cppTBot)
```

### Example `main.cpp`

Here is a simple example of `main.cpp` to get you started with cppTBot.

```cpp
#include <iostream>
#include <fstream>
#include <unordered_set>
#include <nlohmann/json.hpp>
#include "../include/cpptbot.hpp"

template <typename chat_t, typename msg_t>
class parrot {
public:
    parrot(chat_t &chat) : chat_{chat} {
        chat.receive(*this);
    }
    
    void operator()(msg_t msg) {
        std::cerr << "repeating: " << msg.dump() << std::endl;
        chat_.send(msg);
        if (--count_ < 0) {
            chat_.send({{"text", "This session is over."}});
            chat_.receive({});
        }
    }

private:
    chat_t &chat_;
    int count_{3};
};

uint64_t read_last_chat_id() {
    uint64_t result{};
    std::ifstream in("last_chat.txt");
    if (in.is_open()) {
        in >> result;
    }
    return result;
}

void write_last_chat_id(uint64_t value) {
    std::ofstream out("last_chat.txt");
    out << value;
}

int main() {
    const char* env_api_key = std::getenv("TBOT_SAMPLE_KEY");
    if (env_api_key == nullptr) {
        std::cerr << "Error: TBOT_SAMPLE_KEY environment variable not set." << std::endl;
        return 1;
    }

    using parrot_t = parrot<ignacionr::cpptbot::chat, nlohmann::json>;
    std::unordered_set<parrot_t*> parrots;
    ignacionr::cpptbot bot{env_api_key};

    if (auto last_chat = read_last_chat_id(); last_chat) {
        auto &chat = bot.chat_from_id(last_chat);
        chat.send({{"text", "and, we're back!"}});
        parrots.emplace(new parrot_t(chat));
    }

    bot.poll([&parrots](auto &chat, auto chat_id) {
        write_last_chat_id(chat_id);
        parrots.emplace(new parrot_t(chat));
    });
}
```

### Building Your Project

1. Create a `CMakeLists.txt` file in your project's root directory as shown above.
2. Create a `main.cpp` file or other source files as needed.
3. Create a build directory and run CMake and your build tool:

```bash
mkdir build
cd build
cmake ..
cmake --build .
```

4. Run your application:

```bash
./MyBotApp
```

## Repository Structure

- `include/cppTBot/` - Header files for cppTBot
- `src/` - Source files for cppTBot
- `sample/` - Sample CMakeLists and source files
- `tests/` - Unit tests for cppTBot

## Contributing

Contributions are welcome! Please fork the repository and submit a pull request with your changes.

## License

This project is licensed under the MIT License - see the [LICENSE](LICENSE) file for details.

## Contact

For any questions or support, please open an issue on the repository or contact the maintainer.
