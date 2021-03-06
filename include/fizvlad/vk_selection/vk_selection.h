#ifndef VK_SELECTION_H_INCLUDED
#define VK_SELECTION_H_INCLUDED

#include "fizvlad/vk_api/vk_utility.h"


namespace fizvlad {namespace vk_selection {

    const std::string FILE_EXTENSION = "vks";

    /// Pseudonym for unsigned long. Id of user or group.
    typedef unsigned long UnitId;

    /// Pseudonym for string. Custom id of user or group.
    typedef std::string UnitCustomId;

    /// Type of Unit.
    enum UnitType : char {Undefined = '0', User, Public, Group, Event};
    /// Unit types.
    const std::vector<std::string> unitTypeNames = {"Undefined", "User", "Public", "Group", "Event"};
    std::string unitTypeName(UnitType type);
    /// Unit url prefixes.
    const std::vector<std::string> unitTypePrefixes = {"", "id", "public", "club", "event"};
    std::string unitTypePrefix(UnitType type);


    /// Class to work with selection of Units.
    /// Each Selection have its own file in "${name_}.vks" which contains ids.
    class Selection {
    public:

        Selection();

        Selection(const Selection& other);
        Selection&operator=(const Selection& other);

        Selection(Selection &&other);
        Selection&operator=(Selection &&other);

        ~Selection();


        Selection operator&&(const Selection &other) const;
        Selection operator||(const Selection &other) const;
        Selection operator!() const;


        void intersect(Selection &other);
        void join(Selection &other);
        void invert();


        /// TRUE if selection is inverted.
        bool isInverted() const;


        /// Size of selection.
        size_t size() const;


        /// Pseudonym for string. Used as file name
        typedef std::string Filename;


        /// Save to file "${name}.txt".
        void saveAs(Filename name) const;


        /// Apply function to each unit in selection
        template <typename F>
        void forEach(F func) {
            // TODO Correct work with function from template
            std::FILE *file = std::fopen(name_.c_str(), "rb");
            std::fseek(file, sizeof(char) + sizeof(size_t), SEEK_SET);
            for (size_t i = 0; i < size_; i++) {
                UnitType type = (UnitType) std::fgetc(file);
                UnitId id;
                std::fread(&id, sizeof(id), 1, file);
                func(type, id);
            }
            std::fclose(file);
        }


    private:

        friend class Unit;

        /// Static variables which is incremented on every creation of Selection.
        /// Used to create files with unique names.
        static size_t tIndex_;


        bool     isInverted_;
        size_t   size_;
        Filename name_;


        void removeFile_();


        template <typename F>
        void inFile_(const char* mode, F func) const {
            // TODO Correct work with function from template
            std::FILE *file = std::fopen(name_.c_str(), mode);
            func(file);
            std::fclose(file);
        }


        template <typename F>
        static void inFiles2_(const vk_selection::Selection &selection1, const char* mode1, const vk_selection::Selection &selection2, const char* mode2, F func) {
            // TODO Correct work with function from template
            std::FILE *file1 = std::fopen(selection1.name_.c_str(), mode1);
            std::FILE *file2 = std::fopen(selection2.name_.c_str(), mode2);
            func(file1, file2);
            std::fclose(file1);
            std::fclose(file2);
        }


        template <typename F>
        void inFiles3_(const char* mode, const vk_selection::Selection &selection1, const char* mode1, const vk_selection::Selection &selection2, const char* mode2, F func) const {
            // TODO Correct work with function from template
            std::FILE *file = std::fopen(name_.c_str(), mode);
            std::FILE *file1 = std::fopen(selection1.name_.c_str(), mode1);
            std::FILE *file2 = std::fopen(selection2.name_.c_str(), mode2);
            func(file, file1, file2);
            std::fclose(file);
            std::fclose(file1);
            std::fclose(file2);
        }


        void updateMeta_();


        friend void swap(Selection &l, Selection &r);
    };

    void swap(Selection &l, Selection &r);

    /// Print Selection info.
    std::ostream &operator<<(std::ostream &os, const Selection &selection);


    /// Class to work with single User, Group, Public or Event.
    class Unit {
    public:

        /// Create using id.
        Unit(UnitId id, bool isUser, vk_api::Token token);

        /// Create using custom id.
        /// Automatically identifies Unit type.
        Unit(UnitCustomId id, vk_api::Token token);


        Unit &operator=(const Unit&) = default;
        Unit(const Unit&) = default;
        Unit &operator=(Unit&&) = default;
        Unit(Unit&&) = default;
        ~Unit() = default;


        /// Unit type.
        UnitType type() const;


        /// Unit id.
        UnitId id() const;


        /// Unit custom id.
        UnitCustomId customId() const;


        /// Selection of user friends (10000 max).
        Selection friends(vk_api::Token token) const;


        /// Selection of user subscribers.
        /// Notice: Requires user token.
        Selection subscribers(vk_api::Token token) const;


        /// Selection of group/public/event members.
        Selection members(vk_api::Token token) const;


    private:

        friend class Selection;

        UnitType type_;
        UnitId id_;
        UnitCustomId customId_;

        Unit(UnitType type = Undefined, UnitId id = 0, UnitCustomId customId = "");


        void initUser_(UnitId id, vk_api::Token token);

        void initGroup_(UnitId id, vk_api::Token token);

    };


    /// Print type, id and custom id.
    std::ostream &operator<<(std::ostream &os, const Unit &unit);

    bool operator<(const Unit &l, const Unit &r);

}}

#endif // VK_SELECTION_H_INCLUDED
