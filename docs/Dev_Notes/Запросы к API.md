# Запросы к API
Для обращения к API используются POST *(можно и GET, если не более 2 КБ данных)* запросы вида  
`https://api.vk.com/method/{$METHOD_NAME}?{$PARAMETERS}&access_token={$ACCESS_TOKEN}&v={$V}`

Параметры запроса:
* *METHOD_NAME* - метод API
* *PARAMETERS* - параметры в виде `name1=value1&name2=value2`
* *ACCESS_TOKEN* - ключ доступа
* *V* - версия API (по умолчанию 3.0)
