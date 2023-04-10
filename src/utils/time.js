"use strict";
Object.defineProperty(exports, "__esModule", { value: true });
exports.getLocalHour = exports.getLocalUnixTimestamp = void 0;
const luxon_1 = require("luxon");
function getLocalUnixTimestamp() {
    const localDate = luxon_1.DateTime.now().setZone('Europe/Zagreb');
    // Fake the constructed date so that we can pass the unix timestamp in local timezone
    const hackedDate = luxon_1.DateTime.fromObject({
        year: localDate.year,
        month: localDate.month,
        day: localDate.day,
        hour: localDate.hour,
        minute: localDate.minute,
        second: localDate.second
    }, {
        zone: 'utc'
    });
    return hackedDate.toUnixInteger();
}
exports.getLocalUnixTimestamp = getLocalUnixTimestamp;
function getLocalHour() {
    return luxon_1.DateTime.now().setZone('Europe/Zagreb').hour;
}
exports.getLocalHour = getLocalHour;
//# sourceMappingURL=time.js.map