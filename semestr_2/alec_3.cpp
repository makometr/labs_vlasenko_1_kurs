#include <iostream>
#include <fstream>
#include <string>
#include <iomanip>
#include <cstring>
#include <string>
#include <stdlib.h>

struct CityInfo {
    char* name = nullptr; // название города
    int populationNum; // население кол-во
};

struct nodeCity { // узел списка городов
    CityInfo info; //инф-ция о городе
    nodeCity* next = nullptr; // укзаатель не следующий элемент в односвязном списке городов
};

struct nodeRegion { // узел списка регионов
    int regionNum; // номер региона
    nodeRegion* next = nullptr; // // укзаатель не следующий элемент в односвязном списке регионов
    nodeCity* cityListHead = nullptr; // указатель не первый элемент списка городов в узле списка регионов
};

struct RegionList {
    nodeRegion* head = nullptr;
    nodeRegion* curr = nullptr;
};


void readData(RegionList *list);//&in-ссылка. объект потока. передается раз.им. указатель
void addCityToList(RegionList *list, int regionNumber, CityInfo newCity);//если указатель,то функция может поменять значение.
//если перемен, то она копируется каждый раз(не меню знач)

// работаем со списком регионов
void addCityToCityList(nodeCity** cityListHead, CityInfo newCity); // ** - указатель на указатель, т.к. требуется менять указатель в функции
void addNodeBeforeCurrent_Region(RegionList *list, nodeRegion *prev);
void addNodeAfterPrev_Region(RegionList *list, nodeRegion *prev);

// работаем со списком узлов
void addNodeBeforeCurrent_City(nodeCity **head, nodeCity **curr, nodeCity* prev);
void addNodeAfterPrev_City(nodeCity **head, nodeCity **curr, nodeCity* prev);

void printList_1(std::ostream &out, RegionList list);
void printList_2(std::ostream &out, RegionList list);

void clearNodeRegion(nodeRegion* ptrNode);
void clearNodeCity(nodeCity* ptr);

int main()
{
    RegionList regList;
    readData(&regList); //&-взятие адреса у перемен. стало указателем

    printList_1(std::cout, regList);
    printList_2(std::cout, regList);

    std::ofstream outputFile("result_3.txt");
    if (!outputFile.is_open()){
        std::cout << "Error! No such file in folder!\n";
        exit(-1);
    }
    printList_1(outputFile, regList);
    printList_2(outputFile, regList);

    clearNodeRegion(regList.head);
    return 0;
}


void readData(RegionList *list){
    int recordsNum = 0;
    std::cin >> recordsNum; std::cin.ignore();
    const int bufferSize = 64;

    for (int i = 0; i < recordsNum; i++){
        char buffer[bufferSize] = {}; // заполянием нуль-байтами
        std::cin.getline(buffer, bufferSize);

        char *lexeme = nullptr; // указатель на часть строки

        lexeme = std::strtok(buffer, " ");// разбивает строку по проеблам
        char *name = new char[std::strlen(lexeme)+1]{}; // выделяем для неё память
        std::strcpy(name, lexeme); // записываем найденную лексему в в выделеный массив

        lexeme = std::strtok(NULL," "); // получаем следующую лексему (часть строки, раделенную пробелами)
        int region = atoi(lexeme); // переводим первый символ в число

        lexeme = std::strtok(NULL," "); // получаем следующую лексему (часть строки, раделенную пробелами)
        int population = atoi(lexeme); // переводим первый символ в число

        CityInfo newCity;
        newCity.name = name;
        newCity.populationNum = population;

        addCityToList(list, region, newCity);
    }
}

