# electricity-bill-estimator


Electricity Bill Estimator is a C++17 console application that helps users record household appliances and estimate their electricity units and monthly bill. It supports SDG 7 (Affordable and Clean Energy) by making energy use visible.

## Features

* Add appliances of three types: Bulb, Fan and AC.
* Calculate daily units (kWh) using watts × hours ÷ 1000. Bulbs also multiply by quantity.
* Show a high-usage warning when an AC runs more than 8 hours per day.
* View all appliances with units per day and cost per day.
* Calculate total units and an estimated bill for 1 to 31 days.
* Save appliances to `appliances.txt` and reload them on the next run.
* Validate input: numbers are range-checked and invalid or non-numeric input is rejected.
* Demonstrates abstraction, encapsulation, inheritance, runtime polymorphism, STL vectors, smart pointers, file handling and input validation.


## Assumptions

* The rate is a fixed assumed value of Rs 8 per unit. It is not an official tariff.
* Wattage values are entered by the user (typical examples: LED bulb 9 W, ceiling fan 75 W, AC 1500 W).

## Build and run

```
g++ -std=c++17 -Wall -Wextra -pedantic main.cpp -o estimator
./estimator
```

On Windows PowerShell, run `.\estimator` instead.

## Suggested demonstration

1. Add a Bulb (9 W, 5 hours, quantity 4), a Fan (75 W, 8 hours) and an AC (1500 W, 10 hours).
2. View all appliances and check the AC warning.
3. Choose total units and bill, and enter 30 days.
4. Enter an invalid value (such as a letter) to see validation.
5. Save and exit, run again, and view all to verify file persistence.

