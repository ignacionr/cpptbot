#pragma once
#include <string>
#include <vector>
#include <cpr/cpr.h>
#include <nlohmann/json.hpp>

using json = nlohmann::json;

namespace ignacionr
{
    class cpptbot
    {
    private:
        std::string api_key;   // Your Telegram Bot API key
        std::string base_url;  // Base URL for Telegram API

    public:
        cpptbot(const std::string &api_key)
            : api_key(api_key), base_url("https://api.telegram.org/bot" + api_key + "/")
        {}

        // Function to send a message to a Telegram chat
        json sendMessage(long long chat_id, const std::string &message_text)
        {
            // Prepare the API request payload
            json payload = {
                {"chat_id", chat_id},
                {"text", message_text}};

            // Send the API request
            auto r = cpr::Post(
                cpr::Url{base_url + "sendMessage"},
                cpr::Header{{"Content-Type", "application/json"}},
                cpr::Body{payload.dump()});

            // Parse the API response
            auto response = json::parse(r.text);

            return response;
        }
    };
}
