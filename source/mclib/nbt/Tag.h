#pragma once

#include "../mclib.h"
#include "../common/Types.h"

#include <memory>
#include <vector>
#include <iosfwd>
#include <algorithm>

namespace mc {

    class DataBuffer;

    namespace nbt {

        enum class TagType { End, Byte, Short, Int, Long, Float, Double, ByteArray, String, List, Compound, IntArray };

        std::string to_string(mc::nbt::TagType type);
        std::ostream& operator<<(std::ostream& os, TagType type);

        class TagList;
        class TagCompound;

        class Tag {
        protected:
            std::wstring m_Name;

            virtual void Write(DataBuffer& buffer) const = 0;
            virtual void Read(DataBuffer& buffer) = 0;

        public:
            Tag(const std::string& name) : m_Name(name.begin(), name.end()) { }
             Tag(const std::wstring& name) : m_Name(name) { }

             std::wstring GetName() const noexcept;
            void  SetName(const std::wstring& name) { m_Name = name; }

            virtual TagType  GetType() const = 0;

            friend  DataBuffer& operator<<(DataBuffer& out, const Tag& tag);
            friend  DataBuffer& operator>>(DataBuffer& in, Tag& tag);

            friend class TagList;
            friend class TagCompound;
        };

        typedef std::shared_ptr<Tag> TagPtr;

        class TagString : public Tag {
        private:
            std::wstring m_Value;

            void  Write(DataBuffer& buffer) const;
            void  Read(DataBuffer& buffer);

        public:
             TagString() : Tag(L"") { }
             TagString(std::wstring name, std::wstring val) : Tag(name), m_Value(val) { }
             TagString(std::string name, std::string val) : Tag(name), m_Value(val.begin(), val.end()) { }

            TagType  GetType() const noexcept;

            std::wstring  GetValue() const noexcept { return m_Value; }
            friend  DataBuffer& operator<<(DataBuffer& out, const Tag& tag);
        };

        class TagByteArray : public Tag {
        private:
            std::string m_Value;

            void  Write(DataBuffer& buffer) const;
            void  Read(DataBuffer& buffer);

        public:
             TagByteArray() : Tag(L"") { }
             TagByteArray(std::wstring name, std::string val) : Tag(name), m_Value(val) { }
             TagByteArray(std::string name, std::string val) : Tag(name), m_Value(val) { }

            TagType  GetType() const noexcept;

            std::string  GetValue() const noexcept { return m_Value; }
            friend  DataBuffer& operator<<(DataBuffer& out, const Tag& tag);
        };

        class TagIntArray : public Tag {
        private:
            std::vector<s32> m_Value;

            void  Write(DataBuffer& buffer) const;
            void  Read(DataBuffer& buffer);

        public:
             TagIntArray() : Tag(L"") { }
             TagIntArray(std::wstring name, std::vector<s32> val) : Tag(name), m_Value(val) { }
             TagIntArray(std::string name, std::vector<s32> val) : Tag(name), m_Value(val) { }

            TagType  GetType() const noexcept;

            std::vector<s32>  GetValue() const noexcept { return m_Value; }
            friend  DataBuffer& operator<<(DataBuffer& out, const Tag& tag);
        };

        class TagList : public Tag {
        private:
            std::vector<TagPtr> m_Tags;
            TagType m_ListType;

            void  Write(DataBuffer& buffer) const;
            void  Read(DataBuffer& buffer);
            void  CopyOther(const TagList& rhs);
        public:
             TagList() : Tag(L""), m_ListType(TagType::End) { }
             TagList(std::wstring name, TagType listType) : Tag(name), m_ListType(listType) { }
             TagList(std::string name, TagType listType) : Tag(name), m_ListType(listType) { }
             ~TagList();

             TagList(const TagList& rhs);
             TagList& operator=(const TagList& rhs);
            TagList(TagList&& rhs) = default;
            TagList& operator=(TagList&& rhs) = default;

