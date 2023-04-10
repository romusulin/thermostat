import {Thermostat} from "../models";
import {getLocalUnixTimestamp} from "../utils/time";
import {Request, Response} from "express";

export async function getHours(req: Request, res: Response) {
    const thermostat = await Thermostat.findOne({
        where: { name: 'dom' }
    });

    if (!thermostat) {
        throw new Error('Thermostat not found.');
    }

    const hours = thermostat.hours.join('');
    const unixTimestamp = getLocalUnixTimestamp();

    return res.json({
        hours,
        unixTimestamp
    });
}
