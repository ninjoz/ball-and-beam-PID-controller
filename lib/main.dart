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
          backgroundColor:Color.fromARGB(255, 75,0,130),
          title: Text(
            'distance controlling'.toUpperCase(),style: TextStyle(fontSize: 33,color:Color.fromARGB(255, 242, 237, 241),),
          ),
        ),
        body: Form(
          key: _formKey,
          child: ListView(
            children: [
              Text('Please Choose a Distance Between 0 to 40 cm',style :TextStyle(fontSize: 15,color:Color.fromARGB(7,200,160,155))),
              Row(children: [
                
                buildSlideLable('   0'),
                
                Expanded(
                  child: buildSlidingBar(),
                ),
                buildSlideLable('40   '),

              ]),
              const SizedBox(height: 16,),
              Row(children: [
                Text('  Distance:  ',style:TextStyle(fontSize: 25,color:Color.fromARGB(255, 75,0,130))),
                Expanded(
                  child: buildDistanceInput(),
                ),
              ]),
              const SizedBox(height: 16,),
              buildApplyButton(),
              const SizedBox(height: 16,),
              Row(children: [
                Text('  Last Applied Distance : ',style:TextStyle(fontSize: 25,color:Color.fromARGB(255, 75,0,130),),),
                Expanded(
                  child: Text(

                    '$lastAppliedValue',style:TextStyle(fontSize: 25,color:Color.fromARGB(255, 75,0,130),),
                  ),
                ),
              ]),
            ],
          ),
        ),
      );
  Widget buildSlideLable(String value) => Container(
      width: 25,
      child: Text(
        value,
        style: TextStyle(fontSize: 18, fontWeight: FontWeight.bold),
      ));
  Widget buildSlidingBar() => SizedBox(
        width: 300,
        child: Slider(
          value: (inputValue >= 0 && inputValue <= 40) ? inputValue : 0.0,
          min: 0,
          max: 40,
          divisions: 40,
          label: sliderValue.round().toString(),
          onChanged: (value) => setState(() {
            inputValue = sliderValue = value;
          }),
        ),
      );
  Widget buildDistanceInput() => TextFormField(
        validator: (value) {
          if (double.parse(value!) >= 1 && double.parse(value!) <= 40) {
            return null;
          } else if (value[1] == '.' || value[0] == '.') {
            return 'Please Enter An Integer Number';
          } else {
            return 'You Are Out Of Range';
          }
        },
        keyboardType: TextInputType.numberWithOptions(decimal: true),
        maxLength: 2,
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
      setState(() {
        lastAppliedValue = inputValue;
        _formKey.currentState?.reset();
      });
    }
  }

  Widget buildLastAppliedDis() => Text(
        'Last Applied Distance',
      );
}
