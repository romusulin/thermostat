import { DateTime } from 'luxon';

export function getLocalUnixTimestamp() {
    const localDate = DateTime.now().setZone('Europe/Zagreb');

    // Fake the constructed date so that we can pass the unix timestamp in local timezone
    const hackedDate = DateTime.fromObject({
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

export function getLocalHour() {
    return DateTime.now().setZone('Europe/Zagreb').hour;
}
