#include <iostream>
#include <sstream>
#include <exception>
#include <fstream>

#include "fizvlad/vk_api/vk_chat.h"

#include "selection_manager.h"

using namespace std;
using nlohmann::json;
using namespace fizvlad::vk_api::chat;
using namespace fizvlad::vk_selection;
using namespace fizvlad::selection_manager;

int main() {
    setlocale(LC_CTYPE, "rus");

    fstream fin("private/groupToken.txt");
    string token;
    fin >> token;
    cout << "Token: " << token[0] << token[1] << token[2] << token[3] << "..." << endl << endl;

    fstream fin2("private/serviceToken.txt");
    string token2;
    fin2 >> token2;
    cout << "Token: " << token2[0] << token2[1] << token2[2] << token2[3] << "..." << endl << endl;

    size_t maxToDisplay = 15;

    Calculator calculator;
    Selector selector(token, token2);


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
            try {
                bot.sendMessage(m.sender, "Запрос получен");
                cout << "Selection request: " << request << endl;
                Selection result = selector.query(request);
                stringstream msg;
                msg << "Выборка" << (result.isInverted() ? " " : " не ") << "инвертирована. Размер: " << result.size();
                bot.sendMessage(m.sender, msg.str());
                if (result.size() <= maxToDisplay) {
                    string output = "";
                    result.forEach([&output](UnitType type, UnitId id){
                        output += unitTypeName(type) + " " + to_string(id) + "\n";
                    });
                    bot.sendMessage(m.sender, output);
                } else {
                    result.saveAs(to_string(m.sender));
                    string filename = to_string(m.sender) + ".txt";
                    bot.uploadAndSend(m.sender, filename, "Selection.txt");
                    remove(filename.c_str());
                }
            } catch(exception e) {
                bot.sendMessage(m.sender, "Ой! :0\n Случилась ошибка. Попробуйте ещё раз");
                cerr << "ERROR: request: " << request << endl << "what: " << e.what() << endl;
            }
            return true;
        }


        if (m.text.find("!calc ") != string::npos) {
            string request = m.text.substr(6);
            try {
                float result = calculator.query(request);
                // Using stringstream for beautified output
                stringstream temp;
                temp << result;
                bot.sendMessage(m.sender, temp.str());
            } catch(exception e) {
                bot.sendMessage(m.sender, "Ой! :0\n Случилась ошибка. Попробуйте ещё раз");
                cerr << "ERROR: request: " << request << endl << e.what() << endl;
            }

            return true;
        }


        if (m.text == "!stop") {

            bot.setOnlineStatus(false);
            return false; // Stop listening
        }


        bot.sendMessage(m.sender, "Неизвестная команда. Отправьте \"!help\", чтобы увидеть список доступных комманд");
        return true;

    });

    return 0;
}
