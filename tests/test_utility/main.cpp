#include <iostream>
#include <fstream>

#include "fizvlad/vk_api/vk_utility.h"

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
    cout << "Parameters test: " << to_string(p) << endl;

    cout << utility::xml_decode("XML encode test: amp:&amp; quot:&quot; unknown:&asfasf; ") << endl << endl;


    string server = apiRequest("docs.getMessagesUploadServer", {{"peer_id", "157230821"}}, token)["upload_url"];
    cout << "Server for file posting: " << endl << server << endl << endl;

    string attachment = uploadDoc(server, "test.txt", "testTitle", token);
    cout << "Attachment: " << attachment << endl;

    apiRequest("messages.send", {{"user_id", "157230821"}, {"attachment", attachment}}, token);
    cout << "Sent message" << endl;
    return 0;
}
