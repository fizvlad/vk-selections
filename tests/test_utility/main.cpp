#include <iostream>
#include <fstream>

#include "vk_utility.h"

using namespace std;
using nlohmann::json;
using namespace fizvlad::vk_api;

int main() {
    setlocale(LC_CTYPE, "rus");

    fstream fin("private/groupToken.txt");
    string token;
    fin >> token;
    cout << "Token: " << token[0] << token[1] << token[2] << token[3] << "..." << endl << endl;


    Parameters p = {{"a", "1"}, {"b", "2"}, {"sample text", "sample 2"}};
    cout << "Parameters test: " << to_string(p) << endl << endl;


    json members = apiRequest("groups.getMembers", {{"group_id","157926798"}}, token);
    cout << "apiRequest test #1: " << endl << members.dump() << endl << endl;
    return 0;
}
