#ifndef VK_LONGPOLL_H_INCLUDED
#define VK_LONGPOLL_H_INCLUDED

#include "vk_utility.h"

namespace fizvlad {namespace vk_api {
namespace longpoll {

    const Version VERSION = "3";

    /// Pesudonym for unsigned. Used in flags for longpoll mode and events.
    typedef unsigned Flag;

    /// Longpoll request mode.
    namespace MODE {
        const Flag GET_ATTACHMENTS = 2; // 1   2
        const Flag EXTENDED_EVENTS = 8; // 3   8
        const Flag GET_PTS         = 32; // 5  32
        const Flag GET_EXTRA       = 64; // 6  64
        const Flag GET_RANDOM_ID   = 128; // 7 128
    }


    /// Id of events which can be received with longpoll.
    namespace EVENTS {
        namespace MESSAGE {
            namespace FLAGS {
                const Flag CHANGE = 1;
                const Flag SET    = 2;
                const Flag RESET  = 3;
            }
            const Flag NEW    = 4;
            const Flag EDITED = 5;

            const Flag INPUT_READ    = 6; // Flag of incoming  letter changed to 'read'
            const Flag OUTPUT_READ   = 7; // User read our message

            const Flag DELETED   = 13;
            const Flag RECOVERED = 14;
        }

        namespace USER {
            const Flag ONLINE  = 8;
            const Flag OFFLINE = 9;

            const Flag TYPING = 61;
            const Flag TYPING_IN_CHAT = 62;
            const Flag CALLED = 70;
        }

        namespace DIALOGUE {
            namespace FLAGS // Only for group messages
            {
                const Flag RESET  = 10;
                const Flag CHANGE = 11;
                const Flag SET    = 12;
            }
            const Flag PARAMETERS_CHANGED = 51;
        }

        namespace OTHER {
            const Flag COUNTER_CHANGED = 80;
            const Flag NOTIFICATIONS_PARAMETERS_CHANGED = 114;
        }
    }


    /// Message flags.
    namespace MESSAGE_FLAGS {
        const Flag UNREAD = 1;
        const Flag OUTBOX = 2;
        const Flag REPLIED = 4;
        const Flag IMPORTANT = 8;
        const Flag CHAT = 16; // DEPRICATED
        const Flag FRIENDS = 32;
        const Flag SPAM = 64;
        const Flag DELETED = 128;
        const Flag FIXED = 256; // DEPRICATED
        const Flag MEDIA = 512; // DEPRICATED
        const Flag HIDDEN = 65536;
        const Flag DELETED_FOR_ALL = 131072;
    }


    /// Class for work with longpoll.
    class Session {
    public:

        /// Pseudonym for unsigned short. Time of request timeout in seconds.
        typedef unsigned short Timeout;

        /// Pseudonym for json. Used as type of array of updates
        typedef nlohmann::json Updates;

        /// Initialize longpoll session
        Session(Token token, Flag mode = 0, Timeout timeout = 25, Version version = VERSION);

        Session &operator=(const Session&) = delete;
        Session(const Session&) = delete;
        Session &operator=(Session&&) = delete;
        Session(Session&&) = delete;

        ~Session();


        /// Single longpoll request. Return f(updates).
        template <typename Func>
        bool listen_once(Func f) {
            // TODO Correct work with function from template
            Updates updates = request_();
            return f(updates);
        }


        /// N of logpoll requests. Stop listening early if handler return FALSE.
        template <typename Func>
        bool listen_n(Func f, unsigned n) {
            // TODO Correct work with function from template
            for (unsigned i = 0; i < n; i++) {
                if (!listen_once(f)) {
                    return false;
                }
            }
            return true;
        }


        /// Listening while predicate returns TRUE. Stop listening early if handler return FALSE.
        template <typename Func, typename Predicate>
        bool listen_while(Func f, Predicate p) {
            // TODO Correct work with functions from template
            while (p()) {
                if (!listen_once(f)) {
                    return false;
                }
            }
            return true;
        }


        /// Listen while handler return TRUE.
        template <typename Func>
        bool listen(Func f) {
            // TODO Correct work with function from template
            while (true) {
                if (!listen_once(f)) {
                    return false;
                }
            }
            return true;
        }


    private:

        Url     server_;
        Token   key_;
        time_t  ts_;
        Flag    mode_;
        Timeout timeout_;
        Version version_;

        /// Setting up access token. Necessary for requests.
        void initialize_(Token token);

        /// Single request for changes.
        Updates request_();
    };

}
}}

#endif // VK_LONGPOLL_H_INCLUDED
