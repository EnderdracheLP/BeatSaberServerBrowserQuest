#pragma once
#include "beatsaber-hook/shared/config/rapidjson-utils.hpp"
#include "Exceptions.hpp"

#define DECLARE_JSON_CLASS(namespaze, name, impl) \
namespace namespaze { \
    class name { \
    public: \
        void Deserialize(const rapidjson::Value& jsonValue); \
    impl \
    }; \
} \

#define GETTER_VALUE(type, name) \
private: \
    type name; \
public: \
    const type& Get##name() const { return name; }

#define GETTER_VALUE_OPTIONAL(type, name) \
private: \
    std::optional<type> name; \
public: \
    std::optional<type> Get##name() const { return name; }

#define DESERIALIZE_METHOD(namespaze, name, impl) \
void namespaze::name::Deserialize(const rapidjson::Value& jsonValue) { \
    impl \
}

#define DESERIALIZE_VALUE(name, jsonName, type) \
if (!jsonValue.HasMember(#jsonName)) throw ServerBrowser::JsonException(ServerBrowser::Exceptions::NoMember, #jsonName); \
if (!jsonValue[#jsonName].Is##type()) throw ServerBrowser::JsonException(ServerBrowser::Exceptions::WrongType, #jsonName ", Type expected was: " #type); \
name = jsonValue[#jsonName].Get##type();

#define DESERIALIZE_VALUE_2TYPES(name, jsonName, returnType, checkType2) \
if (!jsonValue.HasMember(#jsonName)) throw ServerBrowser::JsonException(ServerBrowser::Exceptions::NoMember, #jsonName); \
if (jsonValue[#jsonName].Is##returnType() || jsonValue[#jsonName].Is##checkType2()) name = jsonValue[#jsonName].Get##returnType(); \
else throw ServerBrowser::JsonException(ServerBrowser::Exceptions::WrongType, #jsonName ", Type expected was: " #returnType "or" #checkType2);


#define DESERIALIZE_VALUE_OPTIONAL(name, jsonName, type) \
if(jsonValue.HasMember(#jsonName) && jsonValue[#jsonName].Is##type()) { \
    name = jsonValue[#jsonName].Get##type(); \
} else { \
    name = std::nullopt; \
}

#define DESERIALIZE_CLASS(name, jsonName) \
if (!jsonValue.HasMember(#jsonName)) throw ServerBrowser::JsonException(ServerBrowser::Exceptions::NoMember, #jsonName); \
if (!jsonValue[#jsonName].IsObject()) throw ServerBrowser::JsonException(ServerBrowser::Exceptions::WrongType, #jsonName ", Type expected was: JsonObject"); \
name.Deserialize(jsonValue[#jsonName]);

#define DESERIALIZE_CLASS_OPTIONAL(name, jsonName, type) \
if(jsonValue.HasMember(#jsonName) && jsonValue[#jsonName].IsObject()) { \
    type val##name; \
    name = val##name;\
    name.value().Deserialize(jsonValue[#jsonName]); \
} else { \
    name = std::nullopt; \
}

#define DESERIALIZE_VECTOR(name, jsonName, type) \
if (!jsonValue.HasMember(#jsonName)) throw ServerBrowser::JsonException(ServerBrowser::Exceptions::NoMember, #jsonName); \
name.clear(); \
auto& jsonName = jsonValue[#jsonName]; \
if(jsonName.IsArray()) { \
    for (auto it = jsonName.Begin(); it != jsonName.End(); ++it) { \
        type value; \
        value.Deserialize(*it); \
        name.push_back(value); \
    } \
} else throw ServerBrowser::JsonException(ServerBrowser::Exceptions::WrongType, #jsonName ", Type expected was: JsonArray");