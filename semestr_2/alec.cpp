#include <iostream>
#include <fstream>
#include <string>
#include <iomanip>
#include <map>
#include <vector>
#include <algorithm>
#include <cstring>
#include <string>
#include <stdlib.h>

constexpr size_t work_id_size = 10;

struct ProcessInfo {
    int process_id;
    char work_id[work_id_size]{};
    int work_duration;
    int time_end;

    ProcessInfo(int p_id, char* w_id, int w_d): process_id(p_id), work_duration(w_d), time_end(w_d) {
        std::strcpy(work_id, w_id);
    };

    // из метода класса можно сдеать прост функцию
    // но тогда ещё придётся передавать в качестве аргумента экземпляр этого класса
    // и вызывать вот так: print(std::cout, process);
    // вместо: process.print(std::cout);
    void print(std::ostream &out){
        out << "ID: " << process_id << "\n"
            << "Work_ID: " << work_id << "\n"
            << "Work Duration: " << work_duration << "\n\n";
    }

    // // продвинутый уровень: перегружем оператор вывода для нашего типа (часть 1)
    // friend std::ostream& operator<<(std::ostream &out, const ProcessInfo &proecess);
};

// // продвинутый уровень: перегружем оператор вывода для нашего типа (часть 2)
// std::ostream& operator<<(std::ostream &out, const ProcessInfo &proecess){
//     out << "ID: " << proecess.process_id << "\n"
//     << "Work_ID: " << proecess.work_id << "\n"
//     << "Work Duration: " << proecess.work_duration << "\n\n";
// }

ProcessInfo readRecord(std::istream &in);
void printAnswer(std::ostream &out, const std::vector <ProcessInfo> &Processes);

int main(int argc, char const *argv[])
{
    std::ifstream inputFile("data.txt");
    if (!inputFile.is_open()){
        std::cout << "Error! No such file in folder!\n";
        exit(-1);
    }

    std::vector <ProcessInfo> Processes;
    std::map <int, std::pair<int,int>> UniqueProcesses;
    // Ассоциативный контейнер (ключ-значение), которые хранить уникальные значения по ключу
    // в нашем случае ключ - это номер процесса (process_id),
    // а значение - пара значений: first - кол-во работ в процессе, second - суммарное время работ процесса (сумма work_duration)
    // Иначе приёдтся реализовывать какую-нибудь функцию, которая ищет в массиве уникальные значения, складывает, запоминает и так далее


    while (!inputFile.eof()){
        Processes.emplace_back(readRecord(inputFile));
        // почти(чуть лучше) то же самое, что и:
        // ProcessInfo process =  readRecord(data_file);
        // Processes.push_back(process);

        Processes.back().print(std::cout); // если просто print()
        // std::cout << Processes.back(); // В случае использоваия перегруженного оператора вывода оставить это

        // result - пара значений. Первый - указатель на вставленный элемент, второй - логические тип "был ли вставлен элемент"
        // мы это проверяем далее, так как мы не можем вставить два одинаковых ключа
        // чуть понятнее написано в следующем абзаце
        auto result = UniqueProcesses.insert({Processes.back().process_id, {1, Processes.back().work_duration}});
        if (result.second == false){ // если процесс с таким id уже есть
            Processes.back().time_end += result.first->second.second; // добавляем текущий последний момент освобождения
            result.first->second.first++; // увеличиваем кол-во работ в процессе
            result.first->second.second += Processes.back().work_duration; // отодвигаем момент окончания работ на процессоре (суммируем    )
        }

        // Если что, это фича из c++17, так что ключ компиляции в настройках компилятора указывать следующий: -std=c++17
        // auto [insertedElemPtr, elemNotExist] = UniqueProcesses.insert({Processes.back().process_id, {1, Processes.back().work_duration}});
        // if (elemNotExist == false){ // если процесс с таким id уже есть в контейнере
        //     Processes.back().time_end += insertedElemPtr->second.second; // добавляем текущий последний момент освобождения
        //     insertedElemPtr->second.first++; // увеличиваем кол-во работ в процессе
        //     insertedElemPtr->second.second += Processes.back().work_duration; // отодвигаем момент окончания работ на процессоре
        // }
    }

    std::cout << "Processes number: " << UniqueProcesses.size() << "\n";
    std::cout << "Process - works number and summary time of work:\n";
    // так как массив ассоциативный (т.е. реализация не имеет непрервного участка в памяти)
    // то без итераторов не обойтись:( По простому: итератор это указатель на элемент контейнера
    // это сокращёная запись (почти) варианта ниже. const и & можно убрать.
    for (const auto &elem : UniqueProcesses)
        std::cout << elem.first << " - " << elem.second.first << " works and " << elem.second.second << " summary\n";
    // // по-другому можно написать:
    // for (auto it = UniqueProcesses.begin(); it != UniqueProcesses.end(); it++)
    //     std::cout << it->first << " - " << it->second.first << " works and " << it->second.second << " summary\n";
    // // т.е. мы берём итератор на первый элемент и проходим все элементы в контейнере, пока не достигнем последнего

    // производим сортировку массива с помощью лямбда-функции (анонимного функтора)
    // проще говоря, третьим аргументм после итераторов на начала и конец контейнера передаём функцию,
    // которая сравнивает два элемента между собой
    // если поменять < на >, то контейнер отсортируется обратным образом
    // const и & можно убрать
    std::sort(Processes.begin(), Processes.end(), [](const ProcessInfo &lhs, const ProcessInfo &rhs){
        return lhs.time_end < rhs.time_end;
    });

    // вывод на консоль
    printAnswer(std::cout, Processes);
    
    // вывод в файл
    std::ofstream output_file{"ans.txt"};
    if (output_file.is_open())
        printAnswer(output_file, Processes);
    else {
        std::cout << "Error: Can`t create/reopen output file \"ans.txt\"\n";
        exit(-1);
    }

    return 0;
}

