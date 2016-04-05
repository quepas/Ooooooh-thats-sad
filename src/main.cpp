#include <iostream>

#include "Scanner.h"

int main() {
  Scanner scanner;
  std::cout << "----------------------- Basic info -----------------------" << std::endl;
  std::cout << "RF version: " << scanner.getRFVersion() << std::endl;
  std::cout << "Num. of sensors connected: " << scanner.getNumSensor() << std::endl;
  std::cout << "RF license: " << (scanner.hasLicense() ? "yes" : "no") << std::endl;
  std::cout << "----------------------------------------------------------" << std::endl;
  while (true) {
    std::cout << "\n\n";
    std::cout << "\t1. Start reconstruction." << std::endl;
    std::cout << "\t2. Stop reconstruction." << std::endl;
    int choice;
    std::cout << "What to do?: ";
    std::cin >> choice;
    switch (choice) {
    case 1:
      scanner.start();
      break;
    case 2:
      scanner.stop();
      break;
    default:
      std::cout << "Wrong choice.";
      break;
    }
  }
}
