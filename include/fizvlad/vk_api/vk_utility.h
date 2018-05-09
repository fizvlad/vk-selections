#ifndef VK_UTILITY_H_INCLUDED
#define VK_UTILITY_H_INCLUDED

#include <string> // std::string
#include <sstream> // std::stringstream
#include <iostream> // std::cerr
#include <utility> // std::pair
#include <unordered_map> // std::unordered_map
#include <exception> // std::exception

#include "curl.h" // curl
#include "json.hpp" // json


namespace fizvlad {namespace vk_api {

    /// VK_API exception class.
    class ApiRequestExpetion : std::exception {
    public:

        explicit ApiRequestExpetion(const char* message) : what_(message) {}
        explicit ApiRequestExpetion(const std::string message) : what_(message) {}

        const char* what() const noexcept {
            return what_.c_str();
        }


    private:

        std::string what_;
    };


    /// Pseudonym for std::string type. Used for URL.
    typedef std::string Url;


    /// Pseudonym for std::pair. Used for parameter.
    typedef std::pair<std::string, std::string> Parameter;

    /// Pseudonym for std::unordered_map.
    typedef std::unordered_map<std::string, std::string> Parameters;


    /// Create parameters string "PARAMETER_NAME_1=KEY_1&PARAMETER_NAME_2=KEY_2".
    /// All parameters keys and values will be url encoded.
    std::string to_string(Parameters parameters);


    /// Send HTTPS request for given url.
    /// Notice: Request is sent to page "${url}${to_string(parameters)}" so it is acceptable to provide string with already added parameters.
    nlohmann::json jsonRequest(Url url, Parameters parameters = {});


    /// Pseudonym for std::string type. Used for method name in api requests.
    typedef std::string Method;


    /// Pseudonym for std::string type. Used for token in api requests.
    typedef std::string Token;


    /// Pseudonym for std::string type. Used for version in api requests.
    typedef std::string Version;

    const Url VK_URL_PREFIX = "https://api.vk.com/method/";
    const Version VERSION = "5.74";


    /// Send HTTPS request for given method and return full response.
    nlohmann::json apiRequest_raw(Method methodName, Parameters parameters = {}, Token token = "", Version version = VERSION);


    /// Send HTTPS request for given method and return response["response"].
    nlohmann::json apiRequest(Method methodName, Parameters parameters = {}, Token token = "", Version version = VERSION);


    /// Pseudonym for std::string type. Used for code in execute request.
    typedef std::string JScode;


    /// Executes given JS code on VK server.
    /// Please take a look at https://vk.com/dev/execute before use.
    nlohmann::json execute(JScode code, Token token, Version version = VERSION);

}}

#endif // VK_UTILITY_H_INCLUDED
