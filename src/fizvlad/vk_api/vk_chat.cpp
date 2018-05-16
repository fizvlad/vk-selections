#include "fizvlad/vk_api/vk_chat.h"

// Utility namespace. Unreachable from other files.
namespace {
    std::string js_unreadMessages(unsigned offset) {
        return "var unread=1;var preview_length=0;var count=200;var offset=" + std::to_string(offset) + ";var requestsLimit=25;var result=[];var first=API.messages.getDialogs({\"unread\":unread,\"preview_length\":preview_length,\"count\":count,\"offset\":offset});result.push(first.items);var total=first.count;var current=first.items.length;var i=1;while (i*count<total&&i<requestsLimit){var r=API.messages.getDialogs({\"unread\":unread,\"preview_length\":preview_length,\"count\":count,\"offset\":offset+i*count});result.push(r.items);current=current+r.items.length;i=i+1;}return {\"total\":total,\"current\":current,\"items\":result};";
    }
}


namespace fizvlad {namespace vk_api {
namespace chat {

    ChatBot::ChatBot(Token token, UnitId selfId) : longpoll_(token), token_(token), selfId_(selfId) {
    }


    ChatBot::~ChatBot() {
    }


    void ChatBot::markAsRead(Message message) {
        Parameters parameters = {
            {"peer_id", std::to_string(message.sender)},
            {"start_message_id", std::to_string(message.id)}
        };

        apiRequest("messages.markAsRead", parameters, token_);
    }


    void ChatBot::setOnlineStatus(bool isOnline) {
        bool currentOnline = apiRequest("groups.getOnlineStatus", {{"group_id", std::to_string(selfId_)}}, token_)["status"] != "none";
        if (currentOnline != isOnline) {
            apiRequest(isOnline ? "groups.enableOnline" : "groups.disableOnline", {{"group_id", std::to_string(selfId_)}}, token_);
        }
    }


    void ChatBot::sendMessage(UnitId receiver, std::string text, std::string attachment) {
        Parameters parameters = {
            {"user_id", std::to_string(receiver)},
            {"message", text}
        };
        if (attachment != "") {
            parameters.insert({"attachment", attachment});
        }
        apiRequest("messages.send", parameters, token_);
    }


    void ChatBot::uploadAndSend(UnitId receiver, std::string filePath, std::string fileTitle, std::string text) {
        std::string server = apiRequest("docs.getMessagesUploadServer", {{"peer_id", std::to_string(receiver)}}, token_)["upload_url"];
        std::string attachment = uploadDoc(server, filePath, fileTitle, token_);

        Parameters parameters = {
            {"user_id", std::to_string(receiver)},
            {"attachment", attachment}
        };
        if (text != "") {
            parameters.insert({"message", text});
        }

        apiRequest("messages.send", parameters, token_);
    }


    Messages ChatBot::getUnreadMessages_() {
        Messages result;

        size_t total = 1;
        size_t current = 0;
        while (current < total) {
            // NOTICE Result of following command is array of arrays (due to API.execute restrictions)
            nlohmann::json response = vk_api::execute(js_unreadMessages(current), token_);
            total = (size_t)response["total"];
            current += (size_t)response["current"];
            for (auto sub : response["items"]) {
                for (auto m : sub) {
                    result.push_back(Message(m["message"]));
                }
            }
        }

        return result;
    }

}
}}
