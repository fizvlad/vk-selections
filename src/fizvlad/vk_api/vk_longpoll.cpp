#include "fizvlad/vk_api/vk_longpoll.h"

namespace fizvlad {namespace vk_api {
namespace longpoll {

    Session::Session(Token token, Flag mode, Timeout timeout, Version version) : token_(token), server_(""), key_(""), ts_(0), mode_(mode), timeout_(timeout), version_(version) {
        initialize_();
    }


    Session::~Session() {
    }


    void Session::initialize_() {
        Parameters parameters = {{"need_pts", mode_ & MODE::GET_PTS ? "1" : "0"}, {"lp_version", version_}};
        nlohmann::json response = apiRequest("messages.getLongPollServer", parameters, token_);
        server_ = response["server"];
        key_    = response["key"];
        ts_     = response["ts"];
    }


    nlohmann::json Session::request_() {
        std::string PREFIX_PROTOCOL = "https://";
        Url url = PREFIX_PROTOCOL + server_;
        Parameters parameters = {
            {"version", version_},
            {"act", "a_check"},
            {"key", key_},
            {"ts", std::to_string(ts_)},
            {"wait", std::to_string(timeout_)},
            {"mode", std::to_string(mode_)}
        };

        nlohmann::json response = jsonRequest(url, parameters);
        if (response == nullptr) {
            std::cerr << "Longpoll server returned null" << std::endl;
            return request_();
        }
        if (response.find("failed") != response.end()) {
            int errCode = response["failed"];
            std::cerr << "Longpoll error": << errCode << std::endl;
            switch (errCode) {
                case ERROR_CODES::UPDATE_TS:
                    ts_ = response["ts"];
                    return request_();
                    break;
                case ERROR_CODES::UPDATE_KEY:
                case ERROR_CODES::UPDATE_ALL:
                    initialize_();
                    return request_();
                    break;
                default:
                    throw ApiRequestException("Longpoll request error. Response: " + response.dump());
            }
        }

        ts_ = response["ts"];
        return response["updates"];
    }

}
}}
