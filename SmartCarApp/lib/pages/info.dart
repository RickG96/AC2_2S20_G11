import 'package:flutter/material.dart';

class Info extends StatefulWidget {
  @override
  _InfoState createState() => _InfoState();
}

class _InfoState extends State<Info> {
  @override
  Widget build(BuildContext context) {
    return Scaffold(
      appBar: AppBar(
        title: Text('Grupo X'),
        centerTitle: true,
        backgroundColor: Colors.blue[50],
      ),
      body: Padding(
        padding: EdgeInsets.all(20),
        child: Column(
          children: <Widget>[
            Text('Javier Alejandro Monterroso Lopez - 201700831'),
            SizedBox(height: 10,),
            Text('David Omar Enriquez Reyes - 201602744'),
            SizedBox(height: 10,),
            Text('Selvin Estuardo Icu Son - 201325657'),
            SizedBox(height: 10,),
            Text('Ricardo Antonio Alvarado Ramirez - 201603157'),
          ],
        ),
      ),
      backgroundColor: Colors.blue[300],
    );
  }
}
