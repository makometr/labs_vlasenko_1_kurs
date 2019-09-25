#include <iostream>
#include <fstream>
#include <string>
#include <iomanip>
#include <cstring>
#include <string>
#include <stdlib.h>
#include <cassert>
#include <algorithm>

struct Station {
    char* name = nullptr;
    char* prev_name = nullptr;
    char* next_name = nullptr;

    Station() = default; // оставляем конструктор по-умолчанию

    Station(char* name_, char* prev_name_, char* next_name_){// конструктор
        name = name_;
        prev_name = prev_name_;
        next_name = next_name_;
    }
};


struct arrayData {
    int length = 0;
    Station* array = nullptr;
};


struct nodeBranch {
    int branch_num;
    arrayData stations;
    nodeBranch* next = nullptr;
    nodeBranch* prev = nullptr;
};

struct ListOfBranches
{
    nodeBranch* head = nullptr;
    nodeBranch* curr = nullptr;
};

void readAndAddStation(std::istream &in, ListOfBranches *list);//&in-ссылка. объект потока. передается раз.им. указатель
void addStationToList(ListOfBranches *list, int branchNumber, Station newStation);//если указатель,тофункция может поменять значение.
//если перемен, то она копируется каждый раз(не меню знач)
void clearStation(Station *station);

void addStationToStationsArray(arrayData* stations, Station newStation);
void addNodeBeforeCurrent(ListOfBranches *list, nodeBranch *prev);
void addNodeAfterPrev(ListOfBranches *list, nodeBranch *prev);

void arrangeStations(ListOfBranches list);
void arrangeStationsInOneBranch(arrayData stat);
void printList(std::ostream &out, ListOfBranches list);

void clear(nodeBranch* ptr);

int main()
{
    std::ifstream inputFile("data_2.txt");
    if (!inputFile.is_open()){
        std::cout << "Error! No such file in folder!\n";
        exit(-1);
    }

    ListOfBranches list;

    while (!inputFile.eof())
        readAndAddStation(inputFile, &list);//&-взятие адреса у перемен. стало указателем

    // std::cout << "\n Before arrange:\n"; 
    // printList(std::cout, list);

    arrangeStations(list);

    std::ofstream outputFile("result_2.txt");
    if (!outputFile.is_open()){
        std::cout << "Error! No such file in folder!\n";
        exit(-1);
    }
    printList(outputFile, list);

    // std::cout << "\n After arrange:\n";
    // printList(std::cout, list);
    // clear(list.head);
    return 0;
}


void readAndAddStation(std::istream &in, ListOfBranches *list){
    int bufferSize = 128;
    char buffer[bufferSize] = {}; // заполянием нуль-байтами
    in.getline(buffer, bufferSize);// читаем строку из файла и кладём в массив

    //std::cout << buffer << "\n";

    char *lexeme = nullptr; // указатель на часть строки

    lexeme = std::strtok(buffer, " ");// разбивает строку по проеблам
    int branchNumber = atoi(lexeme); // переводим первый символ в число

    lexeme = std::strtok(NULL," "); // получаем следующую лексему (часть строки, раделенную пробелами)
    char *name = new char[std::strlen(lexeme)+1]{}; // выделяем для неё память
    std::strcpy(name, lexeme); // записываем найденную лексему в в выделеный массив

    lexeme = std::strtok(NULL," ");
    char *prev = new char[std::strlen(lexeme)+1]{};
    std::strcpy(prev, lexeme);

    lexeme = std::strtok(NULL," ");
    char *next = new char[std::strlen(lexeme)+1]{};
    std::strcpy(next, lexeme);

    Station newStation(name, prev, next);//создаём объект посредством вызова его конструктора

    addStationToList(list, branchNumber, newStation);
}

