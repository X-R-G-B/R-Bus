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
nlohmann::json Json::getDataByJsonType(JsonType dataType);
```

- Get a data with a key :
```cpp
nlohmann::json Json::getDataByJsonType(const std::string &index, JsonType dataType);
```

- Get a data with vector of key :
```cpp
nlohmann::json Json::getDataByVector(const std::vector<std::string> &indexes, JsonType dataType);
```

- Get a data from a json list :  
This function is useful when you want to take a json list inside your json because all of your items of the list are in the vector.
```cpp
std::vector<nlohmann::json> Json::getDatasByJsonType(const std::vector<std::string> &indexes, JsonType dataType);
```

- Get a data from an existing json data :
```cpp
nlohmann::json &Json::getDataFromJson(nlohmann::json jsonData, const std::string &index);
```

- Get a data but precise the type of what you want with template :
```cpp
template <typename T>
T Json::getDataFromJson(nlohmann::json jsonData, const std::string &index);
```

- Get a data from a vector of json data :
```cpp
std::vector<nlohmann::json> Json::getDatasFromList(const std::vector<nlohmann::json> &list, const std::string &key);
```

- Get a json object from a jsonType and an id :
```cpp
nlohmann::json Json::getJsonObjectById(JsonType type, const std::string &id)
```

- Get a json list from a json data :
```cpp
std::vector<nlohmann::json> Json::getDatasFromList(const nlohmann::json &list, const std::string &key);
```

- Get a json list from a json data :
```cpp
std::vector<nlohmann::json> Json::getDatasFromList(const nlohmann::json &list);
```

- Check if the data exists in the json :
```cpp
bool Json::isDataExist(nlohmann::json jsonData, const std::string &index);
```

## Errors handling
All the methods above Log an error with the class [Logger][logger] and throw an std::runtime_error if the arguments are incorect or the key in the json is not found because if the data is not get correctly the rest of rest programm might crash.

Here's a json with spritePath of enemy missing :
```json
{
    "enemy" : {
    }
}
```

Here's a code that will try to acces it :
```cpp
Json::getInstance().getDataByVector({"enemy", "spritePath"},  
    JsonType::DEFAULT_ENEMY)
```

This won't work and output this :
```
2023-10-15 13:29:57.813141624 [FATAL] (getDataByVector) Key : spritePath is not valid
```

However you can handle you proper way the errors with the method isDataExist listed above.
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
std::vector<nlohmann::json> enemyData =  
    Json::getInstance().getDataByJsonType("enemy", enemyType);
```
And now you can iterate on your datas :
```cpp
for (auto &data : enemyData) {
    std::cout << Json::getInstance().getDataFromJson(elem, "spritePath") <<  
        std::endl;
}
```

### Get a json object from a jsonType and an id
Here's a json file with an array of enemy objects :
```json
{
    "enemy" : [
        {
            "id" : "1",
            "spritePath" : "path"
        },
        {
            "id" : "2",
            "spritePath" : "path"
        }
    ]
}
```

Here's how to get an enemy object from his id :
```cpp
    nlohmann::json object = Json::getJsonObjectById(enemyType, "1");
    //the returned object will be :
    // {
    //     "id" : "1",
    //     "spritePath" : "path"
    // }
```
