#include "vk_selection.h"

// Utility namespace. Unreachable from other files.
namespace {

    std::string js_userSubscribers(fizvlad::vk_selection::UnitId id, size_t offset = 0) {
        return "var id=" + std::to_string(id) + ";var count=1000;var offset=" + std::to_string(offset) + ";var requestsLimit=25;var result=[];var first=API.users.getFollowers({\"group_id\":id,\"count\":count,\"offset\":offset});result.push(first.items);var total=first.count;var current=first.items.length;var i=1;while (i*count<total&&i<requestsLimit){var r=API.users.getFollowers({\"user_id\":id,\"count\":count,\"offset\":offset+i*count});result.push(r.items);current=current+r.items.length;i=i+1;}return {\"total\":total,\"current\":current,\"items\":result};";
    }

    std::string js_groupMembers(fizvlad::vk_selection::UnitId id, size_t offset = 0) {
        return "var id=" + std::to_string(id) + ";var count=1000;var offset=" + std::to_string(offset) + ";var requestsLimit=25;var result=[];var first=API.groups.getMembers({\"group_id\":id,\"count\":count,\"offset\":offset});result.push(first.items);var total=first.count;var current=first.items.length;var i=1;while (i*count<total&&i<requestsLimit){var r=API.groups.getMembers({\"group_id\":id,\"count\":count,\"offset\":offset+i*count});result.push(r.items);current=current+r.items.length;i=i+1;}return {\"total\":total,\"current\":current,\"items\":result};";
    }


    /// Struct to stor unit type and id. Will be used to sort units.
    /// Only for use in anonumous namespace to interact with file.
    struct util_Unit {
        // Using char instead of UnitType. UnitType is enum:char though so it doesn't really matter.
        char type;
        fizvlad::vk_selection::UnitId id;

        util_Unit() : type(fizvlad::vk_selection::Undefined), id(0) {}

        /// Creating by reading from selection file.
        /// Returnes cursor back if pickOnly.
        util_Unit(std::FILE *file, bool pickOnly = false) {
            next(file, pickOnly);
        }

        /// Returnes thether have reached EOF.
        bool next(std::FILE *file, bool pickOnly = false) {
            std::fread(&type, sizeof(char), 1, file);
            std::fread(&id, sizeof(fizvlad::vk_selection::UnitId), 1, file);
            if (pickOnly) {
                std::fseek(file, -(sizeof(char) + sizeof(fizvlad::vk_selection::UnitId)), SEEK_CUR);
            }
            return std::feof(file) != 0;
        }

        /// Saves to unit to target file.
        void saveTo(std::FILE *target) {
            fwrite(&type, sizeof(type), 1, target);
            fwrite(&id, sizeof(id), 1, target);
        }
    };

    bool operator<(const util_Unit &l, const util_Unit &r) {
        return l.type == r.type ? l.id < r.id : l.type < r.type;
    }
    bool operator==(const util_Unit &l, const util_Unit &r) {
        return l.type == r.type && l.id == r.id && l.type == r.type;
    }
    bool operator>(const util_Unit &l, const util_Unit &r) {
        return !(l < r || l == r);
    }
    bool operator!=(const util_Unit &l, const util_Unit &r) {
        return !(l == r);
    }
    bool operator<=(const util_Unit &l, const util_Unit &r) {
        return l < r || l == r;
    }
    bool operator>=(const util_Unit &l, const util_Unit &r) {
        return l > r || l == r;
    }


    // Notice: Files cursors must be in place.
    // Notice: Parsing till the end of the file.
    // Notice: Files should be sorted by ascending of Units.

