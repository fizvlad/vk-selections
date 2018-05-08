# Библиотека для работы с json в C++

## Описание
[Используемая библиотека - *nlohmann/json*](https://github.com/nlohmann/json)  
Данная header-only библиотека является удобным инструментом для работы с Web и обладает лицензией MIT, что позволяет свободно использовать её даже в коммерческих целях

## Установка
Достаточно подключить с помощью `#include`.  
Однако, следует учитывать, что для библиотеки требуется поддержка C++11.  
Также, следует помнить, что [не все версии компиляторов поддерживают](https://github.com/nlohmann/json#supported-compilers) данную библиотеку.

## Работа с библиотекой

### Создание нового объекта
``` C++
    json j = {
        {"int_value", 1},
        {"bool_value", false},
        {"str", "Niels"},
        {"null_object", nullptr},
        {"object_in_object", {
            {"answer_to_everything", 42}
        }},
        {"array", {1, 0, 2}},
        {"another_object", {
            {"currency", "USD"},
            {"value", 42.99}
        }}
    };
```

### Доступ к полям
`j["object_in_object"]["answer_to_everything"] = 2`

### Про автоопределение типа
В основном, тип указывать не приходится, однако, если требуется, можно использовать конструкции:
``` C++
    // Пустой массив
    json empty_array = json::array();

    // Пустой объект
    json empty_object1 = json({});
    json empty_object2 = json::object();

    // Массив пар
    json array_not_object = json::array({ {"yes", true}, {"no", 0} });
```

### json ⇄ string
Для создание json из строки можно использовать [строковые литералы](https://msdn.microsoft.com/ru-ru/library/69ze775t.aspx):
``` C++
    json j = "{ \"happy\": true, \"pi\": 3.141 }"_json;

    // Или проще
    auto j2 = R"(
        {
            "happy": true,
            "pi": 3.141
        }
    )"_json;
```
**Важно не забывать про литерал *_json***

Конвертация с помощью функции `parse` и метода `dump`:
``` C++
    auto j = json::parse("{ \"bool\": true, \"num\": 3.141 }");
    std::string s = j.dump();
```

### istream, ostream
Библиотека полностью поддерживает работу с потоками, что позволяет, например, получать объект из *.json* файла.

### STL
Библиотека поддерживает создание из STL контейнеров, а также многие методы и функции.
