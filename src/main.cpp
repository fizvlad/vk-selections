#include <iostream>
#include <exception>
#include <fstream>

#include "fizvlad/vk_api/vk_chat.h"

#include "selection_manager.h"

using namespace std;
using nlohmann::json;
using namespace fizvlad::vk_api::chat;
using namespace fizvlad::selection_manager;

int main() {
    setlocale(LC_CTYPE, "rus");

    fstream fin("private/groupToken.txt");
    string token;
    fin >> token;
    cout << "Token: " << token[0] << token[1] << token[2] << token[3] << "..." << endl << endl;


    Calculator calculator;


    ChatBot bot(token, 157926798);
    bot.setOnlineStatus(true);
    bot.start([&] (Message m, bool isOld) {

        cout << (isOld ? "(Old message) " : "") << m.sender << " send message to " << m.receiver << " with id " << m.id << " at " << m.ts << ". Content: " << endl << m.text << endl;
        bot.markAsRead(m);

        if (isOld) {
            bot.sendMessage(m.sender, "Обратите внимание, что будет обработано только последнее сообщение!");
        }

        if (m.text == "!help") {
            string msg = "";
            msg = msg + "Список доступных комманд: \n " +
                    "!calc <ВЫРАЖЕНИЕ> - посчитать выражение \n" +
                    "!selection <ЗАПРОС> - запрос выборки пользователей (Пример: \"!selection fizvlad_bot.members\") \n " +
                    "!stop - остановить работу бота";
            bot.sendMessage(m.sender, msg);
            return true;
        }

        if (m.text.find("!selection ") != string::npos) {
            string request = m.text.substr(11);
            bot.sendMessage(m.sender, "Получен запрос: \n " + request + "\n К сожалению, я ещё на них не отвечаю (но скоро начну)");
            return true;
        }

        if (m.text.find("!calc ") != string::npos) {
            string request = m.text.substr(6);
            try {
                float result = calculator.query(request);
                bot.sendMessage(m.sender, std::to_string(result));
            } catch(exception e) {
                bot.sendMessage(m.sender, "Ой! :0\n Случилась ошибка. Попробуй ещё раз");
                std::cerr << "ERROR: request: " << request << endl << e.what() << endl;
            }

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
