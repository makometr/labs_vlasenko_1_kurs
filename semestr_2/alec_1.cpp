#include <iostream>
#include <fstream>
#include <string>
#include <iomanip>
#include <cstring>

const int work_id_size = 10;

struct ProcessInfo {
    int process_id;
    char work_id[work_id_size]{};
    int work_duration;
    int time_end;
};

struct arrayProc {
    ProcessInfo* array = nullptr;
    int size = 0;
};

struct ProcessParametr {
    int Process;
    int works;
};

struct arrayParam {
    ProcessParametr* array = nullptr;
    int size = 0;
};

ProcessInfo readRecord(std::istream &in);
void addProcess(arrayProc* arrayData, ProcessInfo newProcess);
int isProcessExist(arrayParam *params, int process);
void countParametres(arrayParam *params, arrayProc Processes);

void sortProcesses(arrayProc *processes);
void sortParametres(arrayParam *params);

void printProcesses(std::ostream &out, arrayProc arrayData);
void printParametres(std::ostream &out, arrayParam params);

void clearParams(arrayParam *Params);
void clearProcesses(arrayProc *Processes);

int main(int argc, char const *argv[])
{
    std::ifstream inputFile("data_1.txt");
    if (!inputFile.is_open()){
        std::cout << "Error! No such file in folder!\n";
        exit(-1);
    }

    arrayProc Processes;
    while (!inputFile.eof()){ // считываем данные
        ProcessInfo readedProc = readRecord(inputFile);
        addProcess(&Processes, readedProc); // добавляем в массив
        // addProcess(&Processes, readRecord(inputFile)); // можно одной строкой
    }

    // сортируем по возрастанию времени окончания процесса
    sortProcesses(&Processes);

    arrayParam Params;
    countParametres(&Params, Processes); // подсчитываем кол-во уникальных процессов и работы на них
    sortParametres(&Params); // сортируем парметры по номеру процесса

    // вывод на консоль
    printParametres(std::cout, Params);
    printProcesses(std::cout, Processes);    
    // вывод в файл
    std::ofstream output_file{"ans.txt"};
    if (output_file.is_open()){
        printParametres(output_file, Params);
        printProcesses(output_file, Processes);
    }
    else {
        std::cout << "Error: Can`t create/reopen output file \"ans.txt\"\n";
        exit(-1);
    }
    clearParams(&Params);
    clearProcesses(&Processes);
    return 0;
}

ProcessInfo readRecord(std::istream &in) {
    // Вариант 2
    // считываем первую цифру
    int id;
    in >> id;

    // В буфер считываем промежуточную строку между цифрами в строке данных
    char buffer[work_id_size]{}; // {} вызывает для каждого элемента дефлотный конструктор: для char - \0, иначе будет мусор
    char symbol; // в эту переменную будем записывать считанный символ
    int characters_readed = 0;

    in.get(); // пропускаем пробел
    while (in.get(symbol)){
        if (isspace(symbol)) // если прочитанный символ - пробел
            break; // выходим из цикла посимвольного чтения
        else {
            characters_readed++;
            if (characters_readed > work_id_size){
                std::cout << "Error: Work ID: \""  << "\" is more then 10 symbols!\n";
                exit(-1);
            }
            else {
                buffer[characters_readed-1] = symbol; // записываем наш символ в буфер, возвращаемся в цикл
            }
        }
    }
    // считываем второе число в строке
    int work_duration;
    in >> work_duration;

    // создаем объект и заполянем поля
    ProcessInfo newRecord;
    newRecord.process_id = id;
    newRecord.work_duration = work_duration;
    newRecord.time_end = work_duration;
    std::strcpy(newRecord.work_id, buffer);
    return newRecord;
}

