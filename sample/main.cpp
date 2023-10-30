#include <fstream>
#include <iostream>
#include <memory>
#include <unordered_set>
#include <nlohmann/json.hpp>
#include "../include/cpptbot.hpp"

template<typename chat_t, typename msg_t>
class parrot {
public:
    parrot(chat_t &chat): chat_{chat} {
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
        // Read API key from environment variable
    const char* env_api_key = std::getenv("TBOT_SAMPLE_KEY");
    if (env_api_key == nullptr)
    {
        std::cerr << "Error: TBOT_SAMPLE_KEY environment variable not set." << std::endl;
        return 1;
    }
    using parrot_t = parrot<ignacionr::chat<nlohmann::json>,nlohmann::json>;
    std::unordered_set<std::unique_ptr<parrot_t>> parrots;
    ignacionr::cpptbot bot{env_api_key};

    // reopen last chat
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
