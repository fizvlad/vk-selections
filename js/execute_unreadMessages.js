var unread = 1;
var preview_length = 0;
var count = 200;
var offset = 0;
var requestsLimit = 25;
var result = [];

var first = API.messages.getDialogs({
    "unread": unread,
    "preview_length": preview_length,
    "count": count,
    "offset": offset
});
result.push(first.items);
var total = first.count;
var current = first.items.length;

var i = 1;
while (i * count < total && i < requestsLimit) {
    var r = API.messages.getDialogs({
        "unread": unread,
        "preview_length": preview_length,
        "count": count,
        "offset": offset + i * count
    });
    result.push(r.items);
    current = current + r.items.length;
    i = i + 1;
}
return {"total": total, "current": current, "items": result};
