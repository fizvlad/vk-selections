#include "fizvlad/vk_api/vk_utility.h"

// Utility namespace. Unreachable from other files.
namespace {

    // Requests
    size_t writeFunction_(char *recievedData, size_t size, size_t nmemb, std::string *buffer) {
        size_t result = 0;
        if (buffer != nullptr) {
            buffer->append(recievedData, size * nmemb);
            result = size * nmemb;
        }
        return result;
    }
    std::string safeRequest_(std::string url) {
        CURL *curl; // curl object
        curl = curl_easy_init();
        if (!curl) {
            throw std::runtime_error("Failed to create curl object");
        }
        // Created curl

        std::string errorBuffer;
        std::string outputBuffer;

        curl_easy_setopt(curl, CURLOPT_ERRORBUFFER, &errorBuffer);
        curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
        curl_easy_setopt(curl, CURLOPT_HEADER, 0); // Don't show header
        curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1); // Do redirect
        curl_easy_setopt(curl, CURLOPT_NOBODY, 0); // Don't hide body
        curl_easy_setopt(curl, CURLOPT_USE_SSL, CURLUSESSL_ALL); // Use SSL
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &outputBuffer); // Buffer for callback function
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, writeFunction_); // Callback

        // Ready to go
        CURLcode result = curl_easy_perform(curl);
        if (result != CURLE_OK) {
            throw std::runtime_error("Request error: " + errorBuffer);
        }
        curl_easy_cleanup(curl);
        return outputBuffer;
    }


    // Uploading file
    std::string uploadDoc_(std::string server, std::string fieldName, std::string filePath) {
        CURL *curl; // curl object
        curl = curl_easy_init();
        if (!curl) {
            throw std::runtime_error("Failed to create curl object");
        }
        // Created curl

        std::string errorBuffer;
        std::string outputBuffer;

        curl_easy_setopt(curl, CURLOPT_ERRORBUFFER, &errorBuffer);
        curl_easy_setopt(curl, CURLOPT_URL, server.c_str());
        curl_easy_setopt(curl, CURLOPT_HEADER, 0); // Don't show header
        curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1); // Do redirect
        curl_easy_setopt(curl, CURLOPT_NOBODY, 0); // Don't hide body
        curl_easy_setopt(curl, CURLOPT_USE_SSL, CURLUSESSL_ALL); // Use SSL
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &outputBuffer); // Buffer for callback function
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, writeFunction_); // Callback


        #if CURL_AT_LEAST_VERSION(7,56,0)

        // Setting up MIME
        curl_mime *mime = curl_mime_init(curl);
        curl_mimepart *mimepart = curl_mime_addpart(mime);
        curl_mime_name(mimepart, fieldName.c_str());
        curl_mime_filedata(mimepart, filePath.c_str());

        // Adding MIME to request
        curl_easy_setopt(curl, CURLOPT_MIMEPOST, mime);

        // Ready to go
        CURLcode result = curl_easy_perform(curl);
        curl_easy_cleanup(curl);
        curl_mime_free(mime);

        #else // C_CURL_VERSION

        // Borders of postlist
        struct curl_httppost* beginPostList = NULL;
        struct curl_httppost* endPostList = NULL;

        curl_formadd(&beginPostList, &endPostList,
                    CURLFORM_COPYNAME, fieldName.c_str(),
                    CURLFORM_FILE, filePath.c_str(),
                    CURLFORM_END);

        // POST request
        curl_easy_setopt(curl, CURLOPT_POST, true);
        curl_easy_setopt(curl, CURLOPT_HTTPPOST, beginPostList);

        // Ready to go
        CURLcode result = curl_easy_perform(curl);
        curl_easy_cleanup(curl);

        #endif // C_CURL_VERSION


        if (result != CURLE_OK) {
            throw std::runtime_error("Request error: " + errorBuffer);
        }
        return outputBuffer;
    }


    // XML entitities
    const std::vector< std::pair<std::string, char> > XML_entities = {
        {"quot", '"'},
        {"amp", '&'},
        {"apos", '\''},
        {"lt", '<'},
        {"gt", '>'}
    };
}


namespace fizvlad {namespace vk_api {

