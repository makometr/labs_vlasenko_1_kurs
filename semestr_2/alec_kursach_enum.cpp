#include <iostream>
#include <fstream>
#include <string>
#include <iomanip>
#include <cstring>
#include <string>
#include <stdlib.h>
#include <locale.h>

struct StudentInfo {
    char* FIO = nullptr; // фио студента
    FinanceType type; // форма обучения
    int money; // деньги 
    int marks[5]; // оценки
};

struct nodeStudent { // узел списка студентов
    StudentInfo info; //инф-ция о студенте
    nodeStudent* next = nullptr; // укзаатель не следующий элемент в односвязном списке студентов
};

struct nodeGroup { // узел списка групп
    int groupNumber; // номер группы
    int studentsNumber = 0; // кол-во студентов
    int losersNumber = 0; // кол-во двоечников
    nodeGroup* next = nullptr; // укзаатель не следующий элемент в односвязном списке групп
    nodeStudent* studentListHead = nullptr; // указатель не первый элемент списка студентов в узле списка группы
};

struct GroupList {
    nodeGroup* head = nullptr;
    nodeGroup* curr = nullptr;
    float sumMark = 0;
    int sumStudents = 0;
};

// Для индексирования:

struct IndexNodeNumber {
    IndexNodeNumber* next = nullptr;
    IndexNodeNumber* prev = nullptr;
    StudentInfo* data = nullptr;
};

struct IndexNumberList {
    IndexNodeNumber* head = nullptr;
    IndexNodeNumber* last = nullptr;
};

struct IndexNode { // узел списка для индексирования
    IndexNode* next = nullptr;
    IndexNode* prev = nullptr;
    nodeGroup* dataPtr = nullptr;
};

struct IndexList {
    IndexNode* head = nullptr;
    IndexNode* last = nullptr;
};

enum class FinanceType : bool {
    BUDGET = false,
    CONTRACT = true
};

// работа с меню
void openChangeMenu(GroupList* list);
void openChangeStudentMenu(GroupList* list, StudentInfo* student);

// чтение и обработка данных
void readDataFromConsole(GroupList *list);
void readDataFromFile(GroupList *list);
void processDataString(GroupList* list, char *str);

// обработка с данных в списке
float getStudentAverage(StudentInfo info);
float getGroupsAverage(GroupList list);
int getGroupSchoolarship(nodeGroup* group);
int getGroupPayment(nodeGroup* group);
int getContractStudentsNumber(nodeGroup* group);
bool isStudentExcellentAndContract(StudentInfo info);
bool isStudentLooser(StudentInfo info);
void addMarksToStatistics(GroupList* list, StudentInfo newStudent);

// работаем со списком групп
void addStudentToList(GroupList *list, int groupNum, StudentInfo newStudent);
void addNodeBeforeCurrent_Group(GroupList *list, nodeGroup *prev, int groupNum);
void addNodeAfterPrev_Group(GroupList *list, nodeGroup *prev, int groupNum);

// работаем со списком cтудентов
void addStudentToStudentList(nodeGroup* groupNode, StudentInfo newStudent);
void addNodeBeforeCurrent_Student(nodeGroup* groupNode, nodeStudent *curr, nodeStudent* prev, StudentInfo);
void addNodeAfterPrev_Student(nodeGroup* groupNode, nodeStudent *curr, nodeStudent* prev, StudentInfo);
void deleteStudentNode(GroupList* list, nodeStudent* chNode);
nodeStudent* getStudentByIndex(GroupList* list, int index);

// функции вывода
void printStudents(std::ostream &out, GroupList list);
void printAllInFiles(GroupList groupsList);
void print_1a(std::ostream &out, GroupList list);
void print_1b(std::ostream &out, GroupList list);
void print_2a(std::ostream &out, GroupList list);
void print_2b(std::ostream &out, GroupList list);

