const express = require('express');
const cors = require('cors');

var app = express();
app.use(cors());

var objeto = {
    load: true,
    weight: 5,
    waterLevel: 3
}

app.get('/getstats', (req, res) => {
    res.json(objeto);
});


app.listen(3000, (err) => {
    if (err) console.log('Ocurrio un error'), process.exit(1);

    console.log('Escuchando en el puerto 3000');
})
