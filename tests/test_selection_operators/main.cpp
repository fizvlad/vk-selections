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

    cout << "Loading groups members" << endl;

    Selection habr =       Unit("habr", token).members(token);
    cout << "Habrahabr. " << habr << endl;

    Selection vkdev =      Unit("apiclub", token).members(token);
    cout << "Developers of VK API. " << vkdev << endl;

    Selection javarush =   Unit("JavaRush", token).members(token);
    cout << "Students of JavaRush. " << javarush << endl;

    Selection geekbrains = Unit("geekbrainsru", token).members(token);
    cout << "Students of Geekbrains. " << geekbrains << endl;

    Selection stepik =     Unit("rustepik", token).members(token);
    cout << "Students of Stepik. " << stepik << endl;

    Selection itmo =       Unit("itmoru", token).members(token);
    cout << "Students of ITMO university. " << itmo << endl;

    Selection mm =         Unit("mmspbu", token).members(token);
    cout << "Students of SPBU university - Math and Mechanics. " << mm << endl;

    cout << "Loaded all groups members" << endl;

    cout << endl;

    Selection test1 = habr && vkdev;
    cout << "Test 1 loaded. Inverted: " << (test1.isInverted() ? "true" : "false") << ". " << "Size: " << test1.size() << endl;
    test1.saveAs("SELECTION_vk_api_developers_AND_habr_subscribers");

    cout << endl;

    Selection test2 = javarush || geekbrains || stepik;
    cout << "Test 2 loaded. Inverted: " << (test2.isInverted() ? "true" : "false") << ". " << "Size: " << test2.size() << endl;
    test2.saveAs("SELECTION_online_educated_devs");

    cout << endl;

    Selection test3 = (itmo || mm) && test2;
    cout << "Test 3 loaded. Inverted: " << (test3.isInverted() ? "true" : "false") << ". " << "Size: " << test3.size() << endl;
    test3.saveAs("SELECTION_online_educated_devs_from_IT_universities");

    cout << endl;

    Selection test4 = habr && !geekbrains;
    cout << "Test 4 loaded. Inverted: " << (test4.isInverted() ? "true" : "false") << ". " << "Size: " << test4.size() << endl;
    test4.saveAs("SELECTION_habr_subscribers_AND_not_geekbrains_subscribers");

    return 0;
}
