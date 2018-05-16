#ifndef VK_CHAT_H_INCLUDED
#define VK_CHAT_H_INCLUDED

#include <vector> // std::vector

#include "fizvlad/vk_api/vk_longpoll.h"


namespace fizvlad {namespace vk_api {
namespace chat {

    /// Pseudonym for unsigned long. Id of message.
    typedef unsigned long MessageId;

    /// Pseudonym for unsigned long. Id of user or group.
    typedef unsigned long UnitId;


    struct Message {

        time_t      ts;
        MessageId   id;
        UnitId      sender;
        UnitId      receiver;
        std::string text;


        Message() : ts(0), id(0), sender(0), receiver(0), text("") {}

        Message(time_t t, MessageId i, UnitId s, UnitId r, std::string content) :
            ts(t), id(i), sender(s), receiver(r), text(content) {}

        /// Parsing message info from response.
        Message(nlohmann::json response) :
            ts(response["date"]), id(response["id"]), sender((int)response["out"] == 1 ? 0 : (UnitId)response["user_id"]),
            receiver((int)response["out"] == 0 ? 0 : (UnitId)response["user_id"]), text(utility::xml_decode(response["body"].get<std::string>())) {}

    };

    /// Pseudonym for array of Message.
    typedef std::vector<Message> Messages;


    /// Simple group chat bot. Require froup with enabled chat and group token.
    class ChatBot {
    public:

        /// Saving accessToken and setting up longpoll session.
        ChatBot(Token token, UnitId selfId);


        ChatBot() = delete;
        ChatBot&operator=(const ChatBot&) = delete;
        ChatBot(const ChatBot&) = delete;
        ChatBot&operator=(ChatBot&&) = delete;
        ChatBot(ChatBot&&) = delete;

        ~ChatBot();


        /// Handle last unread message in every dialogue and starts longpoll.
        /// Stop listening when handler return FALSE.
        template <typename MessageHandlerFunc>
        void start (MessageHandlerFunc handler) {
            // TODO Correct work with function from template

            // Handling messages received while offline
            Messages unread = getUnreadMessages_();
            for (Message message : unread) {
                if (!handler(message, true)) {
                    // Stop listening if handler return FALSE
                    return;
                }
            }

            // Listening for new messages
            longpoll_.listen([&] (longpoll::Session::Updates upd) {
                for (auto i : upd) {
                    if ((unsigned)i[0] == (unsigned)longpoll::EVENTS::MESSAGE::NEW &&
                        !((unsigned)i[2] & (unsigned)longpoll::MESSAGE_FLAGS::OUTBOX)) {
                        // Received new message
                        Message message(i[4], i[1], i[3], 0, utility::xml_decode(i[5]));
                        bool isOldMessage = false;
                        if (!handler(message, isOldMessage)) {
                            return false;
                        }
                    }
                }
                return true;
             });
        }


        /// Mark message as read.
        void markAsRead(Message message);


        /// Sets online status on web-site.
        void setOnlineStatus(bool isOnline);


        /// Send message to user.
        void sendMessage(UnitId receiver, std::string text, std::string attachment = "");


        /// Sends file to user.
        void uploadAndSend(UnitId receiver, std::string filePath, std::string fileTitle, std::string text = "");


    private:

        longpoll::Session longpoll_;
        Token token_;
        UnitId selfId_;

        Messages getUnreadMessages_();
    };

}
}}

#endif // VK_CHAT_H_INCLUDED
