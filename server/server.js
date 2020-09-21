const express = require('express');
const cors = require('cors');
const bodyParser = require('body-parser');

const low = require('lowdb');
const FileSync = require('lowdb/adapters/FileSync');

const adapter = new FileSync('db.json');
const db = low(adapter)

db.defaults({ peso: [], hora_salida: [], hora_llegada: [], t_entrega: [], t_regreso: [], obstaculos: 0 })
    .write();

var app = express();
app.use(cors());
app.use(bodyParser.json());

var actual = -1;
var nsalidas = 0;
var nllegadas = 0;

//Ubicacion-Estado-Peso-NumeroObstaculos
app.post('/setestados', (req, res) => {
    //console.log(new Date().toISOString().replace(/T/, ' ').replace(/\..+/, ''));
    console.log(req.body.valores);

    let datos = req.body.valores.split('-');

    if(datos[0] == 0) {
        if(actual != datos[0]) {
            nsalidas++;
            actual = parseInt(datos[0]);

            db.get('salida')
              .push({ id: nsalidas, hora: new Date().toISOString().replace(/T/, ' ').replace(/\..+/, ''), milis: new Date().getTime() })
              .write();

        }
    } else if(datos[0] == 1) {
        if(actual != datos[0]) {
            actual = parseInt(datos[0]);

            db.get('peso')
              .push({ peso: parseFloat(datos[2]) })
              .write();
        }
    } else if(datos[0] == 2) {
        if(actual != datos[0]) {
            nllegadas++;
            actual = parseInt(datos[0]);

            db.get('llegada')
              .push({ id: nllegadas, hora: new Date().toISOString().replace(/T/, ' ').replace(/\..+/, ''), milis: new Date().getTime() })
              .write();

        }
    }
    

    res.json('ok');    
})

app.get('/getstats', (req, res) => {
    res.json(objeto);
});


app.listen(3000, (err) => {
    if (err) console.log('Ocurrio un error'), process.exit(1);

    console.log('Escuchando en el puerto 3000');
})
