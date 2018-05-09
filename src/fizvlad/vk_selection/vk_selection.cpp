#include "vk_selection.h"

// Utility namespace. Unreachable from other files.
namespace {

    std::string js_userSubscribers(fizvlad::vk_selection::UnitId id, size_t offset = 0) {
        return "var id=" + std::to_string(id) + ";var count=1000;var offset=" + std::to_string(offset) + ";var requestsLimit=25;var result=[];var first=API.users.getFollowers({\"group_id\":id,\"count\":count,\"offset\":offset});result.push(first.items);var total=first.count;var current=first.items.length;var i=1;while (i*count<total&&i<requestsLimit){var r=API.users.getFollowers({\"user_id\":id,\"count\":count,\"offset\":offset+i*count});result.push(r.items);current=current+r.items.length;i=i+1;}return {\"total\":total,\"current\":current,\"items\":result};";
    }

    std::string js_groupMembers(fizvlad::vk_selection::UnitId id, size_t offset = 0) {
        return "var id=" + std::to_string(id) + ";var count=1000;var offset=" + std::to_string(offset) + ";var requestsLimit=25;var result=[];var first=API.groups.getMembers({\"group_id\":id,\"count\":count,\"offset\":offset});result.push(first.items);var total=first.count;var current=first.items.length;var i=1;while (i*count<total&&i<requestsLimit){var r=API.groups.getMembers({\"group_id\":id,\"count\":count,\"offset\":offset+i*count});result.push(r.items);current=current+r.items.length;i=i+1;}return {\"total\":total,\"current\":current,\"items\":result};";
    }

}


namespace fizvlad {namespace vk_selection {

    Selection::~Selection() {
        if (std::remove(name_.c_str()) != 0) {
            std::cerr << "Error: Unable to remove " << name_ << " with size of ~" << (size_ * 5 / 1000) << "MB" << std::endl;
        }
    }


    Selection Selection::operator&&(const Selection& other) const {
        Selection result(*this, name_ + "_AND");
        result.intersect(other);
        return result;
    }

    Selection Selection::operator||(const Selection& other) const {
        Selection result(*this, name_ + "_OR");
        result.join(other);
        return result;
    }

    Selection Selection::operator!() const {
        Selection result(*this, name_ + "_NOT");
        result.invert();
        return result;
    }


    void Selection::intersect(const Selection &other) {
        // TODO &&
    }

    void Selection::join(const Selection &other) {
        // TODO ||
    }

    void Selection::invert() {
        isInverted_ = !isInverted_;
        updateMeta_();
    }


    bool Selection::isInverted() const {
        return isInverted_;
    }


    size_t Selection::size() const {
        return size_;
    }


    void Selection::saveAs(Filename name) const {
        std::FILE *file = std::fopen((name + ".txt").c_str(), "w");

        std::fprintf(file, "%s", isInverted_ ? "Inverted\n" : "Not inverted\n");
        std::fprintf(file, "%s%lu%s", "Size of selection: ", size_, "\n");

        // Forced to copy class members and namespace members
        size_t s = size_;
        std::vector<std::string> utn = unitTypeNames;

        inFile_("rb", [utn, s, file](std::FILE *b_file){
            std::fseek(b_file, sizeof(char) + sizeof(size_t), SEEK_SET); // Moving to the beginning of data

            for (size_t i = 0; i < s; i++) {
                char u; // Unit type
                std::fread(&u, sizeof(char), 1, b_file);
                std::fprintf(file, "%s", utn[((int) u) - ((int) Undefined)].c_str());

                UnitId id; // Unit id
                std::fread(&id, sizeof(UnitId), 1, b_file);
                std::fprintf(file, "%c%lu%s", ' ', id, "\n");
            }
        });

        std::fclose(file);
    }


    Selection::Selection(Filename name) : isInverted_(false), size_(0), name_(name + "." + FILE_EXTENSION) {
        inFile_("wb", [this](std::FILE* file){
            char c = isInverted_ ? '1' : '0';
            std::fwrite(&c, sizeof(char), 1, file);
            std::fwrite(&size_, sizeof(size_t), 1, file);
        });
    }


    Selection::Selection(const Selection& other, Filename name)  : isInverted_(other.isInverted_), size_(other.size_), name_(name + "." + FILE_EXTENSION) {
        inFiles2_(other, "rb", *this, "wb", [](std::FILE *source, std::FILE *target){
            size_t B_SIZE = 256;
            char buffer[B_SIZE];
            size_t s = std::fread(buffer, sizeof(char), B_SIZE, source);
            while (s) {
                std::fwrite(buffer, sizeof(char), s, target);
                s = std::fread(buffer, sizeof(char), B_SIZE, source);
            }
        });
    }


