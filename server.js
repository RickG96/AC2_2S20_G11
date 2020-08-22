const express = require('express');
const cors = require('cors');

var app = express();
app.use(cors());

var objeto = {
    load: false,
    weight: 0,
    waterLevel: 0
}

app.post('/setstats', (req, res) => {
    console.log(req.body);
    res.status(200);
})

app.get('/getstats', (req, res) => {
    res.json(objeto);
});


app.listen(3000, (err) => {
    if (err) console.log('Ocurrio un error'), process.exit(1);

    console.log('Escuchando en el puerto 3000');
})
