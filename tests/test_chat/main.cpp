#include <iostream>
#include <fstream>

#include "fizvlad/vk_api/vk_chat.h"

using namespace std;
using nlohmann::json;
using namespace fizvlad::vk_api::chat;

int main() {
    setlocale(LC_CTYPE, "rus");

    fstream fin("private/groupToken.txt");
    string token;
    fin >> token;
    cout << "Token: " << token[0] << token[1] << token[2] << token[3] << "..." << endl << endl;

    ChatBot bot(token, 157926798);
    bot.setOnlineStatus(true);
    bot.start([&] (Message m, bool isOld) {
        cout << (isOld ? "(Old message) " : "") << m.sender << " send message to " << m.receiver << " with id " << m.id << " at " << m.ts << ". Content: " << endl << m.text << endl;
        if (m.text == "!help") {
            bot.markAsRead(m);
            bot.sendMessage(m.sender, "Список доступных комманд:\n !read - прочесть сообщения \n !answer - ответить в беседу \n !stop - остановить работу бота");
        } else if (m.text == "!read") {
            bot.markAsRead(m);
        } else if (m.text == "!answer") {
            bot.markAsRead(m);
            bot.sendMessage(m.sender, "Честно говоря, мне надоел весь этот чёртов фарс");
        } else if (m.text == "!stop") {
            bot.markAsRead(m);
            bot.setOnlineStatus(false);
            return false;
        }
        return true;
    });

    return 0;
}
