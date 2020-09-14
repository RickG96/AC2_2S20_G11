const express = require('express');
const cors = require('cors');
const bodyParser = require('body-parser');

var app = express();
app.use(cors());
app.use(bodyParser.json());

var objeto = {
    load: false,
    weight: 0,
    waterLevel: 0
}

app.post('/setstats', (req, res) => {
    console.log(req.body);
    objeto.load = req.body.load;
    objeto.weight = req.body.weight;
    objeto.waterLevel = req.body.waterLevel;
    res.status(200);
})

app.get('/getstats', (req, res) => {
    res.json(objeto);
});


app.listen(3000, (err) => {
    if (err) console.log('Ocurrio un error'), process.exit(1);

    console.log('Escuchando en el puerto 3000');
})
