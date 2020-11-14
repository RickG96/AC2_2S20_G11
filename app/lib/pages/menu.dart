import 'package:firebase_messaging/firebase_messaging.dart';
import 'package:flutter/material.dart';
import 'package:http/http.dart' as http;
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
    var response = await http.get('http://64.227.23.182:3000/infoed3');
    Map data = jsonDecode(response.body);
    // print(data);
    this.objeto = data['clean'];
    this.usuario = data['temp_user'];
    this.ambiente = data['temp_amb'];
    this.warning = data['temp_ok'];
    this.etapa = data['fase'];
  }

  void sendState(status) async {
    // 1 Para activo, 0 para inactivo
    Map<String, String> requestHeaders = {
      'Content-type': 'application/json; charset=utf-8',
      'Accept': 'application/json'
    };
    Map data = {"status": status};
    var response = await http.post('http://64.227.23.182:3000/infoed3',
        body: jsonEncode(data), headers: requestHeaders);
    // Map data = jsonDecode(response.body);
    print(response.body);
  }

  // Objetos Desinfectados, Temperatura, Usuario, Visitas Semanales
  String objeto = '0',
      usuario = '0',
      ambiente = '0',
      warning = '0',
      etapa = '0';

  @override
  Widget build(BuildContext context) {
    // Push notificacions
    this.initialize();

    getData();

    return Scaffold(
      appBar: AppBar(
        title: Text('Estacion Inteligente'),
        centerTitle: true,
        backgroundColor: Colors.blue[300],
      ),
      backgroundColor: Colors.blue[50],
      body: Padding(
        padding: EdgeInsets.symmetric(vertical: 5, horizontal: 15),
        child: Column(
          children: <Widget>[
            Text(
              'Etapa Actual:' + this.etapa,
              style: TextStyle(fontSize: 18, color: Colors.blue[500]),
            ),
            Divider(
              color: Colors.blue[300],
              thickness: 3,
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
                            Icons.category,
                            size: 40,
                          ),
                          Text('Objetos: ' + this.objeto)
                        ],
                      )
                    ],
                  ),
                  Row(
                    children: <Widget>[
                      Column(
                        children: <Widget>[
                          Icon(
                            Icons.accessibility,
                            size: 40,
                          ),
                          Text('Temperatura: ' + this.usuario)
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
                            Icons.ac_unit,
                            size: 40,
                          ),
                          Text('Ambiente: ' + this.ambiente)
                        ],
                      )
                    ],
                  ),
                  Row(
                    children: <Widget>[
                      Column(
                        children: <Widget>[
                          Icon(
                            Icons.ac_unit,
                            size: 40,
                          ),
                          Text('Visitas Semanales: ' + this.ambiente)
                        ],
                      )
                    ],
                  )
                ],
              ),
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
