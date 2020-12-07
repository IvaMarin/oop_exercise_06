/* 
 *  Мариничев И. А.
 *  М8О-208Б-19
 *  github.com/IvaMarin/oop_exercise_06
 *  Вариант 13:
 *  Контейнер: список
 *  Фигура:    ромб
 *  Аллокатор: динамический массив
 */


#include <iostream>
#include <vector>
#include <string>
#include <iomanip>
#include <cmath>
#include <algorithm>

#include "list.hpp"
#include "rhombus.hpp"



void AddSubOptions() {
    std::cout << "Add suboptions: " << std::endl;
    std::cout << "1. Add rhombus at the beginning of the list" << std::endl;
    std::cout << "2. Add rhombus at the end of the list" << std::endl;
    std::cout << "3. Add rhombus to the list by index" << std::endl;
    std::cout << std::endl;
}

void DeleteSubOptions() {
    std::cout << "Delete suboptions: " << std::endl;
    std::cout << "1. Delete rhombus from the beginning of the list" << std::endl;
    std::cout << "2. Delete rhombus from the end of the list" << std::endl;
    std::cout << "3. Delete rhombus from the list by index" << std::endl;
    std::cout << std::endl;
}

void PrintSubOptions() {
    std::cout << "Print suboptions: " << std::endl;
    std::cout << "1. Print the first rhombus in the list" << std::endl;
    std::cout << "2. Print the last rhombus in the list" << std::endl;
    std::cout << "3. Print the rhombus from the list by index" << std::endl;
    std::cout << std::endl;
}

void Options() {
    std::cout << "Options: " << std::endl;            
    std::cout << "1. Add rhombus" << std::endl;
    std::cout << "2. Delete rhombus" << std::endl;
    std::cout << "3. Print one rhombus in the list" << std::endl;
    std::cout << "4. Print all rhombuses in the list" << std::endl;
    std::cout << "5. Exit" << std::endl;
    std::cout << std::endl;
}

int main() {
    Options();

    List<Rhombus<int>> MyList;
    Rhombus<int> r;

    int  option, suboption, index;
    double area;
    
    std::cout << "Select option: ";
    while (std::cin >> option) {
        if (option == 1) { // Add rhombus
            AddSubOptions();
            std::cout << "Select add suboption: ";
            std::cin >> suboption;
            switch (suboption) {
                case 1: // Add rhombus at the beginning of the list
                    std::cin >> r.center.first >> r.center.second >> r.diag1 >> r.diag2;
                    MyList.PushFront(r);
                    std::cout << "Rhombus succesfully added" << std::endl;
                    break;
                case 2: // Add rhombus at the end of the list
                    std::cin >> r.center.first >> r.center.second >> r.diag1 >> r.diag2;
                    MyList.PushBack(r);
                    std::cout << "Rhombus succesfully added" << std::endl;
                    break;
                case 3: // Add rhombus to the list by index
                    std::cin >> index;
                    if (MyList.Length() + 1 < index || index < 0) {
                        std::cout << "Index is out of range!" << std::endl;
                        break;
                    }
                    std::cin >> r.center.first >> r.center.second >> r.diag1 >> r.diag2;
                    MyList.InsertByNumber(index, r);
                    std::cout << "Rhombus succesfully added" << std::endl;
                    break;
            }
        }
        else if (option == 2) { // Delete rhombus
            DeleteSubOptions();
            std::cout << "Select remove suboption: ";
            std::cin >> suboption;
            switch (suboption) {
                case 1: // Delete rhombus from the beginning of the list
                    if (MyList.Length() == 0) {
                        std::cout << "List is empty!" << std::endl;
                        break;
                    }
                    MyList.PopFront();
                    std::cout << "Rhombus succesfully removed" << std::endl;
                    break;
                case 2: // Delete rhombus from the end of the list
                    if (MyList.Length() == 0) {
                        std::cout << "List is empty!" << std::endl;
                        break;
                    }
                    MyList.PopBack();
                    std::cout << "Rhombus succesfully removed" << std::endl;
                    break;
                case 3: // Delete rhombus from the list by index
                    std::cin >> index;
                    if (MyList.Length() + 1 < index || index < 0) {
                        std::cout << "Index is out of range!" << std::endl;
                        break;
                    }
                    if (MyList.Length() == 0) {
                        std::cout << "List is empty!" << std::endl;
                        break;
                    }

                    MyList.EraseByNumber(index);
                    std::cout << "Rhombus succesfully removed" << std::endl;
                    break;
            }
        }
        else if (option == 3) { // Print one rhombus in the list
            PrintSubOptions();
            std::cout << "Select print suboption: ";
            std::cin >> suboption;
            switch (suboption) {
                case 1: // Print the first rhombus in the list
                    if (MyList.Length() == 0) {
                        std::cout << "List is empty!" << std::endl;
                        break;
                    }
                    Print(MyList.Front());
                    break;
                case 2: // Print the last rhombus in the list
                    if (MyList.Length() == 0) {
                        std::cout << "List is empty!" << std::endl;
                        break;
                    }
                    Print(MyList.Back());
                    break;
                case 3: // Print the rhombus from the list by index
                    std::cin >> index;
                    if (MyList.Length() + 1 < index || index < 0) {
                        std::cout << "Index is out of range!" << std::endl;
                        break;
                    }
                    if (MyList.Length() == 0) {
                        std::cout << "List is empty!" << std::endl;
                        break;
                    }
                    Print(MyList[index]);
                    break;
            }
        }
        else if (option == 4) { // Print all rhombuses in the list
            if (MyList.Length() == 0) 
                std::cout << "List is empty!" << std::endl;
            std::for_each(MyList.begin(), MyList.end(), [](const Rhombus<int> &r) {
                Print(r);
            });
        }
        else if (option == 5) { // Exit
            break; 
        }
        else // Wrong option
        std::cout << "There is no such option, please try again!" << std::endl;
        std::cout << std::endl;
        std::cout << "Select option: "; // Repeat input
    }
    
    return 0;
}