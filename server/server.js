const express = require('express');
const cors = require('cors');
const bodyParser = require('body-parser');


var app = express();
app.use(cors());
app.use(bodyParser.json());

let enviar;

app.post('/infoed3', (req, res) => {
   if(req.body.data == null) {
       res.json("null");
   } else {
       let datos = req.body.data.split('-');
       
       enviar = {
           clean: datos[0],
           temp_user: datos[1],
           temp_amb: datos[2],
           temp_ok: datos[3],
           fase: datos[4],
       }
       console.log(enviar);
   }
   
    res.json('ok'); 
});


app.get('/getinfoed3', (req, res) => {
    res.json(enviar);
});

app.listen(3000, (err) => {
    if (err) console.log('Ocurrio un error'), process.exit(1);

    console.log('Escuchando en el puerto 3000');
});