// работа с индексными списками
void createNumberStudentsIndex(IndexList* list, nodeGroup* node);
void createLooserStudentsIndex(IndexList* list, nodeGroup* node);
void createIndexList(IndexList* list, IndexNode* node);
void insertAfterLast(IndexList* list, IndexNode* curr, IndexNode* newNode);
void insertBeforeCurrent(IndexList* list, IndexNode* curr, IndexNode* newNode);

// очистка памяти
void clearNodeIndex(IndexNode* node);
void clearNodeGroup(nodeGroup* ptrNode);
void clearNodeStudent(nodeStudent* ptr);

int main() {
    GroupList groupsList;
    char key;
    bool isDataReaded = false;
    do {
        std::cout << "How do you like to enter the info? (1 - from console, 2 - from data.txt)" << std::endl;
        std::cin >> key;
        switch (key) {
            case '1':
                readDataFromConsole(&groupsList);
                isDataReaded = true;
                break;
            case '2':
                readDataFromFile(&groupsList);
                isDataReaded = true;
                break;
            default:
                std::cout << "Incorrect symbol! Try again.\n";
                break;
        }
        
    } while (isDataReaded == false);
    std::cout << "Data was successfully readed.\n";
    bool isExit = false;
    do {
        std::cout << "What to do now? Press:\n"
                << "  1 - Task 1-a\n"
                << "  2 - Task 1-b\n"
                << "  3 - Task 2-a\n"
                << "  4 - Task 2-b\n"
                << "  5 - Write all tasks in files\n"
                << "  6 - View Data\n"
                << "  7- Change data\n"
                << "  0 - Exit\n";
        std::cin >> key;

        switch (key) {
            case '1':
                print_1a(std::cout, groupsList);
                break;
            case '2':
                print_1b(std::cout, groupsList);
                break;
            case '3':
                print_2a(std::cout, groupsList);
                break;
            case '4':
                print_2b(std::cout, groupsList);
                break;
            case '5':
                printAllInFiles(groupsList);
                std::cout << "All tasks were successfully written in txt files.\n";
                break;
            case '6':
                printStudents(std::cout, groupsList);
                break;
            case '7':
                openChangeMenu(&groupsList);
                break;
            case '0': 
                isExit = true;
                break;
            default:
                std::cout << "Incorrect symbol! Try again.\n";
                break;
        }
    } while (isExit == false);

    clearNodeGroup(groupsList.head);
    return 0;
}

void openChangeMenu(GroupList* list){
    char key;
    bool isExit = false;
    do {
        std::cout << "How do you like to change the data? Press:\n"
                  << "    1 - Add Student info\n"
                  << "    2 - Change Student info\n"
                  << "    3 - Delete Student info\n"
                  << "    0 - Exit From Chainging menu" << std::endl;
        std::cin >> key;

        switch (key) {
            case '1': {
                std::cout << "Enter string in format:\n<Surname> <Name> <patronymic> <group> b/c <money> <5 marks>" << std::endl; 
                char buffer[50 + 3 + 4 + 1 + 5 + 1] = {};
                std::cin.ignore();
                std::cin.getline(buffer, 50 + 3 + 4 + 1 + 5 + 1);
                processDataString(list, buffer);
                break;
            }
            case '2': {
                int index;
                std::cout << "Enter index of changing student: ";
                std::cin >> index;
                nodeStudent* chNode = getStudentByIndex(list, index);
                if (chNode == nullptr){
                    std::cout << "Invalid index!\n";
                    char tmp;
                    std::cin >> tmp;
                }
                else {
                    StudentInfo* oldInfo = &(chNode->info);
                    openChangeStudentMenu(list, oldInfo);
                }
                break;
            }
            case '3': {
                int index;
                std::cout << "Enter index of deleting student: ";
                std::cin >> index;
                nodeStudent* chNode = getStudentByIndex(list, index);
                if (chNode == nullptr){
                    std::cout << "Invalid index!\n";
                    char tmp;
                    std::cin >> tmp;
                }
                else {
                    deleteStudentNode(list, chNode);
                }
                break;
            }   
            case '0':
                isExit = true;
                break;
            default:
                std::cout << "Incorrect symbol! Try again.\n";
                break;
        }
        
    } while (isExit == false);
}