            TagType  GetType() const noexcept;
            TagType  GetListType() const noexcept { return m_ListType; }
            const std::vector<TagPtr>& GetList() const { return m_Tags; }
            std::size_t GetSize() const { return m_Tags.size(); }

            std::vector<TagPtr>::iterator  begin() { return m_Tags.begin(); }
            std::vector<TagPtr>::iterator  end() { return m_Tags.end(); }
            std::vector<TagPtr>::const_iterator  begin() const { return m_Tags.begin(); }
            std::vector<TagPtr>::const_iterator  end() const { return m_Tags.end(); }

            void  AddItem(TagPtr item);
            friend  DataBuffer& operator<<(DataBuffer& out, const TagList& tag);
            friend  DataBuffer& operator<<(DataBuffer& out, const Tag& tag);
        };

        class TagCompound : public Tag {
        public:
            using DataType = std::pair<TagType, TagPtr>;

        private:
            std::vector<DataType> m_Tags;

            void  Write(DataBuffer& buffer) const;
            void  Read(DataBuffer& buffer);

            void CopyOther(const TagCompound& rhs);
        public:
             TagCompound() : Tag(L"") { }
             TagCompound(const std::wstring& name) : Tag(name) { }
             TagCompound(const std::string& name) : Tag(name) { }
             ~TagCompound();

             TagCompound(const TagCompound& rhs);
             TagCompound& operator=(const TagCompound& rhs);
            TagCompound(TagCompound&& rhs) = default;
            TagCompound& operator=(TagCompound&& rhs) = default;

            TagType  GetType() const noexcept;

            void  AddItem(TagType type, TagPtr item);

            std::vector<DataType>::iterator  begin() { return m_Tags.begin(); }
            std::vector<DataType>::iterator  end() { return m_Tags.end(); }
            std::vector<DataType>::const_iterator  begin() const { return m_Tags.begin(); }
            std::vector<DataType>::const_iterator  end() const { return m_Tags.end(); }

            template <typename T>
            T* GetTag(const std::wstring& tagName) const {
                auto iter = std::find_if(m_Tags.begin(), m_Tags.end(), [&](DataType entry) {
                    return entry.second->GetName() == tagName;
                });

                if (iter == m_Tags.end()) return nullptr;

                return dynamic_cast<T*>(iter->second.get());
            }

            friend  DataBuffer& operator<<(DataBuffer& out, const TagCompound& tag);
            friend  DataBuffer& operator<<(DataBuffer& out, const Tag& tag);
        };

        class TagByte : public Tag {
        private:
            u8 m_Value;

            void  Write(DataBuffer& buffer) const;
            void  Read(DataBuffer& buffer);

        public:
             TagByte() : Tag(L""), m_Value(0) { }
             TagByte(std::wstring name, u8 value) : Tag(name), m_Value(value) { }
             TagByte(std::string name, u8 value) : Tag(name), m_Value(value) { }
            TagType  GetType() const noexcept;

            u8  GetValue() const noexcept { return m_Value; }
            friend  DataBuffer& operator<<(DataBuffer& out, const Tag& tag);
        };

        class TagShort : public Tag {
        private:
            s16 m_Value;

            void  Write(DataBuffer& buffer) const;
            void  Read(DataBuffer& buffer);

        public:
             TagShort() : Tag(L""), m_Value(0) { }
             TagShort(std::wstring name, s16 value) : Tag(name), m_Value(value) { }
             TagShort(std::string name, s16 value) : Tag(name), m_Value(value) { }
            TagType  GetType() const noexcept;

            s16  GetValue() const noexcept { return m_Value; }
            friend  DataBuffer& operator<<(DataBuffer& out, const Tag& tag);
        };

        class TagInt : public Tag {
        private:
            s32 m_Value;

            void  Write(DataBuffer& buffer) const;
            void  Read(DataBuffer& buffer);

        public:
             TagInt() : Tag(L""), m_Value(0) { }
             TagInt(std::wstring name, s32 value) : Tag(name), m_Value(value) { }
             TagInt(std::string name, s32 value) : Tag(name), m_Value(value) { }
            TagType  GetType() const noexcept;

