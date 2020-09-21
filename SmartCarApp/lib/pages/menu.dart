import 'package:firebase_messaging/firebase_messaging.dart';
import 'package:flutter/material.dart';
import 'package:http/http.dart';
import 'dart:convert';
import 'dart:async';

class Home extends StatefulWidget {
  @override
  _HomeState createState() => _HomeState();
}

class _HomeState extends State<Home> {
  // Declaracion de las push notifications
  final FirebaseMessaging fcm = FirebaseMessaging();
  Future initialize() async {
    fcm.configure(
      onMessage: (Map<String, dynamic> message) async {
        print('onMessage: $message');
        showDialog(
            context: context,
            builder: (context) => AlertDialog(
                  content: ListTile(
                    title: Text(message['notification']['title']),
                    subtitle: Text(message['notification']['body']),
                  ),
                  backgroundColor: Colors.blue[100],
                  actions: <Widget>[
                    FlatButton(
                      child: Text('Ok'),
                      onPressed: () => Navigator.of(context).pop(),
                    )
                  ],
                ));
      },
      onLaunch: (Map<String, dynamic> message) async {
        print('onLaunch: $message');
      },
      onResume: (Map<String, dynamic> message) async {
        print('onResume: $message');
      },
    );
  }

  // Declaracion de los mensajes http
  void getData() async {
    Response response = await get('http://167.71.93.195:3000/getstats');
    Map data = jsonDecode(response.body);
    //print(data);
    this.state = data['state'];
    this.packets = data['packets'];
    this.obstacles = data['obstacles'];
    this.position = data['position'];
    this.averageWeight = double.parse(data['averageWeight'].toString());
    this.averageGoing = double.parse(data['averageGoing'].toString());
    this.averageReturn = double.parse(data['averageReturn'].toString());
  }

  void sendState(status) async {
    // 1 Para activo, 0 para inactivo
    Response response = await post('http://167.71.93.195:3000/setStatus',
        body: {'status': status});
    Map data = jsonDecode(response.body);
    print(data);
  }

  //Posicion 1 = partida, 2 = buzon, 3 = camino
  // Estado 0 = en reposo, 1 = de ida (buzon), 2 = de regreso, 3 = detenido
  int state, packets, obstacles, position;
  double averageWeight, averageGoing, averageReturn;

  @override
  Widget build(BuildContext context) {
    // Push notificacions
    this.initialize();
    // runs every 1 second
    Timer.periodic(new Duration(seconds: 1), (timer) {
      this.getData();
    });
    //getData();

    return Scaffold(
      appBar: AppBar(
        title: Text('Repartidor Inteligente'),
        centerTitle: true,
        backgroundColor: Colors.blue[300],
      ),
      backgroundColor: Colors.blue[50],
      body: Padding(
        padding: EdgeInsets.symmetric(vertical: 15, horizontal: 15),
        child: Column(
          children: <Widget>[
            Text('Datos',
                style: TextStyle(fontSize: 18, color: Colors.blue[500])),
            Padding(
              padding: const EdgeInsets.all(8.0),
              child: Row(
                mainAxisAlignment: MainAxisAlignment.spaceEvenly,
                children: <Widget>[
                  Row(
                    children: <Widget>[
                      Column(
                        children: <Widget>[
                          Icon(
                            Icons.location_searching,
                            size: 40,
                          ),
                          Text('Ubicacion:')
                        ],
                      )
                    ],
                  ),
                  Row(
                    children: <Widget>[
                      Column(
                        children: <Widget>[
                          Icon(
                            Icons.local_car_wash,
                            size: 40,
                          ),
                          Text('Estado: ')
                        ],
                      )
                    ],
                  ),
                ],
              ),
            ),
            Padding(
              padding: const EdgeInsets.all(8.0),
              child: Row(
                mainAxisAlignment: MainAxisAlignment.spaceEvenly,
                children: <Widget>[
                  Row(
                    children: <Widget>[
                      Column(
                        children: <Widget>[
                          Icon(
                            Icons.check_box,
                            size: 40,
                          ),
                          Text('Paquetes: ')
                        ],
                      )
                    ],
                  ),
                  Row(
                    children: <Widget>[
                      Column(
                        children: <Widget>[
                          Icon(
                            Icons.category,
                            size: 40,
                          ),
                          Text('Obstaculos: ')
                        ],
                      )
                    ],
                  ),
                ],
              ),
            ),
            Divider(
              color: Colors.blue[300],
              thickness: 3,
            ),
            Text(
              'Analisis',
              style: TextStyle(fontSize: 18, color: Colors.blue[500]),
            ),
            Padding(
              padding: const EdgeInsets.all(8.0),
              child: Row(
                mainAxisAlignment: MainAxisAlignment.spaceEvenly,
                children: <Widget>[
                  Row(
                    children: <Widget>[
                      Column(
                        children: <Widget>[
                          Icon(
                            Icons.multiline_chart,
                            size: 40,
                          ),
                          Text('Peso Promedio: ')
                        ],
                      )
                    ],
                  ),
                  Row(
                    children: <Widget>[
                      Column(
                        children: <Widget>[
                          Icon(
                            Icons.timer,
                            size: 40,
                          ),
                          Text('Promedio Ida: ')
                        ],
                      )
                    ],
                  ),
                ],
              ),
            ),
            Padding(
              padding: const EdgeInsets.all(8.0),
              child: Row(
                mainAxisAlignment: MainAxisAlignment.spaceEvenly,
                children: <Widget>[
                  Row(
                    children: <Widget>[
                      Column(
                        children: <Widget>[
                          Icon(
                            Icons.timer,
                            size: 40,
                          ),
                          Text('Promedio Regreso: ')
                        ],
                      )
                    ],
                  ),
                ],
              ),
            ),
            Divider(
              color: Colors.blue[300],
              thickness: 3,
            ),
            Text(
              'Funciones',
              style: TextStyle(fontSize: 18, color: Colors.blue[500]),
            ),
            Row(
              mainAxisAlignment: MainAxisAlignment.spaceEvenly,
              children: <Widget>[
                Row(
                  children: <Widget>[
                    Column(
                      children: <Widget>[
                        IconButton(
                          icon: Icon(Icons.play_arrow),
                          color: Colors.blue[400],
                          iconSize: 40,
                          onPressed: () {},
                        ),
                        Text('Activar Carro')
                      ],
                    )
                  ],
                ),
                Row(
                  children: <Widget>[
                    Column(
                      children: <Widget>[
                        IconButton(
                          icon: Icon(Icons.pause),
                          color: Colors.blue[400],
                          iconSize: 40,
                          onPressed: () {},
                        ),
                        Text('Desactivar Carro')
                      ],
                    )
                  ],
                ),
              ],
            ),
          ],
        ),
      ),
      floatingActionButton: FloatingActionButton(
        child: Icon(Icons.info),
        onPressed: () {
          Navigator.pushNamed(context, '/info');
        },
        backgroundColor: Colors.blue[300],
      ),
      floatingActionButtonLocation: FloatingActionButtonLocation.miniEndTop,
    );
  }
}