void openChangeStudentMenu(GroupList* list, StudentInfo* student){
    char key;
    bool isExit = false;
    do {
        std::cout << "What piece of student`s data do you want to change?\n"
                  << "    1 - Education Form\n"
                  << "    2 - Money\n"
                  << "    3 - Marks (5)\n"
                  << "    0 - Exit From Chainging menu" << std::endl;
        std::cin >> key;

        switch (key) {
            case '1': {
                std::cout << "Enter new Education form:\n<b> - Budget\n<c> - Contract\n<0> - Exit" << std::endl; 
                char ans;
                bool isExit = false;
                do {
                    std::cin >> ans;
                    switch (ans) {
                        case 'b':
                            student->type = FinanceType::BUDGET;
                            isExit = true;
                            break;
                        case 'c':
                            student->type = FinanceType::CONTRACT;
                            isExit = true;
                            break;
                        case '0':
                            isExit = true;
                            break;
                        default:
                            break;
                    }
                    break;
                } while (isExit == false);
            }
            break;
            case '2':
                if (student->type == FinanceType::BUDGET)
                    std::cout << "Enter new schoolarship for student: (number)\n" << std::endl;
                else
                    std::cout << "Enter new payment for student: (number)\n" << std::endl;
                int ans;
                std::cin >> ans;
                student->money = ans;
                break;
            case '3':
                std::cout << "Enter 5 new marks of student: (number from 2 to 5):\n";
                for (int i = 0; i < 5; i++){
                    int mark;
                    std::cin >> mark;
                    student->marks[i] = mark;
                }
                break;
            case '0':
                isExit = true;
                break;
            default:
                std::cout << "Incorrect symbol! Try again.\n";
                break;
        }
    } while (isExit == false);
}


void readDataFromConsole(GroupList *list){
    std::cout << "Enter number of students to read: ";
    int recordsNum = 0;
    std::cin >> recordsNum; std::cin.ignore();
    const int bufferSize = 50 + 3 + 4 + 1 + 5 + 1;
    std::cout << "Enter strings in format:\n<Surname> <Name> <patronymic> <group> b/c <money> <5 marks>\n"; 
    for (int i = 0; i < recordsNum; i++){
        char buffer[bufferSize] = {}; // заполянием нуль-байтами
        std::cin.getline(buffer, bufferSize);
        processDataString(list, buffer);
    }
}

void readDataFromFile(GroupList *list){
    std::ifstream inputFile("data_kursach.txt");
    if (!inputFile.is_open()){
        std::cout << "Error! No file in folder!\n";
        exit(-1);
    }
    int recordsNum = 0;
    inputFile >> recordsNum; inputFile.ignore();
    const int bufferSize = 50 + 3 + 4 + 1 + 5 + 1;

    for (int i = 0; i < recordsNum; i++){
        char buffer[bufferSize] = {}; // заполянием нуль-байтами
        inputFile.getline(buffer, bufferSize);
        processDataString(list, buffer);
    }
}

