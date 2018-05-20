# vk-selections
A C++ tool to create selectios of vk.com users and groups using curl


## Dependencies
[libcurl](https://curl.haxx.se/libcurl/)


## Installing with cmake
1. Execute `cmake /path/to/sources` in folder where you want to build the app
2. Execute `make`

You will be able to launch bot with `./vk-selections` **after creating configuration file**


## Configuring
Bot configuration is stored in file `bot.config` in the same folder with following structure:

`key1 value1`  
`key2 value2`  
`key3 value3`

Required configurations:
* `group_token {TOKEN}` - token of main group used in most of requests
* `service_token {TOKEN}` - token of app (used in some requests)
* `group_id {ID}` - id of main group
* `master_id {ID}` - id of bot owner
* `max_units_to_display {INT}` - amount of units to display in dialogue
* `display_mode {0|1|2}` - how to display ids in dialougue (*0* - type and id, *1* - url, *2* - `@{TYPE_PREFIX}{ID}`)
* `change_online {0|1}` - whether change bot online status on start and end of bot work
