import 'dart:async';
import 'dart:io';

class Result {
  int _result_var;
  Result() : _result_var = 0;

  void add(int value) {
    _result_var += value;
  }

  int get value => _result_var;
}

// Function to read the integer from a file.
Future<void> readIntegerFromFile(String filename, Result result) async {
  for (int i = 0; i < 100000; i++) {
    final file = File('./numbers/$filename.txt');
    final contents = await file.readAsString();
    result.add(
      int.parse(contents),
    );
  }
}

Future<void> main(List<String> arguments) async {
  // Check if the number of arguments is correct.
  if (arguments.length != 2) {
    print('Usage: dart main.dart num1 num2');
    exit(1);
  }

  var result = Result();
  // Read the integers from the files.
  final futures = [
    readIntegerFromFile(arguments[0], result),
    readIntegerFromFile(arguments[1], result),
  ];
  await Future.wait(futures);

  // Add the numbers and print the result.
  print('The result is: ${result.value}');
}
