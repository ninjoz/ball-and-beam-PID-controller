import 'package:flutter/material.dart';


final _formKey = GlobalKey<FormState>();
double sliderValue = 0;
double inputValue = 0;
double lastAppliedValue = 0;

void main() => runApp(myApp());

class myApp extends StatelessWidget {
  static final String title = 'Control App';
  const myApp({super.key});

  @override
  Widget build(BuildContext context) => MaterialApp(
        debugShowCheckedModeBanner: false,
        title: title,
        home: ControlPage(title: title),
      );
}

class ControlPage extends StatefulWidget {
  final String title;
  const ControlPage({
    required this.title,
  });

  @override
  mainPage createState() => mainPage();
}

class mainPage extends State<ControlPage> {
  final formKey = GlobalKey<FormState>();

  @override
  Widget build(BuildContext context) => Scaffold(
        appBar: AppBar(
          title: Text(
            'distance controlling'.toUpperCase(),
          ),
        ),
        body: Form(
          key: _formKey,
          child: ListView(
            children: [
              Text('Please Choose a Distance Between 0 to 40 cm'),
              buildSlidingBar(),
              Text('Distance:'),
              buildDistanceInput(),
              buildApplyButton(),
              Text('Last Applied Distance'),
              Text(
                '$lastAppliedValue',
              ),
            ],
          ),
        ),
      );

  Widget buildSlidingBar() => Slider(
        value: (inputValue >= 0 && inputValue <= 40) ? inputValue : 0.0,
        min: 0,
        max: 40,
        onChanged: (value) => setState(() {
          inputValue = sliderValue = value;
        }),
      );
  Widget buildDistanceInput() => TextFormField(
        validator: (value) {
          if (double.parse(value!) >= 0 && double.parse(value!) <= 40) {
            return null;
          } else {
            return 'You Are Out Of Range';
          }
        },
        keyboardType: TextInputType.numberWithOptions(decimal: true),
        maxLength: 5,
        onChanged: (value) => setState(() {
          inputValue = double.parse(value);
        }),
        onSaved: (value) async => setState(() {
          inputValue = double.parse(value!);
        }),
      );

  Widget buildApplyButton() => Builder(
        builder: (context) => ElevatedButton(
          onPressed: () async {
            try {
              await applyDistance();
            } catch (e) {
              print('Error: $e');
            }
          },
          child: const Text('Apply'),
        ),
      );

  Future<void> applyDistance() async {
    setState(() {
      lastAppliedValue = sliderValue;
    });

    if (_formKey.currentState!.validate()) {
      
      setState(() {lastAppliedValue = inputValue;});
    }
  }

  Widget buildLastAppliedDis() => Text(
        'Last Applied Distance',
      );
}
