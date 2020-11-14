import 'package:flutter/material.dart';
import 'package:SmartCarApp/pages/menu.dart';
import 'package:SmartCarApp/pages/info.dart';

void main() => runApp(MaterialApp(initialRoute: '/', routes:{
  '/': (context) => Home(),
  '/info': (context) => Info(),
},));
