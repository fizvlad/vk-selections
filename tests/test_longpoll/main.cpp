#include <iostream>
#include <fstream>

#include "vk_longpoll.h"

using namespace std;
using nlohmann::json;
using namespace fizvlad::vk_api;
using namespace fizvlad::vk_api::longpoll;

int main() {
    setlocale(LC_CTYPE, "rus");

    fstream fin("private/groupToken.txt");
    string token;
    fin >> token;
    cout << "Token: " << token[0] << token[1] << token[2] << token[3] << "..." << endl << endl;

    Session s(token, MODE::EXTENDED_EVENTS | MODE::GET_RANDOM_ID);
    s.listen([&](Session::Updates upd){
        if (upd.size() != 0) {
            cout << "Received new events." << endl;
        } else {
            cout << "No new events." << endl;
        }

        bool receivedMessage = false;
        for (auto i : upd) {
            cout << "  " << i << endl;
            if ((unsigned short)i[0] == EVENTS::MESSAGE::NEW) {
                cout << "User with id" << i[3] << " at " << i[4] << ": " << i[5] << endl;
                receivedMessage = true;
            }
        }
        return !receivedMessage;
     });

}