void addStationToList(ListOfBranches *list, int branchNumber, Station newStation){
    if (list->head == nullptr){ // если спииска неt
        nodeBranch* newNode = new nodeBranch; // создаем новый узел
        newNode->branch_num = branchNumber; // записываем номер ветки в узел
        addStationToStationsArray(&(newNode->stations), newStation); // добавляем станцию в ветку
        // добавить инфо в массив станций
        list->head = newNode; // делаем новый узел головой списка (первым элементом)
        list->curr = list->head; // текуший узел теперь голова
        return; // досрочно выходим из функции
    }
    // общий случай: добавляем в уже существующий список
    list->curr = list->head; // делаем первый узел списка текущим
    nodeBranch* prev = nullptr; // создаем перменную, где будем хранить указатель на предыдущую станцию
    // т.к. список односвязный
    // ищем подходящий узел:
    while (true) { // пока не вставим узел
        if (list->curr == nullptr){ // дошли до последнего, т.е. текущий пуст
            // значит текущий узел заведомо больше всех отсальных
            addNodeAfterPrev(list, prev); // вставляем в конец списка
            list->curr->branch_num = branchNumber; // пишем номер ветки
            addStationToStationsArray(&(list->curr->stations), newStation); // добавляем станцию в массив станций
            break; // выходим из цикла
        }
        else
        if (branchNumber == list->curr->branch_num){ // если номер ветки равен номеру узла
            addStationToStationsArray(&(list->curr->stations), newStation);
            break; // выходим из цикла
        }
        else 
        if (branchNumber < list->curr->branch_num){ // если номер узла больше ветки
            addNodeBeforeCurrent(list, prev); // вставляем новый узел перед текущим
            list->curr->branch_num = branchNumber; // пишем номер ветки
            addStationToStationsArray(&(list->curr->stations), newStation); // добавляем станцию
            break; // выходим из цикла
        }

        prev = list->curr; // текуший становится предыдущим
        list->curr = list->curr->next; // а следующий в списке - текущим
    }
    
}

void addNodeBeforeCurrent(ListOfBranches *list, nodeBranch* prev){
    nodeBranch* newNode = new nodeBranch;
    if (prev != nullptr)
        prev->next = newNode; // следующий предыдущего равен новому

    newNode->next = list->curr; // у нового элемента перекидываем указатель next на текущий (первый)
    list->head = newNode; // делаем новый элемент головой списка
    list->curr = list->head; // делаем новый элемент текущим
}

void addNodeAfterPrev(ListOfBranches *list, nodeBranch *prev){
    // здесь prev не ноль точно
    nodeBranch* newNode = new nodeBranch;
    prev->next = newNode; // следующий последнего равен новому

    list->curr = newNode; // текущий равнен новому
}

void addStationToStationsArray(arrayData* data, Station newStation){
    Station* newArray = new Station[data->length+1];//выделение памяти под на 
    for (int i = 0; i < data->length; i++)
        newArray[i] = data->array[i];// копироваине старого массива в новый 
    newArray[data->length] = newStation;//запись новой ячейки в новый массив 
    data->length++;//увел на одну счет

    delete [] data->array;
    //data->array = nullptr; // можно убрать
    data->array = newArray;

    
}

void arrangeStations(ListOfBranches list){
    nodeBranch *currentBranch = list.head;
    while (currentBranch != nullptr){ // пока есть ветки в списке
        arrangeStationsInOneBranch(currentBranch->stations);
        currentBranch = currentBranch->next;
    }
}

void arrangeStationsInOneBranch(arrayData data){
    // находим первую станцию и ставим её на первое место
    for (int i = 0; i < data.length; i++)
        if (!strcmp(data.array[i].prev_name,"-")) // сравниваем строки
            std::swap(data.array[0], data.array[i]); // меняем местами

    // аналогично для последней
    for (int i = 0; i < data.length; i++)
        if (!strcmp(data.array[i].next_name,"-"))// сравниваем строки
            std::swap(data.array[data.length-1], data.array[i]); // меняем местами

    int indexCurrStation = 0; // индекс текущей станции
    while (indexCurrStation != data.length-1){
        for (int i = indexCurrStation+1; i < data.length; i++){
            if (!strcmp(data.array[indexCurrStation].next_name, data.array[i].name)){
                std::swap(data.array[indexCurrStation+1], data.array[i]);
                //if (strcmp(data.array[indexCurrStation].name, data.array[indexCurrStation+1].prev_name))
                   // std::cout << "Рассинхронизация: " << data.array[indexCurrStation].name << " " << data.array[indexCurrStation+1].prev_name << "\n";
            }
        }
        indexCurrStation++;
    }
      
}

void clearStation(Station *station){
    delete[] station->name;
    delete[] station->prev_name;
    delete[] station->next_name;
}


void printList(std::ostream &out, ListOfBranches list){
    nodeBranch *ptr = list.head;
    while (ptr != nullptr){
        out << "Ветка: " << ptr->branch_num << "\n";
        out << "    Кол-во станций: " << ptr->stations.length << "\n";
        for (int i = 0; i < ptr->stations.length; i++)
            out << "   " << i << ") "
                      << ptr->stations.array[i].name << " "
                      << ptr->stations.array[i].prev_name << " "
                      << ptr->stations.array[i].next_name << "\n";

        ptr = ptr->next;
    }
}

void clear(nodeBranch* ptr){
    if (ptr == nullptr) return;
    for (int i = 0; i < ptr->stations.length; i++)
        clearStation(&(ptr->stations.array[i]));
    delete [] ptr->stations.array;
    auto next = ptr->next;
    delete ptr;
    return clear(next);
}