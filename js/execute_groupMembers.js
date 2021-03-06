var id = 58219172;
var count = 1000;
var offset = 0;
var requestsLimit = 25;
var result = [];

var first = API.groups.getMembers({
    "group_id": id,
    "count": count,
    "offset": offset
});
result.push(first.items);
var total = first.count;
var current = first.items.length;

var i = 1;
while (i * count < total && i < requestsLimit) {
    var r = API.groups.getMembers({
        "group_id": id,
        "count": count,
        "offset": offset + i * count
    });
    result.push(r.items);
    current = current + r.items.length;
    i = i + 1;
}
return {"total": total, "current": current, "items": result};