void printAnswer(std::ostream &out, const std::vector <ProcessInfo> &Processes){
    out << "   | ProcNum | Work-Id    | Work-Duration | Time-Ended |\n";
    for (size_t i = 0; i < Processes.size(); i++){
        out << i+1 << ") | "
                  << std::left << std::setw(8) << Processes[i].process_id << "| "
                  << std::left << std::setw(work_id_size+1) << Processes[i].work_id << "| "
                  << std::left << std::setw(14) << Processes[i].work_duration << "| "
                  << std::left << std::setw(11) << Processes[i].time_end << "|\n";
    }
}


ProcessInfo readRecord(std::istream &in) {
    // Вариант 1
    // считываем первую цифру
    int id;
    in >> id;

    std::string tmp_work_id; // считываем стркоу символов в string
    in >> tmp_work_id;
    if (tmp_work_id.size() > work_id_size){ // проверяем длину
        std::cout << "Error: Work ID: \"" << tmp_work_id << "\" is more then 10 symbols!\n";
        exit(-1);
    }
    char buffer[10]{}; // {} вызывает для каждого элемента дефлотный конструктор: для char - пробел, иначе будет мусор
    std::strcpy(buffer, tmp_work_id.c_str()); // копируем си-строку из string себе в выделенную только что память

    // считывем второе число в строке
    int work_duration;
    in >> work_duration;


    // // Вариант 2
    // // считываем первую цифру
    // int id;
    // in >> id;

    // // В буфер считываем промежуточную строку между цифрами в строке данных
    // char buffer[work_id_size]{}; // {} вызывает для каждого элемента дефлотный конструктор: для char - пробел, иначе будет мусор
    // char symbol; // в эту переменную будем записывать считанный символ
    // int characters_readed = 0;

    // in.get(); // пропускаем пробел, можно сделать проверку на то, что это был пробел
    // while (in.get(symbol)){
    //     // std::cout << "Readed: " << symbol << "\n";
    //     if (isspace(symbol)){
    //         break; // если встретили пробел, выходим из цикла посимвольного чтения. Можно сделать флаг
    //         // получится что-то вроде этого:
    //         // while (flag / true /){
    //             // in.get(c);
    //             // ...
    //             // if (/пробел/) -> flag = false;
    //         // }
    //     }
    //     else {
    //         characters_readed++;
    //         if (characters_readed > work_id_size){
    //             std::cout << "Error: Work ID: \""  << "\" is more then 10 symbols!\n";
    //             exit(EXIT_FAILURE);
    //         }
    //         else {
    //             buffer[characters_readed-1] = symbol; // записываем наш символ в буффер
    //         }
    //     }
    // }

    // // считывем второе число в строке
    // int work_duration;
    // in >> work_duration;

    // // Варинт 3
    // // Считываем всю строку в буфер и затем парсим её (делим на части)
    // // подробнее зугуглить strtok
    // size_t bufferSize = 32;
    // char tmp[bufferSize] = {};
    // in.getline(tmp, bufferSize);

    // char *lexeme = nullptr;

    // lexeme = std::strtok(tmp, " ");
    // int id = atoi(lexeme);

    // lexeme = std::strtok(NULL," ");
    // char buffer[work_id_size]{}; // {} вызывает для каждого элемента дефлотный конструктор: для char - пробел, иначе будет мусор
    // std::strcpy(buffer, lexeme);

    // lexeme = std::strtok(NULL," ");
    // int work_duration = atoi(lexeme);

    // Конец разветвления вариантов считывания

    return {id, buffer, work_duration}; // эквивалентно двум строкам ниже
    // ProcessInfo newRecord(id, buffer, work_duration); // вызываем конструктор ProcessInfo
    // return newRecord;
}

// @makometr