void processDataString(GroupList* list, char *str){
    char *lexeme = nullptr; // указатель на часть строки

    lexeme = std::strtok(str, " "); // разбивает строку по пробелам
    char *surname = new char[std::strlen(lexeme)+1]{};
    std::strcpy(surname, lexeme); // записываем найденную лексему в в выделеный массив

    lexeme = std::strtok(NULL, " ");
    char *name = new char[std::strlen(lexeme)+1]{}; // выделяем для неё память
    std::strcpy(name, lexeme);

    lexeme = std::strtok(NULL, " ");
    char *patronymic = new char[std::strlen(lexeme)+1]{};
    std::strcpy(patronymic, lexeme);

    lexeme = std::strtok(NULL," "); // получаем следующую лексему (часть строки, разделенную пробелами)
    int groupNum = atoi(lexeme); // переводим первый символ в число

    lexeme = std::strtok(NULL, " ");
    char *typeStr = new char[std::strlen(lexeme)+1]{};
    std::strcpy(typeStr, lexeme);
    FinanceType type;
    if (!strcmp(typeStr, "b"))
        type = FinanceType::BUDGET;
    else if (!strcmp(typeStr, "c"))
        type = FinanceType::CONTRACT;
    else std::cout << "Error on reading line !" << std::endl;
    delete [] typeStr;

    lexeme = std::strtok(NULL," ");
    int moneyValue = atoi(lexeme);

    int marks[5];
    for (int i = 0; i < 5; i++){
        lexeme = std::strtok(NULL," "); // получаем следующую лексему (часть строки, разделенную пробелами)
        marks[i] = atoi(lexeme); // переводим первый символ в число
    }

    // слепляем имя фамилию отчество в фио
    char* FIO = new char[strlen(surname) + strlen(name) + strlen(patronymic) + 3]{};
    strcpy(FIO, surname);    delete[] surname;
    strcat(FIO, " ");
    strcat(FIO, name);       delete[] name;
    strcat(FIO, " ");
    strcat(FIO, patronymic); delete[] patronymic;

    StudentInfo newStudent;
    newStudent.FIO = FIO;
    newStudent.type = type;
    newStudent.money = moneyValue;
    for (int i = 0; i < 5; i++)
        newStudent.marks[i] = marks[i];

    addStudentToList(list, groupNum, newStudent);
}

void addStudentToList(GroupList *list, int groupNum, StudentInfo newStudent){
    addMarksToStatistics(list, newStudent);
    if (list->head == nullptr){ // если спииска неt
        nodeGroup* newNode = new nodeGroup; // создаем новый узел - группу
        newNode->groupNumber = groupNum; // записываем номер группы
        addStudentToStudentList(newNode, newStudent); // добавляем студента в список студентов группы
        list->head = newNode; // делаем новый узел головой списка (первым элементом)
        return; // досрочно выходим из функции
    }
    // общий случай: добавляем в уже существующий список
    list->curr = list->head; // делаем первый узел списка текущим
    nodeGroup* prev = nullptr; // создаем перменную, где будем хранить указатель на предыдущую группу
    // т.к. список односвязный
    // ищем подходящий узел:
    while (true) { // пока не вставим узел
        if (list->curr == nullptr){ // дошли до последнего, т.е. текущий пуст
            // значит текущий узел-группа заведомо больше всех остальных
            addNodeAfterPrev_Group(list, prev, groupNum); // вставляем в конец списка
            addStudentToStudentList(list->curr, newStudent); // добавляем новый узел-студент в список студентов теккущего группы
            break; // выходим из цикла
        }
        else // ok
        if (groupNum == list->curr->groupNumber){ // если номер группы равен номеру узла
            addStudentToStudentList(list->curr, newStudent);
            break; // выходим из цикла
        }
        else  // ok
        if (groupNum < list->curr->groupNumber){ // если номер узла больше группы
            addNodeBeforeCurrent_Group(list, prev, groupNum); // вставляем новый узел перед текущим
            addStudentToStudentList(list->curr, newStudent); // добавляем город в список
            break; // выходим из цикла
        }
        prev = list->curr; // текуший становится предыдущим
        list->curr = list->curr->next; // а следующий в списке - текущим
    }
}

void addNodeBeforeCurrent_Group(GroupList *list, nodeGroup* prev, int groupNum){
    nodeGroup* newNode = new nodeGroup;
    newNode->groupNumber = groupNum;
    if (prev != nullptr) // если перед вставляемым узлом стоит узел
        prev->next = newNode; // следующий предыдущего равен новому
    else // если перед вставляемым пусто
        list->head = newNode; // то новый элемент - голова

    newNode->next = list->curr; // у нового элемента перекидываем указатель next на текущий
    list->curr = newNode; // делаем новый текущим
}