    void template_ToFile_(std::FILE *target, std::FILE *source1, std::FILE *source2,
        bool save1_if_no2 = true,
        bool save2_if_no1 = true,
        bool save_if_equal = true,
        bool save1_if_different = true,
        bool save2_if_different = true
    ) {
        bool ifEOF1 = std::feof(source1) != 0;
        bool ifEOF2 = std::feof(source2) != 0;
        while (true) {
            // If one of sources is now empty
            util_Unit unit;
            if (ifEOF1) {
                while(!ifEOF2) {
                    ifEOF2 = unit.next(source2);
                    if (save1_if_no2) unit.saveTo(target);
                }
                return;
            }
            if (ifEOF2) {
                while(!ifEOF1) {
                    ifEOF1 = unit.next(source1);
                    if (save2_if_no1) unit.saveTo(target);
                }
                return;
            }


            // While still have something in both sources

            // Peeking single units to compare
            util_Unit unit1(source1);
            util_Unit unit2(source2);

            if (unit1 == unit2) {

                // Similar units
                while (!ifEOF1 && !ifEOF2 && unit1 == unit2) {
                    if (save_if_equal) unit1.saveTo(target);

                    ifEOF1 = unit1.next(source1);
                    ifEOF2 = unit2.next(source2);
                }
                if (unit1 != unit2) {
                    // Falling back
                    std::fseek(source1, -(sizeof(char) + sizeof(fizvlad::vk_selection::UnitId)), SEEK_CUR);
                    std::fseek(source2, -(sizeof(char) + sizeof(fizvlad::vk_selection::UnitId)), SEEK_CUR);
                } else {
                    // Reached EOF. But unit1 == unit2 so we need to save it
                    if (save_if_equal) unit1.saveTo(target);
                }

            } else if (unit1 < unit2) {

                while (!ifEOF1 && unit1 < unit2) {
                    if (save1_if_different) unit1.saveTo(target);
                    ifEOF1 = unit1.next(source1);
                }
                if (unit1 >= unit2) {
                    // Falling back
                    std::fseek(source1, -(sizeof(char) + sizeof(fizvlad::vk_selection::UnitId)), SEEK_CUR);
                } else {
                    // Reached EOF. But unit1 < unit2 so we need to save it
                    if (save1_if_different) unit1.saveTo(target);
                }

            } else {

                while (!ifEOF2 && unit2 < unit1) {
                    if (save2_if_different) unit2.saveTo(target);
                    ifEOF2 = unit2.next(source2);
                }
                if (unit2 >= unit1) {
                    // Falling back
                    std::fseek(source2, -(sizeof(char) + sizeof(fizvlad::vk_selection::UnitId)), SEEK_CUR);
                } else {
                    // Reached EOF. But unit2 < unit1 so we need to save it
                    if (save2_if_different) unit2.saveTo(target);
                }

            }
        }
    }


    // All elements from source1 but not from source2
    void exclusionToFile_(std::FILE *target, std::FILE *source1, std::FILE *source2) {
        template_ToFile_(target, source1, source2, true, false, false, true, false);
    }

    void mergerToFile_(std::FILE *target, std::FILE *source1, std::FILE *source2) {
        template_ToFile_(target, source1, source2, true, true, true, true, true);
    }

    void intersectionToFile_(std::FILE *target, std::FILE *source1, std::FILE *source2) {
        template_ToFile_(target, source1, source2, false, false, true, false, false);
    }
}


namespace fizvlad {namespace vk_selection {

    Selection::~Selection() {
        bool autoremove = true;
        if (autoremove) {
            removeFile_();
        }

    }


    Selection Selection::operator&&(Selection& other) {
        Selection result;
        bool ifDifferentInvertation = isInverted_ != other.isInverted_;
        if (ifDifferentInvertation) {
            // Selections got different invertations.
            // We will exclude some units from uninverted selection.
            // Result is uninverted
            bool ifThisInverted = isInverted_;
            inFiles3_("rb", other, "rb", result, "ab", [ifThisInverted](std::FILE *thisFile, std::FILE *otherFile, std::FILE *resultFile){
                std::fseek(thisFile, sizeof(char) + sizeof(size_t), SEEK_SET);
                std::fseek(otherFile, sizeof(char) + sizeof(size_t), SEEK_SET);
                exclusionToFile_(resultFile, ifThisInverted ? otherFile : thisFile, ifThisInverted ? thisFile : otherFile);
            });
        } else {
            // Selections got same invertations.
            if (isInverted_) {
                // We will find merger.
                result.invert();
                // Result is inverted.
                inFiles3_("rb", other, "rb", result, "ab", [](std::FILE *thisFile, std::FILE *otherFile, std::FILE *resultFile){
                    std::fseek(thisFile, sizeof(char) + sizeof(size_t), SEEK_SET);
                    std::fseek(otherFile, sizeof(char) + sizeof(size_t), SEEK_SET);
                    mergerToFile_(resultFile, thisFile, otherFile);
                });
            } else {
                // We will find intersection.
                // Result is uninverted.
                inFiles3_("rb", other, "rb", result, "ab", [](std::FILE *thisFile, std::FILE *otherFile, std::FILE *resultFile){
                    std::fseek(thisFile, sizeof(char) + sizeof(size_t), SEEK_SET);
                    std::fseek(otherFile, sizeof(char) + sizeof(size_t), SEEK_SET);
                    intersectionToFile_(resultFile, thisFile, otherFile);
                });
            }
        }
        return result;
    }

