const express = require('express');
const cors = require('cors');
const bodyParser = require('body-parser');

var app = express();
app.use(cors());
app.use(bodyParser.json());


app.post('/setubicacion', (req, res) => {
    
})

app.post('/sethora', (req, res) => {
    
})

app.post('/setpeso', (req, res) => {
    
})

app.post('/set', (req, res) => {
    
})

app.get('/getstats', (req, res) => {
    res.json(objeto);
});


app.listen(3000, (err) => {
    if (err) console.log('Ocurrio un error'), process.exit(1);

    console.log('Escuchando en el puerto 3000');
})
