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
    Unit gag = Unit("ru9gag", token);
    Unit kinopoisk = Unit("kinopoisk", token);
    Unit stepik = Unit("rustepik", token);
    Unit Filyus_Bot = Unit(283177744, true, token);
    Unit Fizvlad = Unit("id157230821", token);
    Unit buerak = Unit("buerak_spring", token);
    cout << habr << endl << gag << endl << kinopoisk << endl << stepik << endl << Filyus_Bot << endl << Fizvlad << endl << buerak << endl;

    // Works fine
    Selection test = Filyus_Bot.friends(token2);
    (!test).saveAs("Not_Filyus_friends");

    // Works fine. Requires much time
    Selection test2 = stepik.members(token);
    test2.saveAs("stepik_members");

    Selection test3 = habr.members(token);
    test3.saveAs("habr_members");

    // Must be at least me . _.
    (test2 && test3).saveAs("habr_and_stepik_members");

    return 0;
}