void addNodeAfterPrev_Group(GroupList *list, nodeGroup *prev, int groupNum){
    // здесь prev не ноль точно
    nodeGroup* newNode = new nodeGroup;
    newNode->groupNumber = groupNum;
    prev->next = newNode; // следующий последнего равен новому

    list->curr = newNode; // текущий равнен новому
}

void addStudentToStudentList(nodeGroup* groupNode, StudentInfo newStudentInfo){
    groupNode->studentsNumber++;
    if (isStudentLooser(newStudentInfo))
        groupNode->losersNumber++;
    if (groupNode->studentListHead == nullptr){ // если спииска студентов у группы ещё нет
        nodeStudent* newNode = new nodeStudent; // создаём студента
        newNode->info = newStudentInfo; // записываем информацию
        groupNode->studentListHead = newNode; // делаем новый узел головой списка (первым элементом)
        return; // досрочно выходим из функции
    }
    // общий случай: добавляем в уже существующий список
    nodeStudent* curr = groupNode->studentListHead; // делаем первый узел списка текущим
    nodeStudent* prev = nullptr; // создаем перменную, где будем хранить указатель на предыдущий регион
    // т.к. список односвязный
    // ищем подходящий узел:
    while (true) { // пока не вставим узел
        if (curr == nullptr){ // дошли до последнего
            // значит фио нового студента последнее в списке (больше всех остальных)
            addNodeAfterPrev_Student(groupNode, curr, prev, newStudentInfo); // вставляем в конец списка
            // curr->info = newStudentInfo;
            break; // выходим из цикла
        }
        else
        if (strcmp(curr->info.FIO, newStudentInfo.FIO) > 0){ // если фио студента в узле больше нового
            addNodeBeforeCurrent_Student(groupNode, curr, prev, newStudentInfo); // вставляем новый узел перед текущим
            // curr->info = newStudentInfo;
            break; // выходим из цикла
        }
        prev = curr; // текуший становится предыдущим
        curr = curr->next; // а следующий в списке - текущим
    }
}

void addNodeBeforeCurrent_Student(nodeGroup* groupNode, nodeStudent *curr, nodeStudent* prev, StudentInfo newStudentInfo){
    nodeStudent* newNode = new nodeStudent;
    newNode->info = newStudentInfo;
    if (prev != nullptr)
        prev->next = newNode; // следующий предыдущего равен новому
    else
        groupNode->studentListHead = newNode;

    newNode->next = curr; // у нового элемента перекидываем указатель next на текущий (первый)
}

void addNodeAfterPrev_Student(nodeGroup* groupNode, nodeStudent *curr, nodeStudent* prev, StudentInfo newStudentInfo){
    // здесь prev не ноль точно
    nodeStudent* newNode = new nodeStudent;
    newNode->info = newStudentInfo;
    prev->next = newNode; // следующий последнего равен новому
}

nodeStudent* getStudentByIndex(GroupList* list, int index){
    if (index > list->sumStudents-1)
        return nullptr;
    nodeGroup* ptr = list->head;
    nodeStudent* stud = list->head->studentListHead;
    for (int i = 0; i < index; i++){
        stud = stud->next;
        if (stud == nullptr){
            ptr = ptr->next;
            stud = ptr->studentListHead;
        }
    }
    return stud;
}

float getGroupsAverage(GroupList list){
    return list.sumMark / list.sumStudents;
}

float getStudentAverage(StudentInfo info){
    float res = 0;
    for (int i = 0; i < 5; i++)
        res += info.marks[i];
    return res / 5;
}

int getGroupSchoolarship(nodeGroup* group){
    nodeStudent* student = group->studentListHead;
    int result = 0;
    while (student != nullptr){
        if (student->info.type == FinanceType::BUDGET)
            result += student->info.money;
        student = student->next;
    }
    return result;
}

