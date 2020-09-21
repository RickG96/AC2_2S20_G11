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

            db.get('hora_salida')
              .push({ id: nsalidas, hora: new Date().toISOString().replace(/T/, ' ').replace(/\..+/, ''), milis: new Date().getTime() })
              .write();

            if(db.get('hora_llegada').size().value() > 0) {
                db.get('t_regreso')
                  .push({ tiempo: (db.get('hora_salida').find({ id: nsalidas }).value().milis - db.get('hora_llegada').find({ id: nllegadas }).value().milis) / 1000 })
                  .write();
            } 
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

            db.get('hora_llegada')
              .push({ id: nllegadas, hora: new Date().toISOString().replace(/T/, ' ').replace(/\..+/, ''), milis: new Date().getTime() })
              .write();

            db.get('t_entrega')
              .push({ tiempo: (db.get('hora_llegada').find({ id: nsalidas }).value().milis - db.get('hora_salida').find({ id: nsalidas }).value().milis) / 1000 })
              .write();
        }
    }
    
    db.update('obstaculos', n => datos[3]) 
      .write();

    res.json('ok');
})

app.get('/getstats', (req, res) => {
    res.json(objeto);
});


app.listen(3000, (err) => {
    if (err) console.log('Ocurrio un error'), process.exit(1);

    console.log('Escuchando en el puerto 3000');
})