void addCityToList(RegionList *list, int regionNumber, CityInfo newCity){
    if (list->head == nullptr){ // если спииска неt
        nodeRegion* newNode = new nodeRegion; // создаем новый узел - регион
        newNode->regionNum = regionNumber; // записываем номер региона
        addCityToCityList(&(newNode->cityListHead), newCity); // добавляем город в список городов региона
        // добавить инфо в список городов
        list->head = newNode; // делаем новый узел головой списка (первым элементом)
        return; // досрочно выходим из функции
    }
    // общий случай: добавляем в уже существующий список
    list->curr = list->head; // делаем первый узел списка текущим
    nodeRegion* prev = nullptr; // создаем перменную, где будем хранить указатель на предыдущий регион
    // т.к. список односвязный
    // ищем подходящий узел:
    while (true) { // пока не вставим узел
        if (list->curr == nullptr){ // дошли до последнего, т.е. текущий пуст
            // значит текущий узел заведомо больше всех отсальных
            addNodeAfterPrev_Region(list, prev); // вставляем в конец списка
            list->curr->regionNum = regionNumber; // пишем номер региона
            addCityToCityList(&(list->curr->cityListHead), newCity); // добавляем новый узел-город в список городов теккущего региона
            break; // выходим из цикла
        }
        else
        if (regionNumber == list->curr->regionNum){ // если номер региона равен номеру узла
            addCityToCityList(&(list->curr->cityListHead), newCity);
            break; // выходим из цикла
        }
        else 
        if (regionNumber < list->curr->regionNum){ // если номер узла больше региона
            addNodeBeforeCurrent_Region(list, prev); // вставляем новый узел перед текущим
            list->curr->regionNum = regionNumber; // пишем номер региона
            addCityToCityList(&(list->curr->cityListHead), newCity); // добавляем город в список
            break; // выходим из цикла
        }
        prev = list->curr; // текуший становится предыдущим
        list->curr = list->curr->next; // а следующий в списке - текущим
    }
}

void addNodeBeforeCurrent_Region(RegionList *list, nodeRegion* prev){
    nodeRegion* newNode = new nodeRegion;
    if (prev != nullptr)
        prev->next = newNode; // следующий предыдущего равен новому

    newNode->next = list->curr; // у нового элемента перекидываем указатель next на текущий (первый)
    list->head = newNode; // делаем новый элемент головой списка
    list->curr = list->head; // делаем новый элемент текущим
}

void addNodeAfterPrev_Region(RegionList *list, nodeRegion *prev){
    // здесь prev не ноль точно
    nodeRegion* newNode = new nodeRegion;
    prev->next = newNode; // следующий последнего равен новому

    list->curr = newNode; // текущий равнен новому
}

void addCityToCityList(nodeCity** cityListHead, CityInfo newCityInfo){
    if (*cityListHead == nullptr){ // если спииска городов у региона ещё нет
        nodeCity* newNode = new nodeCity;
        newNode->info = newCityInfo; // записываем информацию
        *cityListHead = newNode; // делаем новый узел головой списка (первым элементом)
        return; // досрочно выходим из функции
    }
    // общий случай: добавляем в уже существующий список
    nodeCity* curr = *cityListHead; // делаем первый узел списка текущим
    nodeCity* prev = nullptr; // создаем перменную, где будем хранить указатель на предыдущий регион
    // т.к. список односвязный
    // ищем подходящий узел:
    while (true) { // пока не вставим узел
        if (curr == nullptr){ // дошли до последнего, т.е. текущий пуст
            // значит название нового города последнее в списке (больше всех остальных)
            addNodeAfterPrev_City(cityListHead, &curr, prev); // вставляем в конец списка
            curr->info = newCityInfo;
            break; // выходим из цикла
        }
        else
        if (strcmp(curr->info.name, newCityInfo.name) > 0){ // если назваие города в узле больше нового
            addNodeBeforeCurrent_City(cityListHead, &curr, prev); // вставляем новый узел перед текущим
            curr->info = newCityInfo;
            break; // выходим из цикла
        }
        prev = curr; // текуший становится предыдущим
        curr = curr->next; // а следующий в списке - текущим
    }
}

void addNodeBeforeCurrent_City(nodeCity **head, nodeCity **curr, nodeCity* prev){
    nodeCity* newNode = new nodeCity;
    if (prev != nullptr)
        prev->next = newNode; // следующий предыдущего равен новому

    newNode->next = *curr; // у нового элемента перекидываем указатель next на текущий (первый)
    *head = newNode; // делаем новый элемент головой списка
    *curr = *head; // делаем новый элемент текущим
}

void addNodeAfterPrev_City(nodeCity **head, nodeCity **curr, nodeCity* prev){
    // здесь prev не ноль точно
    nodeCity* newNode = new nodeCity;
    prev->next = newNode; // следующий последнего равен новому

    *curr = newNode; // текущий равнен новому
}

void printList_1(std::ostream &out, RegionList list){
    nodeRegion *regionPtr = list.head;
    while (regionPtr != nullptr){
        out << "Region number: " << regionPtr->regionNum << "\n";
        nodeCity* cityPtr = regionPtr->cityListHead;
        while (cityPtr != nullptr){
            out << "   "
                << cityPtr->info.name << " "
                << cityPtr->info.populationNum << "\n";
            cityPtr = cityPtr->next;
        }
        out << "\n";
        regionPtr = regionPtr->next;
    }
}