int getGroupPayment(nodeGroup* group){
    nodeStudent* student = group->studentListHead;
    int result = 0;
    while (student != nullptr){
        if (student->info.type == FinanceType::CONTRACT)
            result += student->info.money;
        student = student->next;
    }
    return result;
}

int getContractStudentsNumber(nodeGroup* group){
    nodeStudent* student = group->studentListHead;
    int result = 0;
    while (student != nullptr){
        if (student->info.type == FinanceType::CONTRACT)
            result++;
        student = student->next;
    }
    return result; 
}

void deleteStudentNode(GroupList* list, nodeStudent* chNode){
    nodeGroup* groupPtr = list->head;
    nodeGroup* prevGroupPtr = nullptr;
    nodeStudent* studPtr = list->head->studentListHead;
    nodeStudent* prevStudPtr = nullptr;
    while (groupPtr != nullptr){
        if (studPtr == chNode) // нашли узел с таким же адресом
            break;
        prevStudPtr = studPtr;
        studPtr = studPtr->next;
        if (studPtr == nullptr){
            prevGroupPtr = groupPtr;
            groupPtr = groupPtr->next;
            studPtr = groupPtr->studentListHead;
            prevStudPtr = nullptr;
        }
    } // должны выйти через break
    list->sumStudents--;
    list->sumMark -= getStudentAverage(studPtr->info);
    if (prevStudPtr == nullptr) // если удаляемый узел стоит первым
        groupPtr->studentListHead = studPtr->next;
    else // не первый
        prevStudPtr->next = studPtr->next; 
    studPtr->next = nullptr;
    clearNodeStudent(studPtr);

    // проверяем пуста ли группа после удаления из неё студента
    if (groupPtr->studentListHead == nullptr){ // если пуста
        if (prevGroupPtr == nullptr) // если группа стоит первой в списке
            list->head = groupPtr->next; // меняем указатель на начало списка
        else
            prevGroupPtr->next = groupPtr->next; // меняем указатель предыдущей группы
        delete groupPtr;
    }
}

void printStudents(std::ostream &out, GroupList list){
    nodeGroup *groupPtr = list.head;
    int index = 0;
    while (groupPtr != nullptr){
        nodeStudent* studPtr = groupPtr->studentListHead;
        while (studPtr != nullptr){
            out << "Index: " << index << "\n"
                << "    Group: " << groupPtr->groupNumber << "\n"
                << "    FIO: " << studPtr->info.FIO << "\n"
                << "    Type: " << static_cast<int>(studPtr->info.type) << "\n"
                << "    Money: " << studPtr->info.money << "\n"
                << "    Marks: ";
            for (int i = 0; i < 5; i++)
                std::cout << studPtr->info.marks[i] << " ";
            std::cout << std::endl;
            studPtr = studPtr->next;
            index++;
            out << "\n";
        }
        groupPtr = groupPtr->next;
    }
}

void printAllInFiles(GroupList groupsList){
    // вывод в файл
    std::ofstream output_file_1a{"1a.txt"};
    std::ofstream output_file_1b{"1b.txt"};
    std::ofstream output_file_2a{"2a.txt"};
    std::ofstream output_file_2b{"2b.txt"};
    if (output_file_1a.is_open() && output_file_1b.is_open() && output_file_2a.is_open() && output_file_2b.is_open()){
        print_1a(output_file_1a, groupsList);
        print_1b(output_file_1b, groupsList);
        print_2a(output_file_2a, groupsList);
        print_2b(output_file_2b, groupsList);
    }
    else {
        std::cout << "Error: Can`t create/reopen output file \"1a.txt\"\n";
        exit(-1);
    }
}

