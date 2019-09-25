#include <iostream>
#include <fstream>
#include <string>
#include <iomanip>
#include <cstring>

const int work_id_size = 10;

struct WorkArrayElement { // ячейка массива работ на процессоре
	int processor_id;
    char work_id[work_id_size]{};
    int work_duration;
    int time_end;
};

struct ProcessorArrayElement { // ячейка массива процессоров
	WorkArrayElement* array = nullptr;
	int works = 0; // кол-во работ на процессоре
	int processor_id = 0; // номер процессора
};

struct arrayData { // данные о главом массиве - указатель и кол-во элементов
    ProcessorArrayElement* array = nullptr;
    int size = 0;
};

WorkArrayElement readRecord(std::istream &in);
void addWorkToProcessorsArray(arrayData* arrayData, WorkArrayElement newProcess);
ProcessorArrayElement* findProcessorElementNumber(arrayData *data, int id);
void addWorkToWorksArray(ProcessorArrayElement* position, WorkArrayElement newWork);

void printProcesses(std::ostream &out, arrayData data);
void clearData(arrayData *data);

int main(int argc, char const *argv[])
{
    std::ifstream inputFile("data_1.txt");
    if (!inputFile.is_open()){
        std::cout << "Error! No such file in folder!\n";
        exit(-1);
    }

    arrayData Processors;
    while (!inputFile.eof()){ // считываем данные
        WorkArrayElement readedWork = readRecord(inputFile);
        addWorkToProcessorsArray(&Processors, readedWork); // добавляем в массив
    }
    inputFile.close();  

    // вывод на консоль
    printProcesses(std::cout, Processors);    
    // // вывод в файл
    std::ofstream output_file{"ans_1.txt"};
    if (output_file.is_open()){
        printProcesses(output_file, Processors);
    }
    else {
        std::cout << "Error: Can`t create/reopen output file \"ans_1.txt\"\n";
        exit(-1);
    }
    output_file.close();
    clearData(&Processors);
    return 0;
}

WorkArrayElement readRecord(std::istream &in) {
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
    WorkArrayElement newRecord;
    newRecord.processor_id = id;
    newRecord.work_duration = work_duration;
    newRecord.time_end = work_duration;
    std::strcpy(newRecord.work_id, buffer);
    return newRecord;
}

void addWorkToProcessorsArray(arrayData* arrayData, WorkArrayElement newWork){
    // есть ли уже такой процессор в массиве процессров?
    ProcessorArrayElement* position = findProcessorElementNumber(arrayData, newWork.processor_id);

    if (position == nullptr){ // нет -> добавляем новый элемент в в массив processorArray
        ProcessorArrayElement* newProcArray = new ProcessorArrayElement[arrayData->size+1]; //выделение памяти под на 
        if (arrayData->array != nullptr){ // если массив до этого уже был, то копируем
            for (int i = 0; i < arrayData->size; i++)
                newProcArray[i] = arrayData->array[i]; // копирование старого массива в новый 
        }
        arrayData->size++; //увеличиваем счётчик длины
        newProcArray[arrayData->size-1].processor_id = newWork.processor_id; //запись новой ячейки в новый массив 
        delete [] arrayData->array; // удаляем старый массив
        arrayData->array = newProcArray;
        position = &(arrayData->array[arrayData->size-1]); // берем адрес нового элемента  массива для последующей вставки
    }
    addWorkToWorksArray(position, newWork);
}

ProcessorArrayElement* findProcessorElementNumber(arrayData *data, int id){
    for (int i = 0; i < data->size; i++){
        if (data->array[i].processor_id == id)
            return data->array;
    }
    return nullptr;
}

void addWorkToWorksArray(ProcessorArrayElement* position, WorkArrayElement newWork){
    WorkArrayElement* newArray = new WorkArrayElement[position->works+1]; //выделение памяти под на 
    if (position->array != nullptr){ // если массив до этого уже был, то копируем
        for (int i = 0; i < position->works; i++)
            newArray[i] = position->array[i]; // копирование старого массива в новый 
    }
    position->works++; //увеличиваем счётчик работ на процессоре
    newArray[position->works-1] = newWork; //запись новой ячейки в новый массив 
    if (position->works > 1) // пересчитываем время окончания работ
        newArray[position->works-1].time_end += newArray[position->works-2].time_end;

    delete [] position->array; // удляем старый массив
    position->array = newArray;
}

void printProcesses(std::ostream &out, arrayData data){
    out << "\n\tParametres of processors\n";
    out << "Number of processors: " << data.size << "\n";
    for (int i = 0; i < data.size; i++){
        out << "Process ID: " << std::setw(2) << data.array[i].processor_id  << " "
                  << "Works: " << std::setw(2) << data.array[i].works << "\n";
    }

    out << "\n\t Table of times end\n";
    out << "   | ProcNum | Work-Id    | Work-Duration | Time-Ended |\n";
    int work_num = 1;
    for (size_t i = 0; i < data.size; i++){
        for (size_t j = 0; j < data.array[i].works; j++){
            out << work_num++ << ") | "
                    << std::left << std::setw(8) << data.array[i].array[j].processor_id << "| "
                    << std::left << std::setw(work_id_size+1) << data.array[i].array[j].work_id << "| "
                    << std::left << std::setw(14) << data.array[i].array[j].work_duration << "| "
                    << std::left << std::setw(11) << data.array[i].array[j].time_end << "|\n";
        }
    }
}

void clearData(arrayData *data){
    for (int i = 0; i < data->size; i++)
        delete [] data->array[i].array;
    delete [] data->array;
    data->array = nullptr;
}