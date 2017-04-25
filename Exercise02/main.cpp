#include <cstdlib>
#include <fstream>
#include <iostream>
#include <sstream>
#include <stlab/channel.hpp>
#include <stlab/default_executor.hpp>

using namespace stlab;

int main() {
  // Build the chain tail
  sender<std::string> resultInput;
  receiver<std::string> resultOutput;
  std::tie(resultInput, resultOutput) = channel<std::string>(default_executor);

  // Fan-out to console
  auto consolePrinter = [](std::string result) { std::cout << result << "\n"; };

  auto consolePrinterProcess = resultOutput | consolePrinter;

  // Fan-out to file
  std::ofstream outputFile;
  outputFile.open("Exercise2.output.txt");

  auto filePrinter = [&outputFile](std::string result) {
    outputFile << result << "\n";
  };

  auto filePrinterProcess = resultOutput | filePrinter;

  // resultBuilder routes between the two stages in the chain

  auto resultBuilder = [&resultInput](int intVal, std::string stringVal,
                                      double doubleVal) {
    std::ostringstream output;
    output << intVal << stringVal << doubleVal;
    resultInput(output.str());
  };

  resultOutput.set_ready();

  // Build the head of the chain

  sender<int> intInput;
  receiver<int> intOutput;
  std::tie(intInput, intOutput) = channel<int>(default_executor);
  // Structured Bindings gives us a much nicer:
  // auto [intInput, intOutput] = channel<int>(default_executor);

  sender<std::string> stringInput;
  receiver<std::string> stringOutput;
  std::tie(stringInput, stringOutput) = channel<std::string>(default_executor);

  sender<double> doubleInput;
  receiver<double> doubleOutput;
  std::tie(doubleInput, doubleOutput) = channel<double>(default_executor);

  auto builderProcess = join(default_executor, resultBuilder, intOutput,
                             stringOutput, doubleOutput);

  intOutput.set_ready();
  stringOutput.set_ready();
  doubleOutput.set_ready();

  intInput(0);
  intInput(1);
  stringInput("Bananas");
  doubleInput(5.2);
  stringInput("Apples");
  doubleInput(66.3);

  int end;
  std::cin >> end;
}