void printList_2(std::ostream &out, RegionList list){
    nodeRegion *regionPtr = list.head;
    out << "|----------------|\n"
              << "| Reg |  People  |\n"; 
    while (regionPtr != nullptr){
        int people = 0;
        nodeCity* cityPtr = regionPtr->cityListHead;
        while (cityPtr != nullptr){
            people += cityPtr->info.populationNum;
            cityPtr = cityPtr->next;
        }
        out << "|" << std::setw(5) << regionPtr->regionNum
            << "|" << std::setw(10) << people << "|\n"; 
        regionPtr = regionPtr->next;
    }
    out << "|----------------|\n";
}

void clearNodeRegion(nodeRegion* ptrNode){
    if (ptrNode == nullptr)
        return;
    clearNodeCity(ptrNode->cityListHead);
    nodeRegion* next = ptrNode->next;
    delete ptrNode;
    return clearNodeRegion(next);
}

void clearNodeCity(nodeCity* ptr){
    if (ptr == nullptr)
        return;
    delete [] ptr->info.name;
    nodeCity* next = ptr->next;
    delete ptr;
    return clearNodeCity(next);
}








struct ListNode {
    ListNode* next = nullptr;
    int uniqueNumber; // уникальное число
    char* dataString; // строка
};

struct IndexNode { // узел списка для индексирования по числам
    IndexNode* next = nullptr;
    IndexNode* prev = nullptr;
    ListNode* dataPtr = nullptr;
};

struct IndexList { // хранит указатели на начало и конец индексного списка
    IndexNode* head = nullptr;
    IndexNode* last = nullptr;
};

void insertAfterLast(IndexList* list, IndexNode* curr, IndexNode* newNode){
    newNode->prev = list->last; // предыдущий элемент для нового - последний
    list->last->next = newNode; // следующий у последнего теперь новый
    list->last = newNode; // последний элемент теперь новый
}

void insertBeforeCurrent(IndexList* list, IndexNode* curr, IndexNode* newNode){
    if (curr == list->head){ // если вставляем перед головой
        newNode->next = curr; // указатель на следующий нового указывает на текущий
        curr->prev = newNode; // указатель на предыдущий текущего указывает на новый
        list->head = newNode; // новый элемент теперь голова
    }
    else {
        curr->prev->next = newNode; // указатель на следующий у узла перед текущим теперь указывает на новый узел
        newNode->prev = curr->prev; // указатель на предыдущий равен указателю на предыдущий у текущего
        newNode->next = curr; // указатель на следующий равен текущему
        curr->prev = newNode; // указатель на предыдущий у текущего элемента теперь указывает на новый узел
    }
}

void createNumberIndex(IndexList* list, ListNode* node){
    IndexNode* newNode = new IndexNode; // создаем новый узел - индекс
    newNode->dataPtr = node; // запоминаем данные
    if (list->head == nullptr){ // если спииска неt, то создаем его
        list->head = node; // делаем новый узел головой списка (первым элементом)
        list->last = node; // и так же последним элементом
        return;
    }
    // общий случай: добавляем в уже существующий индексный список. В цикле будем искать подходящий узел:
    IndexNode* curr = list->head; // делаем первый узел списка текущим
    while (true) { // пока не вставим узел идеи по списку
        if (curr == nullptr){ // дошли до последнего, т.е. текущий пуст заведомо больше всех остальных
            insertAfterLast(list, curr, newNode); // вставляем после текущего - в конец
            break; // выходим из цикла
        }
        else 
        if (node->number <= curr->dataPtr->number){ // если номер узла больше 
            insertBeforeCurrent(list, curr, newNode); // вставляем перед текущим
            break; // выходим из цикла
        }
        curr = curr->next; // а следующий в списке - текущим
    }
}

void printStrings(ListNode* head){
    // 1 вариант: в тупую
    // находим минимальное число в списке, выводим строку и удаляем этот узел
    // и так, пока список не удалится весь

    // 2 вариант: индексные списки
    ListNode* ptr = head; // переменная для ходьбы по изначальному списку
    IndexList* indexList; // указатель на голову первый элемент индексного списка
    while (ptr){ // идём по списку и добавляем узлы в индексный список
        createNumberIndex(indexList, ptr);
        ptr = ptr->next;
    }

    IndexNode* indexPtr = indexList->head;
    while (indexPtr){
        std::cout << "Num: " << indexPtr->dataPtr->uniqueNumber
                  << " String: " << indexPtr->dataPtr->dataString
                  << std::endl; 
        indexPtr = indexPtr->next;
    }
}   