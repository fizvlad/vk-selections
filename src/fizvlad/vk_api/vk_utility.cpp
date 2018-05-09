#include "vk_utility.h"

// Utility namespace. Unreachable from other files.
namespace {

    // URL encode/decode
    std::string str_encode (std::string str) {
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
    std::string str_decode (std::string str) {
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


    // Requests
    size_t writeFunction_ (char *recievedData, size_t size, size_t nmemb, std::string *buffer) {
        size_t result = 0;
        if (buffer != nullptr) {
            buffer->append(recievedData, size * nmemb);
            result = size * nmemb;
        }
        return result;
    }
    std::string safeRequest_ (std::string url) {
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
            throw std::runtime_error(errorBuffer.c_str());
        }
        curl_easy_cleanup(curl);
        return outputBuffer;
    }

}


namespace fizvlad {namespace vk_api {

    std::string to_string(Parameters parameters) {
        std::stringstream stream;
        stream << "?";
        for (Parameter p : parameters) {
            stream << str_encode(p.first) << "=" << str_encode(p.second) << "&";
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
            std::cerr << "Error occured: \n" << e.dump();
            return e;
        } else {
            throw std::runtime_error("Unable to find re[\"response\"] or re[\"error\"]. \nResponse: " + response.dump());
        }
    }


    nlohmann::json execute(JScode code, Token token, Version version) {
        Parameters parameters = {{"code", code}};
        return apiRequest("execute", parameters, token, version);
    }

}}
