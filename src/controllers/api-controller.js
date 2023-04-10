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
exports.getHours = void 0;
const models_1 = require("../models");
const time_1 = require("../utils/time");
function getHours(req, res) {
    return __awaiter(this, void 0, void 0, function* () {
        const thermostat = yield models_1.Thermostat.findOne({
            where: { name: 'dom' }
        });
        if (!thermostat) {
            throw new Error('Thermostat not found.');
        }
        const hours = thermostat.hours.join('');
        const unixTimestamp = (0, time_1.getLocalUnixTimestamp)();
        return res.json({
            hours,
            unixTimestamp
        });
    });
}
exports.getHours = getHours;
//# sourceMappingURL=api-controller.js.map