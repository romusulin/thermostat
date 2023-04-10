// @ts-nocheck

import { Model } from 'sequelize';

export class Thermostat extends Model {
  name: string;
  hours: number[];
  static associate(models) {
    // define association here
  }
}

export const init = (sequelize, DataTypes) => {
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