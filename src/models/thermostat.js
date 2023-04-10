"use strict";
// @ts-nocheck
Object.defineProperty(exports, "__esModule", { value: true });
exports.init = exports.Thermostat = void 0;
const sequelize_1 = require("sequelize");
class Thermostat extends sequelize_1.Model {
    static associate(models) {
        // define association here
    }
}
exports.Thermostat = Thermostat;
const init = (sequelize, DataTypes) => {
    Thermostat.init({
        id: {
            type: DataTypes.NUMERIC,
            autoIncrement: true,
            primaryKey: true
        },
        name: DataTypes.STRING,
        hours: {
            type: DataTypes.JSON,
            allowNull: false
        }
    }, {
        sequelize,
        modelName: 'thermostat',
        freezeTableName: true
    });
    return Thermostat;
};
exports.init = init;
//# sourceMappingURL=thermostat.js.map