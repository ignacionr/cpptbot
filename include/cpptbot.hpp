#pragma once
#include <functional>
#include <exception>
#include <queue>
#include <unordered_map>
#include <string>
#include <vector>
#include <cpr/cpr.h>
#include <nlohmann/json.hpp>

using json = nlohmann::json;

namespace ignacionr
{
    template <typename message_t>
    class chat
    {
    public:
        using sendfn_t = std::function<void(message_t)>;
        using receivefn_t = sendfn_t;

        chat(sendfn_t send) : sendfn_{send} {}
        void send(message_t msg)
        {
            sendfn_(msg);
        }
        void receive(receivefn_t recv)
        {
            while (!queued_.empty())
            {
                recv(queued_.front());
                queued_.pop();
            }
            receivefn_ = recv;
        }
        void on_message(message_t msg)
        {
            if (receivefn_)
            {
                receivefn_(msg);
            }
            else
            {
                queued_.push(msg);
            }
        }
        json info;

    private:
        sendfn_t sendfn_;
        receivefn_t receivefn_;
        std::queue<message_t> queued_;
    };

    class cpptbot
    {
    public:
        cpptbot(const std::string &api_key)
            : api_key(api_key), base_url("https://api.telegram.org/bot" + api_key + "/")
        {
        }

        // Function to send a message to a Telegram chat
        json sendMessage(uint64_t chat_id, json &payload)
        {
            // Prepare the API request payload
            payload["chat_id"] = chat_id;

            // Send the API request
            auto r = cpr::Post(
                cpr::Url{base_url + "sendMessage"},
                cpr::Header{{"Content-Type", "application/json"}},
                cpr::Body{payload.dump()});

            // Parse the API response
            auto response = json::parse(r.text);
            return response;
        }

        chat<json> &chat_from_id(uint64_t chat_id)
        {
            chats_.emplace(chat_id,
                           chat<json>{
                               [this, chat_id](json msg)
                               {
                                   sendMessage(chat_id, msg);
                               }});
            return chats_.at(chat_id);
        }

        template <typename callback_t>
        void poll(callback_t on_new_chat)
        {
            uint64_t last_update{};
            for (;;) // Infinite loop for continuous polling
            {
                // Prepare the API request payload
                json payload = {
                    {"offset", last_update + 1},
                    {"timeout", 60} // Long polling timeout (in seconds)
                };

                // Send the API request
                auto r = cpr::Post(
                    cpr::Url{base_url + "getUpdates"},
                    cpr::Header{{"Content-Type", "application/json"}},
                    cpr::Body{payload.dump()});

                // Parse the API response
                auto response = json::parse(r.text);

                // Check if the request was successful
                if (response["ok"].get<bool>())
                {
                    // Process each received update
                    for (const auto &update : response["result"])
                    {
                        last_update = update["update_id"].get<uint64_t>();
                        auto msg = update["message"];
                        auto chat_id = msg["chat"]["id"].get<uint64_t>();
                        if (!chats_.contains(chat_id))
                        {
                            auto &ch = chat_from_id(chat_id);
                            ch.info = msg["chat"];
                            on_new_chat(ch, chat_id);
                        }
                        chats_.at(chat_id).on_message(msg);
                    }
                }
                else
                {
                    throw std::runtime_error("Failed to get updates: " + response["description"].get<std::string>());
                }
            }
        }

        std::string get_file_path(std::string_view file_id) {
            json payload = {{
                "file_id", file_id
            }};
            auto r = cpr::Post(
                cpr::Url{base_url + "getFile"},
                cpr::Header{{"Content-Type", "application/json"}},
                cpr::Body{payload.dump()});
            auto response = json::parse(r.text);
            if (response["ok"].get<bool>())
            {
                return response["result"]["file_path"].get<std::string>();
            }
            else
            {
                throw std::runtime_error("Failed to get file: " + response["description"].get<std::string>());
            }
        }

        std::string get_file_contents(std::string_view file_id) {
            auto file_path = get_file_path(file_id);
            auto r = cpr::Get(
                cpr::Url{"https://api.telegram.org/file/bot" + api_key + "/" + file_path});
            return r.text;
        }
    private:
        std::string api_key;  // Your Telegram Bot API key
        std::string base_url; // Base URL for Telegram API
        std::unordered_map<uint64_t, chat<json>> chats_;
    };
}