    void Selection::updateMeta_() {
        inFile_("ab", [this](std::FILE* file){
            std::fseek(file, 0, SEEK_SET); // Moving to the beginning of file
            char c = isInverted_ ? '1' : '0';
            std::fwrite(&c, sizeof(char), 1, file);
            std::fwrite(&size_, sizeof(size_t), 1, file);
        });
    }










    Unit::Unit(UnitId id, bool isUser, vk_api::Token token) : type_(Undefined), id_(id), customId_("") {
        if (isUser) {
            initUser_(id, token);
        } else {
            initGroup_(id, token);
        }
    }


    Unit::Unit(UnitCustomId cid, vk_api::Token token) : type_(Undefined), id_(0), customId_(cid)  {
        nlohmann::json re = vk_api::apiRequest("utils.resolveScreenName", {{"screen_name", cid}});
        std::string type = re["type"];
        UnitId id = re["object_id"];
        if (type == "user") {
            initUser_(id, token);
        } else if (type == "group") {
            initGroup_(id, token);
        } else if (type == "application") {
            throw std::runtime_error("Custom id " + cid + " belongs to application");
        }
    }


    UnitType Unit::type() const {
        return type_;
    }


    UnitId Unit::id() const {
        return id_;
    }


    UnitCustomId Unit::customId() const {
        return customId_;
    }


    Selection Unit::friends(vk_api::Token token) const {
        if (type_ != User) {
            throw std::runtime_error("Can not request friends for this Unit.");
        }

        vk_api::Parameters parameters = {
            {"user_id", std::to_string(id_)},
            {"count", "10000"}
        };
        nlohmann::json response = vk_api::apiRequest("friends.get", parameters, token)["items"];

        Selection result(customId_ + "_friends");
        result.inFile_("ab", [response](FILE *file){
            char pre = (char) User; // Forced to first create char pre because there is no way to save enum element into file
            for (UnitId id : response) {
                fwrite(&pre, sizeof(pre), 1, file);
                fwrite(&id, sizeof(UnitId), 1, file);
            }
        });
        result.size_ += response.size();
        result.updateMeta_();

        return result;
    }


    Selection Unit::subscribers(vk_api::Token token) const {
        // TODO User subscribers
    }


    Selection Unit::members(vk_api::Token token) const {
        if (type_ != Public && type_ != Group && type_ != Event) {
            throw std::runtime_error("Can not request members for this Unit.");
        }

        size_t total = 1;
        size_t current = 0;
        std::vector<UnitId> units;
        while (current < total) {
            // NOTICE Result of following command is array of arrays (due to API.execute restrictions)
            nlohmann::json response = vk_api::execute(js_groupMembers(id_, current), token);
            total = (size_t) response["total"];
            current += (size_t) response["current"];
            for (auto sub : response["items"]) {
                for (UnitId u : sub) {
                    units.push_back(u);
                }
            }
        }

        Selection result(customId_ + "_members");

        result.inFile_("ab", [units](FILE *file){
            char pre = (char) User; // Forced to first create char pre because there is no way to save enum element into file
            for (UnitId id : units) {
                fwrite(&pre, sizeof(pre), 1, file);
                fwrite(&id, sizeof(UnitId), 1, file);
            }
        });

        result.size_ += total;
        result.updateMeta_();
        return result;
    }


    void Unit::initUser_(UnitId id, vk_api::Token token) {
        vk_api::Parameters parameters = {
            {"fields", "screen_name"},
            {"user_ids", std::to_string(id)}
        };
        nlohmann::json userInfo = vk_api::apiRequest("users.get", parameters, token)[0];

        type_ = User;
        id_ = userInfo["id"];
        if (userInfo["screen_name"] == nullptr) {
            // Private profile?
            customId_ = "id" + std::to_string(id);
        } else {
            customId_ = userInfo["screen_name"];
        }
    }


    void Unit::initGroup_(UnitId id, vk_api::Token token) {
        nlohmann::json groupInfo = vk_api::apiRequest("groups.getById", {{"group_ids", std::to_string(id)}}, token)[0];

        std::string st = groupInfo["type"];
        if (st == "page") {
            type_ = Public;
        } else if (st == "group") {
            type_ = Group;
        } else if (st == "event") {
            type_ = Event;
        } else {
            throw vk_api::ApiRequestExpetion("Unknown unit type: " + st);
        }
        id_ = groupInfo["id"];
        customId_ = groupInfo["screen_name"];
    }










    std::ostream &operator<<(std::ostream &os, const Unit &unit) {
        return os << unitTypeNames[(int) unit.type() - (int) Undefined] << " " << unit.id() << " " << unit.customId();
    }


    bool operator<(const Unit &l, const Unit &r) {
        return l.type() == r.type() ? l.id() < r.id() : l.type() < r.type();
    }

}}
