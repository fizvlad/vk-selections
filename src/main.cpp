#include <iostream>
#include <fstream>

#include "vk_chat.h"
#include "vk_selection.h"
#include "cpp-formula.h"

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
        bot.markAsRead(m);

        if (isOld) {
            bot.sendMessage(m.sender, "Обратите внимание, что будет обработано только последнее сообщение!");
        }

        if (m.text == "!help") {
            bot.sendMessage(m.sender, "Список доступных комманд: \n !selection <ЗАПРОС> - запрос выборки пользователей (Пример: \"!selection fizvlad_bot.members\") \n !stop - остановить работу бота");
            return true;
        }

        if (m.text.find("!selection ") != string::npos) {
            string request = m.text.substr(11);
            bot.sendMessage(m.sender, "Получен запрос: \n " + request + "\n К сожалению, я ещё на них не отвечаю (но скоро начну)");
            return true;
        }

        if (m.text == "!stop") {

            bot.setOnlineStatus(false);
            return false;
        }


        bot.sendMessage(m.sender, "Неизвестная команда. Отправьте \"!help\", чтобы увидеть список доступных комманд");
        return true;

    });

    return 0;
}
