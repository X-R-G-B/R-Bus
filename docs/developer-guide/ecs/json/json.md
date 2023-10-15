[logger]: ../logger.md
# Usage

## JsonType available
- DEFAULT_ENEMY,
- DEFAULT_PLAYER,
- DEFAULT_PARALLAX,

## Methods

- Get the instance of the class :
```cpp
Json &Json::getInstance();
```

- Get a data with JsonType :
```cpp
nlohmann::basic_json<> Json::getDataByJsonType(JsonType dataType);
```

- Get a data with a key :
```cpp
nlohmann::basic_json<> Json::getDataByJsonType(const std::string &index, JsonType dataType);
```

- Get a data with vector of key :
```cpp
nlohmann::basic_json<> getDataByVector(const std::vector<std::string> &indexes, JsonType dataType);
```

- Get a data from a json list :  
This function is useful when you want to take a json list inside your json because all of your items of the list are in the vector.
```cpp
std::vector<nlohmann::basic_json<>> getDatasByJsonType(const std::vector<std::string> &indexes, JsonType dataType);
```

- Get a data from an existing json data :
```cpp
nlohmann::json &getDataFromJson(nlohmann::basic_json<> jsonData, const std::string &index);
```

- Get a data from a vector of json data :
```cpp
std::vector<nlohmann::basic_json<>> getDatasFromList(const std::vector<nlohmann::basic_json<>> &list, const std::string &key);
```

- Get a json list from a json data :
```cpp
std::vector<nlohmann::basic_json<>> getDatasFromList(const nlohmann::basic_json<> &list, const std::string &key);
```

- Get a json list from a json data :
```cpp
std::vector<nlohmann::basic_json<>> getDatasFromList(const nlohmann::basic_json<> &list);
```

## Errors handling
All the methods above Log an error with the class [Logger][logger] and throw an std::runtime_error if the arguments are incorect or the key in the json is not found because if the data is not get correctly the rest of rest programm might crash.

## Some examples

### Basic example
Here's a json :  
```json
{
    "enemy" : {
        "spritePath" : "path"
    }
}
```
Here's how you can get spritePath data :
```cpp
std::cout << Json::getInstance().getDataByVector({"enemy", "spritePath"},  
    JsonType::DEFAULT_ENEMY) << std::endl;
```

### Using list data
Here's a new json but with a list inside :
```json
{
    "enemy" : [
        {
            "spritePath" : "path"
        },
        {
            "spritePath" : "path"
        }
    ]
}
```
You can see that all spritePath are in a list.  
Here's how to proceed :
```cpp
std::vector<nlohmann::basic_json<>> enemyData =  
    Json::getInstance().getDataByJsonType("enemy", enemyType);
```
And now you can iterate on your datas :
```cpp
for (auto &data : enemyData) {
    std::cout << Json::getInstance().getDataFromJson(elem, "spritePath") <<  
        std::endl;
}
```
