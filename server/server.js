const express = require('express');
const cors = require('cors');
const bodyParser = require('body-parser');
const mysql = require('mysql')

var app = express();
app.use(cors());
app.use(bodyParser.json());

var conexion = mysql.createConnection({
    host: 'bcqr7ep1diagrduvvqaa-mysql.services.clever-cloud.com',
    user: 'u9qoipgttt3ftqmp',
    password: '3fDEpSdal4zGhhHjrvHs',
    database: 'bcqr7ep1diagrduvvqaa',
    port: '3306'
});

conexion.connect(error => {
    if (error) throw error;
    console.log('Database server running!');
})

// Atributos de la entrega actual
var actual_hora_salida;
var actual_hora_entrega;
var actual_hora_retorno;
var actual_peso;

var actual_direccion;
var actual_obstaculos_ida;
var actual_obstaculos_retorno;

var actual_modo;
var actual_estado;
var actual_ubicacion;

function registrar_entrega() {
    const objeto_envio = {
        hora_salida:actual_hora_salida,
        hora_entrega:actual_hora_entrega,
        hora_retorno:actual_hora_retorno,
        num_obstaculos:actual_obstaculos_ida + actual_obstaculos_retorno,
        peso_paquete:actual_peso,
    }
    const sql = 'INSERT INTO entrega SET ?' ;
    conexion.query(sql, objeto_envio, (error, results) => {
        if(error) throw error;
        res.send(results)
    });
}

// Registra la hora de salida
app.post('/salida', (req, res) => {    
    actual_hora_salida = new Date();
    actual_peso = req.body.peso;

    actual_direccion = true;
    actual_ubicacion = 'En recorrido';
    actual_estado = 'En camino';
    
    res.json({ status: 200 });
});

// Registra la hora de entrega 
app.post('/entrega', (req, res) => {
    actual_hora_entrega = new Date();
    
    actual_ubicacion = 'Punto de destino';
    actual_estado = 'En reposo';
    
    res.json({ status: 200 });
})

// Registra la hora de salida hacia el buzón
app.post('/retorno_ini', (req, res) => {
    actual_direccion = false;
    actual_ubicacion = 'En recorrido';
    actual_estado = 'De regreso';
    
    res.json({ status: 200 });
})

// Registra la hora de llegada al buzon
app.post('/retorno_fin', (req, res) => {
    actual_hora_retorno = new Date();
    
    actual_ubicacion = 'Punto de partida';
    actual_estado = 'En reposo';
    
    registrar_entrega();
    res.json({ status: 200 });
})

// Registra cuando se encuentra con un obstaculo
app.post('/obstaculo_ini', (req, res) => {
    if (actual_direccion) { actual_obstaculos_ida+=1; }
    else { actual_obstaculos_retorno+=1; }
    
    actual_estado = 'Detenido';
    
    res.json({ status: 200 });
})

// Registra cuando el obstaculo es retirado
app.post('/obstaculo_fin', (req, res) => {
    actual_estado = actual_direccion? 'En camino' : 'De regreso';
    
    res.json({ status: 200 });
})

// Habilita o inhabilita el carro 
app.post('/modo', (req, res) => {
    actual_modo = req.body.modo;
    res.json({ status: 200 });
})

// Retorna el modo (habil o no) del carro
app.get('/modo', (req, res) => {
    res.json({ 
        status: 200,
        modo: actual_modo
    });
})

// Retorna el los datos actuales del carro
app.get('/actual', (req, res) => {
    res.json({
        estado: actual_estado,
        ubicacion: actual_ubicacion
    });
})

// Retorna los datos históricos y estadísticos
app.get('/historial', (req, res) => {
    res.json({
        paquetes: 45,
        obstaculos: 105,
        peso_promedio: 35.66,
        tiempo_promedio_ida: 15.15,
        tiempo_promedio_ret: 17.33
    });
})

app.listen(3000, (err) => {
    if (err) console.log('Ocurrio un error'), process.exit(1);

    console.log('Escuchando en el puerto 3000');
})
