/*
    Script to make string to use in VK API 'execute' method

    Launch with command:
    node stringify.js ${TARGET_NAME_1} ${TARGET_NAME_2} ...

    Tested on NodeJS 10.0.0
*/

const COMMANDS = ["var", "while", "return"];

function stringify (str) {
    let result = str.replace(/[\s]/g, function(s, p) {
        for (i in COMMANDS) {
            command = COMMANDS[i];
            if (str.slice(p - command.length, p) == COMMANDS[i]) {
                return " ";
            }
        }
        return "";
    });
    result = result.replace(/"/g, "\\\"")
    return result;
}

let fs = require('fs');
let path = require('path');

for (let i = 2; i < process.argv.length; i++) {
    let filePath = path.join(path.resolve(), process.argv[i]);
    fs.readFile(filePath, {"encoding": "utf-8"}, function (err, data) {
        if (!err) {
            let name = process.argv[i].replace(/\.[^\.]+$/, (s) => (s + ".txt"));
            fs.writeFile(name, "\"" + stringify(data) + "\"", (err) => (err ? console.log(err) : null));
        } else {
            console.log(err);
            console.log(process.argv[i]);
        }
    });
}
