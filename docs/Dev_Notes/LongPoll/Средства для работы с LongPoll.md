# Средства для работы с LongPoll.md

Находятся в `namespace longpoll`

Для работы с LongPoll используется класс `longpoll::session`  
Данный класс содержит информацию о сервере, ключе сервера, timestamp последнего запроса, а также параметры работы с LongPoll (*MODE, TIMEOUT*)

Конструктор принимает `uint8_t mode` - битовую маску режима работы (0 по умолчанию), а также `unsigned short timeout` - таймаут запроса в секундах, не превышающий 90 (25 по умолчанию)

После создания объекта `session` требуется вызвать `session.initialize(accessToken)`, для загрузки сервера, ключа и timestamp.  
После этого можно отправлять запросы с помощью методов `listen_once`, `listen_n`,  `listen_while` и `listen`.

Методам `listen` должна передаваться функция `bool f(json)`, обрабатывающая массив событий (возможно, пустой). Если данная функция вернёт *false*, выполнение `listen` будет завершено досрочно.
