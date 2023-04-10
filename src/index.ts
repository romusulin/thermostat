import express from 'express';
import * as eta from 'eta';
import {
	getHomePage,
	saveHours
} from "./controllers/thermostat-controller";
import {
	getHours
} from "./controllers/api-controller";
import path from "path";
import bodyParser from "body-parser";

const PORT = process.env.PORT;
const app = express();
const viewsPath = path.join(__dirname, 'views');

app.engine('eta', eta.renderFile);
eta.configure({ views: viewsPath, cache: true});
app.set('views', viewsPath);
app.set('view cache', true);
app.set('view engine', 'eta');
app.use(bodyParser.urlencoded({ extended: true}));
app.use(bodyParser.json());

app.get('/', getHomePage);
app.post('/hours', saveHours);

app.get('/api/hours', getHours);

app.listen(PORT, () => {
	console.log(`App listening on port ${PORT}`);
});
