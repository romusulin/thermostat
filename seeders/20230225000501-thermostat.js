'use strict';

module.exports = {
	async up(queryInterface, Sequelize) {
		await queryInterface.bulkInsert('thermostat', [{
			name: 'dom',
			hours: JSON.stringify([0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0]),
			createdAt: new Date(),
			updatedAt: new Date()
		}]);
	},

	async down(queryInterface, Sequelize) {
		return queryInterface.bulkDelete('thermostat', null, {});
	}
};