    namespace utility {

        std::string url_encode(const std::string &str) {
            std::string result;
            CURL *curl = curl_easy_init();
            if(curl) {
                char *output = curl_easy_escape(curl, str.c_str(), str.size());
                if(output) {
                    result.append(output);
                    curl_free(output);
                } else {
                    throw std::runtime_error("Unable to encode string. escape returned NULL.");
                }
            } else {
                throw std::runtime_error("Unable to encode string. Unable to init CURL.");
            }
            curl_easy_cleanup(curl);
            return result;
        }
        std::string url_decode(const std::string &str) {
            std::string result;
            CURL *curl = curl_easy_init();
            if(curl) {
                int l;
                char *output = curl_easy_unescape(curl, str.c_str(), str.size(), &l);
                if(output) {
                    result.append(output, l);
                    curl_free(output);
                } else {
                    throw std::runtime_error("Unable to decode string. unescape returned NULL.");
                }
            } else {
                throw std::runtime_error("Unable to decode string. Unable to init CURL.");
            }
            curl_easy_cleanup(curl);
            return result;
        }

        std::string xml_decode(const std::string &str) {
            std::string result = "";
            for (size_t i = 0; i < str.size(); i++) {
                bool ifReplaced = false;
                if (str[i] == '&') {
                    for (std::pair<std::string, char> entity : XML_entities) {
                        if (str.substr(i + 1, entity.first.size()) == entity.first) {
                            result += entity.second;
                            ifReplaced = true;
                            i += entity.first.size() + 1;
                            break;
                        }
                    }
                }
                if (!ifReplaced) {
                    result += str[i];
                }
            }
            return result;
        }

    }


    std::string to_string(Parameters parameters) {
        std::stringstream stream;
        stream << "?";
        for (Parameter p : parameters) {
            stream << utility::url_encode(p.first) << "=" << utility::url_encode(p.second) << "&";
        }
        std::string result = stream.str();
        if (result.size() != 0) {
            // Deleting last symbol of parameters string
            result.pop_back();
        }
        return result;
    }


    nlohmann::json jsonRequest(Url url, Parameters parameters) {
        std::string responseString = safeRequest_(url + to_string(parameters));
        nlohmann::json response;
        try {
            response = nlohmann::json::parse(responseString);
        } catch (...) {
            throw std::runtime_error("Unable to parse response into JSON. Request URL: " + url + to_string(parameters) + "\nResponse: " + response.dump());
        }
        return response;
    }


    nlohmann::json apiRequest_raw(Method methodName, Parameters parameters, Token token, Version version) {
        parameters.insert({"access_token", token});
        parameters.insert({"v", version});
        return jsonRequest(VK_URL_PREFIX + methodName, parameters);
    }


    nlohmann::json apiRequest(Method methodName, Parameters parameters, Token token, Version version) {
        nlohmann::json response = apiRequest_raw(methodName, parameters, token, version);
        if (response.find("response") != response.end()) {
            return response["response"];
        } else if (response.find("error") != response.end()) {
            nlohmann::json e = response["error"];
            std::cerr << "Error occured: \n" << e.dump() << std::endl;
            return e;
        } else {
            throw std::runtime_error("Unable to find re[\"response\"] or re[\"error\"]. \nResponse: " + response.dump());
        }
    }


    nlohmann::json execute(JScode code, Token token, Version version) {
        Parameters parameters = {{"code", code}};
        return apiRequest("execute", parameters, token, version);
    }


    std::string uploadDoc(std::string server, std::string filePath, std::string title, std::string token) {
        std::string responseString = uploadDoc_(server, "file", filePath);
        nlohmann::json uploadResponse;
        std::string fileStr;
        try {
            uploadResponse = nlohmann::json::parse(responseString);
            fileStr = uploadResponse["file"];
        } catch (...) {
            throw std::runtime_error("Unable to parse response into JSON.\nResponse: " + uploadResponse.dump());
        }

        nlohmann::json response = apiRequest("docs.save", {{"file", fileStr}, {"title", title}}, token)[0];
        std::string result = "doc" + std::to_string((long) response["owner_id"]) + "_" + std::to_string((long) response["id"]);
        return result;
    }

}}