void print_1a(std::ostream &out, GroupList list){
    out << "Task 1a:\n";
    int index = 1;
    nodeGroup *groupPtr = list.head;
    while (groupPtr != nullptr){
        nodeStudent* studPtr = groupPtr->studentListHead;
        while (studPtr != nullptr){
            if (isStudentExcellentAndContract(studPtr->info)){
                out << "    " << index << ") " << groupPtr->groupNumber << " " << studPtr->info.FIO << "\n";
                index++;
            }
            studPtr = studPtr->next;
        }
        groupPtr = groupPtr->next;
    }
    out << "\n\n";
}

bool isStudentExcellentAndContract(StudentInfo info){
    if (info.type == FinanceType::CONTRACT){
        for (int i = 0; i < 5; i++)
            if (info.marks[i] != 5)
                return false;
        return true;
    }
    else
        return false;
}

bool isStudentLooser(StudentInfo info){
    for (int i = 0; i < 5; i++)
        if (info.marks[i] == 2)
            return true;
    return false;
}

void print_1b(std::ostream &out, GroupList list){
    out << "Task 2a:\n";
    float averageMark = (float)list.sumMark / (float)list.sumStudents;
    int index = 1;
    nodeGroup *groupPtr = list.head;
    while (groupPtr != nullptr){
        nodeStudent* studPtr = groupPtr->studentListHead;
        while (studPtr != nullptr){
            if (getStudentAverage(studPtr->info) < getGroupsAverage(list)){
                out << "    " << index << ") " << groupPtr->groupNumber << " " << studPtr->info.FIO << "\n";
                index++;
            }
            studPtr = studPtr->next;
        }
        groupPtr = groupPtr->next;
    }
    out << "\n\n";
}

void addMarksToStatistics(GroupList* list, StudentInfo newStudent){
    list->sumMark += getStudentAverage(newStudent);
    list->sumStudents++;
}

// void printList_2(std::ostream &out, GroupList list){
//     nodeGroup *groupPtr = list.head;
//     out << "|----------------|\n"
//               << "| Reg |  People  |\n"; 
//     while (groupPtr != nullptr){
//         int people = 0;
//         nodeStudent* studPtr = groupPtr->studentListHead;
//         while (studPtr != nullptr){
//             people += studPtr->info.populationNum;
//             studPtr = studPtr->next;
//         }
//         out << "|" << std::setw(5) << groupPtr->groupNumber
//             << "|" << std::setw(10) << people << "|\n"; 
//         groupPtr = groupPtr->next;
//     }
//     out << "|----------------|\n";
// }


void print_2a(std::ostream &out, GroupList list){
    IndexList indexList;

    nodeGroup* currGroup = list.head;
    while (currGroup != nullptr){
        createNumberStudentsIndex(&indexList, currGroup);
        currGroup = currGroup->next;
    }

    IndexNode* currIndex = indexList.head;
    out << "Task 2a:\n";
    out << "\n|---------------------------------------------|"
        << "\n| Group | Students # | Schoolarship | Payment |"
        << "\n|---------------------------------------------|";
    while (currIndex != nullptr){
        int schoolarship = getGroupSchoolarship(currIndex->dataPtr);
        int payment = getGroupPayment(currIndex->dataPtr);
        out << "\n|" << std::setw(6) << currIndex->dataPtr->groupNumber
            <<  " |" << std::setw(7) << currIndex->dataPtr->studentsNumber << "    "
            <<  " |" << std::setw(9) << schoolarship << "    "
            <<  " |" << std::setw(7) << payment << "  |";
        currIndex = currIndex->next;
    }
    out << "\n|---------------------------------------------|";
    std::cout << "\n\n";
    clearNodeIndex(indexList.head);
}

