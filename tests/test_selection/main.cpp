#include <iostream>
#include <fstream>

#include "vk_selection.h"

using namespace std;
using nlohmann::json;
using namespace fizvlad::vk_selection;

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

    Unit habr = Unit("habr", token);
    Unit music = Unit("exclusive_muzic", token); // Largest public takes 3 minute to load ._ .
    Unit stepik = Unit("rustepik", token);
    Unit Filyus_Bot = Unit(283177744, true, token);
    Unit buerak = Unit("buerak_spring", token);
    cout << habr << endl
         << music << endl
         << stepik << endl
         << Filyus_Bot << endl
         << buerak << endl;

    // Works fine
    Selection test = Filyus_Bot.friends(token2);
    test.saveAs("result_Not_Filyus_friends");

    // Works fine. Requires time
    Selection test2 = habr.members(token);
    test2.saveAs("result_Habrahabr_subscribers");

    // Works fine. Requires lot of time
    //Selection test3 = music.members(token);
    //test3.saveAs("result_Music_subscribers");

    return 0;
}
