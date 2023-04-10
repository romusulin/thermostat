import { Request, Response } from "express";
import { Thermostat } from "../models";
import { getLocalHour } from "../utils/time";

export async function getHomePage(req: Request, res: Response) {
	const thermostat = await Thermostat.findOne({
		where: { name: 'dom' }
	});

	const currentHour = getLocalHour();

	res.render('index.eta', {
		hours: thermostat?.hours.map(h => !!h ? 'checked' : ''),
		status: !!thermostat?.hours[currentHour] ? 'upaljena' : 'izgasena'
	});
}

export async function saveHours(req: Request, res: Response) {
	const payload: {[key: string]: 'on' } = req.body;
	const hours = Array(24).fill(0);
	for (const [hourString, value] of Object.entries(payload)) {
		const hourIndex = Number(hourString);
		hours[hourIndex] = value === 'on' ? 1 : 0;
	}

	await Thermostat.update({
		hours
	}, {
		where: { name: 'dom' }
	})


	res.redirect('/');
}