void createNumberStudentsIndex(IndexList* list, nodeGroup* node){
    IndexNode* newNode = new IndexNode; // создаем новый узел - индекс
    newNode->dataPtr = node;
    if (list->head == nullptr){ // если спииска неt, то создаем его
        createIndexList(list, newNode);
        return;
    }
    // общий случай: добавляем в уже существующий список. В цикле будем искать подходящий узел:
    IndexNode* curr = list->head; // делаем первый узел списка текущим
    while (true) { // пока не вставим узел
        if (curr == nullptr){ // дошли до последнего, т.е. текущий пуст заведомо больше всех остальных
            insertAfterLast(list, curr, newNode); // вставляем после текущего - в конец
            break; // выходим из цикла
        }
        else 
        if (node->groupNumber >= curr->dataPtr->groupNumber){ // если номер узла больше  // вставляем новый узел перед текущим
            insertBeforeCurrent(list, curr, newNode); // вставляем перед текущим
            break; // выходим из цикла
        }
        curr = curr->next; // а следующий в списке - текущим
    }
}

void print_2b(std::ostream &out, GroupList list){
    IndexList indexList;

    nodeGroup* currGroup = list.head;
    while (currGroup != nullptr){
        createLooserStudentsIndex(&indexList, currGroup);
        currGroup = currGroup->next;
    }

    IndexNode* currIndex = indexList.head;
    out << "Task 2b:\n";
    out << "\n|-----------------------------------|"
        << "\n| Group | Loosers % | on contract # |"
        << "\n|-----------------------------------|";
    while (currIndex != nullptr){
        float percent = (float)currIndex->dataPtr->losersNumber / (float)currIndex->dataPtr->studentsNumber * 100;
        int contract = getContractStudentsNumber(currIndex->dataPtr);
        out << "\n|" << std::setw(6) << currIndex->dataPtr->groupNumber
            <<  " |" << std::setw(9) << std::setprecision(4) << percent << " "
            <<  " |" << std::setw(9) << contract << "      |";
        currIndex = currIndex->next;
    }
    out << "\n|-----------------------------------|";
    std::cout << "\n\n";
    clearNodeIndex(indexList.head);
}

void createLooserStudentsIndex(IndexList* list, nodeGroup* node){
    IndexNode* newNode = new IndexNode; // создаем новый узел - индекс
    newNode->dataPtr = node;
    if (list->head == nullptr){ // если спииска неt, то создаем его
        createIndexList(list, newNode);
        return;
    }
    // общий случай: добавляем в уже существующий список. В цикле будем искать подходящий узел:
    IndexNode* curr = list->head; // делаем первый узел списка текущим
    while (true) { // пока не вставим узел
        if (curr == nullptr){ // дошли до последнего, т.е. текущий пуст заведомо больше всех остальных
            insertAfterLast(list, curr, newNode); // вставляем после текущего - в конец
            break; // выходим из цикла
        }
        else {
            float nodePercent = (float)node->losersNumber / (float)node->studentsNumber;
            float currPercent = (float)curr->dataPtr->losersNumber / (float)curr->dataPtr->studentsNumber;
            if (nodePercent >= currPercent){ // если номер узла больше  // вставляем новый узел перед текущим
                insertBeforeCurrent(list, curr, newNode); // вставляем перед текущим
                break; // выходим из цикла
            }
        }
        curr = curr->next; // а следующий в списке - текущим
    }
}

void createIndexList(IndexList* list, IndexNode* node){
    list->head = node; // делаем новый узел головой списка (первым элементом)
    list->last = node; // и так же последним элементом
    return;
}

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

void clearNodeGroup(nodeGroup* ptrNode){
    if (ptrNode == nullptr)
        return;
    clearNodeStudent(ptrNode->studentListHead);
    nodeGroup* next = ptrNode->next;
    delete ptrNode;
    return clearNodeGroup(next);
}

void clearNodeStudent(nodeStudent* ptr){
    if (ptr == nullptr)
        return;
    delete [] ptr->info.FIO;
    nodeStudent* next = ptr->next;
    delete ptr;
    return clearNodeStudent(next);
}

void clearNodeIndex(IndexNode* ptr){
    if (ptr == nullptr)
        return;
    IndexNode* next = ptr->next;
    delete ptr;
    return clearNodeIndex(next);
}