#include <iostream>
#include <set>
#include <nlohmann/json.hpp>
#include "../cpptbot.hpp"

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

int main() {
        // Read API key from environment variable
    const char* env_api_key = std::getenv("TBOT_SAMPLE_KEY");
    if (env_api_key == nullptr)
    {
        std::cerr << "Error: TBOT_SAMPLE_KEY environment variable not set." << std::endl;
        return 1;
    }
    ignacionr::cpptbot bot{env_api_key};
    bot.poll([](auto &chat){
        new parrot<ignacionr::chat<nlohmann::json>,nlohmann::json>(chat);
    });
}
