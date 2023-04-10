"use strict";
var __awaiter = (this && this.__awaiter) || function (thisArg, _arguments, P, generator) {
    function adopt(value) { return value instanceof P ? value : new P(function (resolve) { resolve(value); }); }
    return new (P || (P = Promise))(function (resolve, reject) {
        function fulfilled(value) { try { step(generator.next(value)); } catch (e) { reject(e); } }
        function rejected(value) { try { step(generator["throw"](value)); } catch (e) { reject(e); } }
        function step(result) { result.done ? resolve(result.value) : adopt(result.value).then(fulfilled, rejected); }
        step((generator = generator.apply(thisArg, _arguments || [])).next());
    });
};
Object.defineProperty(exports, "__esModule", { value: true });
exports.saveHours = exports.getHomePage = void 0;
const models_1 = require("../models");
const time_1 = require("../utils/time");
function getHomePage(req, res) {
    return __awaiter(this, void 0, void 0, function* () {
        const thermostat = yield models_1.Thermostat.findOne({
            where: { name: 'dom' }
        });
        const currentHour = (0, time_1.getLocalHour)();
        res.render('index.eta', {
            hours: thermostat === null || thermostat === void 0 ? void 0 : thermostat.hours.map(h => !!h ? 'checked' : ''),
            status: !!(thermostat === null || thermostat === void 0 ? void 0 : thermostat.hours[currentHour]) ? 'upaljena' : 'izgasena'
        });
    });
}
exports.getHomePage = getHomePage;
function saveHours(req, res) {
    return __awaiter(this, void 0, void 0, function* () {
        const payload = req.body;
        const hours = Array(24).fill(0);
        for (const [hourString, value] of Object.entries(payload)) {
            const hourIndex = Number(hourString);
            hours[hourIndex] = value === 'on' ? 1 : 0;
        }
        yield models_1.Thermostat.update({
            hours
        }, {
            where: { name: 'dom' }
        });
        res.redirect('/');
    });
}
exports.saveHours = saveHours;
//# sourceMappingURL=thermostat-controller.js.map