            s32  GetValue() const noexcept { return m_Value; }
            friend  DataBuffer& operator<<(DataBuffer& out, const Tag& tag);
        };

        class TagLong : public Tag {
        private:
            s64 m_Value;

            void  Write(DataBuffer& buffer) const;
            void  Read(DataBuffer& buffer);

        public:
             TagLong() : Tag(L""), m_Value(0) { }
             TagLong(std::wstring name, s64 value) : Tag(name), m_Value(value) { }
             TagLong(std::string name, s64 value) : Tag(name), m_Value(value) { }
            TagType  GetType() const noexcept;

            s64  GetValue() const noexcept { return m_Value; }
            friend  DataBuffer& operator<<(DataBuffer& out, const Tag& tag);
        };

        class TagFloat : public Tag {
        private:
            float m_Value;

            void  Write(DataBuffer& buffer) const;
            void  Read(DataBuffer& buffer);

        public:
             TagFloat() : Tag(L""), m_Value(0.0f) { }
             TagFloat(std::wstring name, float value) : Tag(name), m_Value(value) { }
             TagFloat(std::string name, float value) : Tag(name), m_Value(value) { }
            TagType  GetType() const noexcept;

            float  GetValue() const noexcept { return m_Value; }
            friend  DataBuffer& operator<<(DataBuffer& out, const Tag& tag);
        };

        class TagDouble : public Tag {
        private:
            double m_Value;

            void  Write(DataBuffer& buffer) const;
            void  Read(DataBuffer& buffer);

        public:
             TagDouble() : Tag(L""), m_Value(0.0) { }
             TagDouble(std::wstring name, double value) : Tag(name), m_Value(value) { }
             TagDouble(std::string name, double value) : Tag(name), m_Value(value) { }
            TagType  GetType() const noexcept;

            double  GetValue() const noexcept { return m_Value; }
            friend  DataBuffer& operator<<(DataBuffer& out, const Tag& tag);
        };

         DataBuffer& operator<<(DataBuffer& out, const Tag& tag);
         DataBuffer& operator<<(DataBuffer& out, const TagString& tag);
         DataBuffer& operator<<(DataBuffer& out, const TagByteArray& tag);
         DataBuffer& operator<<(DataBuffer& out, const TagList& tag);
         DataBuffer& operator<<(DataBuffer& out, const TagCompound& tag);
         DataBuffer& operator<<(DataBuffer& out, const TagIntArray& tag);
         DataBuffer& operator<<(DataBuffer& out, const TagByte& tag);
         DataBuffer& operator<<(DataBuffer& out, const TagShort& tag);
         DataBuffer& operator<<(DataBuffer& out, const TagInt& tag);
         DataBuffer& operator<<(DataBuffer& out, const TagLong& tag);
         DataBuffer& operator<<(DataBuffer& out, const TagFloat& tag);
         DataBuffer& operator<<(DataBuffer& out, const TagDouble& tag);

         DataBuffer& operator>>(DataBuffer& in, Tag& tag);
         DataBuffer& operator>>(DataBuffer& in, TagString& tag);
         DataBuffer& operator>>(DataBuffer& in, TagByteArray& tag);
         DataBuffer& operator>>(DataBuffer& in, TagList& tag);
         DataBuffer& operator>>(DataBuffer& in, TagCompound& tag);
         DataBuffer& operator>>(DataBuffer& in, TagIntArray& tag);
         DataBuffer& operator>>(DataBuffer& in, TagByte& tag);
         DataBuffer& operator>>(DataBuffer& in, TagShort& tag);
         DataBuffer& operator>>(DataBuffer& in, TagInt& tag);
         DataBuffer& operator>>(DataBuffer& in, TagLong& tag);
         DataBuffer& operator>>(DataBuffer& in, TagFloat& tag);
         DataBuffer& operator>>(DataBuffer& in, TagDouble& tag);

    } // ns nbt
} // ns mc