void addProcess(arrayProc* arrayData, ProcessInfo newProcess){
    ProcessInfo* newArray = new ProcessInfo[arrayData->size+1]; //выделение памяти под на 
    if (arrayData->array != nullptr){ // если массив до этого уже был, то копируем
        for (int i = 0; i < arrayData->size; i++)
            newArray[i] = arrayData->array[i]; // копирование старого массива в новый 
    }
    newArray[arrayData->size] = newProcess; //запись новой ячейки в новый массив 
    arrayData->size++; //увеличиваем счётчик длины
    delete [] arrayData->array; // удляем старый массив
    arrayData->array = newArray; 

    //ищем элемент в массиве с processID
    for (int i = arrayData->size-2; i >= 0; i--){
        if (arrayData->array[i].process_id == newProcess.process_id){ // если такой процесс уже есть в массиве
            arrayData->array[arrayData->size-1].time_end += arrayData->array[i].time_end; // прибавляем время окончания предыдущего 
        }
    }
}

int isProcessExist(arrayParam *params, int process){
    for (int i = 0; i < params->size; i++){
        if (params->array[i].Process == process)
            return i;
    }
    return -1;
}


void countParametres(arrayParam *params, arrayProc Processes){
    for (int i = 0; i < Processes.size; i++){
        int index = isProcessExist(params, Processes.array[i].process_id);
        if (index != -1){ // если такой id уже есть
            params->array[index].works++;
        }
        else { // такого id ещё нет - создаем новый элемент массива
            ProcessParametr* newArray = new ProcessParametr[params->size+1]; //выделение памяти под на 
            if (params->array != nullptr){ // если массив до этого уже был, то копируем
                for (int i = 0; i < params->size; i++)
                    newArray[i] = params->array[i]; // копирование старого массива в новый 
            }
            newArray[params->size].Process = Processes.array[i].process_id;
            newArray[params->size].works = 1;
            params->size++; //увеличиваем счётчик длины
            delete [] params->array; // удаляем старый массив
            params->array = newArray; 
        }
    }
}

void printProcesses(std::ostream &out, arrayProc arrayData){
    out << "\n\t Table of times end\n";
    out << "   | ProcNum | Work-Id    | Work-Duration | Time-Ended |\n";
    for (size_t i = 0; i < arrayData.size; i++){
        out << i+1 << ") | "
                  << std::left << std::setw(8) << arrayData.array[i].process_id << "| "
                  << std::left << std::setw(work_id_size+1) << arrayData.array[i].work_id << "| "
                  << std::left << std::setw(14) << arrayData.array[i].work_duration << "| "
                  << std::left << std::setw(11) << arrayData.array[i].time_end << "|\n";
    }
}

void printParametres(std::ostream &out, arrayParam params){
    out << "\n\tParametres of processes\n";
    out << "Number of processes: " << params.size << "\n";
    for (int i = 0; i < params.size; i++){
        out << "Process ID: " << std::setw(2) << params.array[i].Process  << " "
                  << "Works: " << std::setw(3) << params.array[i].works << "\n";
    }
}

void sortProcesses(arrayProc *Processes){
    for (int i = 0; i < Processes->size - 1; i++) {
        for (int j = 0; j < Processes->size - i - 1; j++) {
            if (Processes->array[j].time_end > Processes->array[j + 1].time_end) {
                ProcessInfo temp = Processes->array[j];
                Processes->array[j] = Processes->array[j + 1];
                Processes->array[j + 1] = temp;
            }
        }
    }
}

void sortParametres(arrayParam *params){
    for (int i = 0; i < params->size - 1; i++) {
        for (int j = 0; j < params->size - i - 1; j++) {
            if (params->array[j].Process > params->array[j + 1].Process) {
                ProcessParametr temp = params->array[j];
                params->array[j] = params->array[j + 1];
                params->array[j + 1] = temp;
            }
        }
    }
}

void clearParams(arrayParam *Params){
    delete [] Params->array;
}

void clearProcesses(arrayProc *Processes){
    delete [] Processes->array;
}