    Selection Selection::operator||(Selection& other) {
        Selection result;
        bool ifDifferentInvertation = isInverted_ != other.isInverted_;
        if (ifDifferentInvertation) {
            // Selections got different invertations.
            // We will exclude some units from inverted selection.
            result.invert();
            // Result is inverted.
            bool ifThisInverted = isInverted_;
            inFiles3_("rb", other, "rb", result, "ab", [ifThisInverted](std::FILE *thisFile, std::FILE *otherFile, std::FILE *resultFile){
                std::fseek(thisFile, sizeof(char) + sizeof(size_t), SEEK_SET);
                std::fseek(otherFile, sizeof(char) + sizeof(size_t), SEEK_SET);
                exclusionToFile_(resultFile, ifThisInverted ? thisFile : otherFile, ifThisInverted ? otherFile : thisFile);
            });
        } else {
            // Selections got same invertations.
            if (isInverted_) {
                // We will find intersection.
                result.invert();
                // Result is inverted.
                inFiles3_("rb", other, "rb", result, "ab", [](std::FILE *thisFile, std::FILE *otherFile, std::FILE *resultFile){
                    std::fseek(thisFile, sizeof(char) + sizeof(size_t), SEEK_SET);
                    std::fseek(otherFile, sizeof(char) + sizeof(size_t), SEEK_SET);
                    intersectionToFile_(resultFile, thisFile, otherFile);
                });
            } else {
                // We will find merger.
                // Result is uninverted.
                inFiles3_("rb", other, "rb", result, "ab", [](std::FILE *thisFile, std::FILE *otherFile, std::FILE *resultFile){
                    std::fseek(thisFile, sizeof(char) + sizeof(size_t), SEEK_SET);
                    std::fseek(otherFile, sizeof(char) + sizeof(size_t), SEEK_SET);
                    mergerToFile_(resultFile, thisFile, otherFile);
                });
            }
        }
        return result;
    }

    Selection Selection::operator!() {
        Selection result(*this);
        result.invert();
        return result;
    }


    void Selection::intersect(Selection &other) {
        Selection result = *this && other;
        swap(*this, result);
    }

    void Selection::join(Selection &other) {
        Selection result = *this || other;
        swap(*this, result);
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


    size_t Selection::tIndex_ = 0;


    Selection::Selection() : isInverted_(false), size_(0), name_("selection_" + std::to_string(tIndex_++) + ".tmp." + FILE_EXTENSION) {
        inFile_("wb", [this](std::FILE *file){
            char c = isInverted_ ? '1' : '0';
            std::fwrite(&c, sizeof(char), 1, file);
            std::fwrite(&size_, sizeof(size_t), 1, file);
        });
    }


    Selection::Selection(Selection& other)  : isInverted_(other.isInverted_), size_(other.size_), name_("selection_" + std::to_string(tIndex_++) + ".tmp." + FILE_EXTENSION) {
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


    void Selection::removeFile_() {
        if (std::remove(name_.c_str()) != 0) {
            std::cerr << "Error: Unable to remove " << name_ << " with size of ~" << (size_ * 5 / 1000) << "MB" << std::endl;
        }
    }


    void Selection::updateMeta_() {
        inFile_("ab", [this](std::FILE *file){
            std::fseek(file, 0, SEEK_SET); // Moving to the beginning of file
            char c = isInverted_ ? '1' : '0';
            std::fwrite(&c, sizeof(char), 1, file);
            std::fwrite(&size_, sizeof(size_t), 1, file);
        });
    }










    void swap(Selection &l, Selection &r) {
        bool i = l.isInverted_;
        size_t s = l.size_;
        Selection::Filename n = l.name_;

        l.isInverted_ = r.isInverted_;
        l.size_ = r.size_;
        l.name_ = r.name_;

        r.isInverted_ = i;
        r.size_ = s;
        r.name_ = n;
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

        Selection result;
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

        Selection result;

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
