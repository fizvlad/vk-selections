#include <iostream>
#include <sstream>
#include <exception>
#include <fstream>
#include <unordered_map>

#include "fizvlad/vk_api/vk_chat.h"

#include "selection_manager.h"

using namespace std;
using nlohmann::json;
using namespace fizvlad::vk_api::chat;
using namespace fizvlad::vk_selection;
using namespace fizvlad::selection_manager;

int main() {
    setlocale(LC_CTYPE, "rus");

    ifstream fin("bot.config");
    unordered_map<string, string> config;
    while(fin.peek() != EOF) {
        string key, val;
        fin >> key >> val;
        config.insert({key, val});
    }
    cout << "Loaded config. Amount of parameters: " << config.size() << endl;




try {
    Calculator calculator;
    Selector selector(config["group_token"], config["service_token"]);


    ChatBot bot(config["group_token"], stoul(config["group_id"]));
    if (config["change_online"] == "1") {
        bot.setOnlineStatus(true);
    }




    cout << "Starting bot" << endl;
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
                if (result.size() <= stoul(config["max_units_to_display"])) {
                    string output = "";
                    if (config["display_mode"] == "0") {
                        result.forEach([&output](UnitType type, UnitId id){
                            output += unitTypeName(type) + " " + to_string(id) + "\n";
                        });
                    } else if (config["display_mode"] == "1") {
                        result.forEach([&output](UnitType type, UnitId id){
                            output += "www.vk.com/" + unitTypePrefix(type) + to_string(id) + "\n";
                        });
                    } else if (config["display_mode"] == "2") {
                        result.forEach([&output](UnitType type, UnitId id){
                            output += "@" + unitTypePrefix(type) + to_string(id) + "\n";
                        });
                    }
                    bot.sendMessage(m.sender, output);
                } else {
                    result.saveAs(to_string(m.sender));
                    string filename = to_string(m.sender) + ".txt";
                    bot.uploadAndSend(m.sender, filename, "Selection.txt");
                    remove(filename.c_str());
                }
            } catch(fizvlad::vk_api::ApiRequestException e) {
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
            if (m.sender != stoul(config["master_id"])) {
                bot.sendMessage(m.sender, "У тебя здесь нет власти");
                return true;
            }
            if (config["change_online"] == "1") {
                bot.setOnlineStatus(false);
            }
            return false; // Stop listening
        }


        bot.sendMessage(m.sender, "Неизвестная команда или не указаны параметры вызова. \nОтправьте \"!help\", чтобы увидеть список доступных комманд");
        return true;

    });
} catch (fizvlad::vk_api::ApiRequestException e) {
    cerr << "ERROR: " << e.what() << endl;
}

    return 0;
}
