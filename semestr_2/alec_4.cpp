#include <iostream>

struct DataNode { // узел списка для хранения данных
    DataNode* next = nullptr;
    int number;
    char character;
};

struct IndexNode { // узел списка для индексирования по числам
    IndexNode* next = nullptr;
    IndexNode* prev = nullptr;
    DataNode* dataPtr = nullptr;
};

struct DataList { // структура для работы cо списком данных
    DataNode* head = nullptr;
    DataNode* last = nullptr;
};

struct IndexList {
    IndexNode* head = nullptr;
    IndexNode* last = nullptr;
};

DataNode* readData();
void createIndexList(IndexList* list, IndexNode* node);
void createNumberIndex(IndexList* list, DataNode* node);
void createCharacterIndex(IndexList* list, DataNode* node);

void insertDataNode(DataList* list, DataNode* newNode);
void insertAfterLast(IndexList* list, IndexNode* curr, IndexNode* newNode);
void insertBeforeCurrent(IndexList* list, IndexNode* curr, IndexNode* newNode);

void printIndex(IndexList list);
void printData(DataList list);

int main(){
    DataList dataList; // список данных
    IndexList characterIndexList; // индексированный список по числам
    IndexList numberIndexList; // индексированный список по буквам

    int num = 0;
    int sum = 0;
    do {
        sum += num;
        std::cout << "Сколько хотите ввести элементов число-буква? (Уже введено: " << sum << ")\n";
        std::cout << num << std::endl;
        std::cin >> num;
        for (int i = 0; i < num; i++){
            DataNode* newDataNode = readData();
            insertDataNode(&dataList, newDataNode);
            createNumberIndex(&numberIndexList, newDataNode);
            createCharacterIndex(&characterIndexList, newDataNode);
        }
    } while (num > 0);

    char ans;
    do {
        std::cout << "Выберите порядок вывода (1 - возрастание ключей, 2 - алфавитный, 3 - конец):\n";
        std::cin >> ans;
        std::cout << "ans: " << ans << "\n";
        if (ans == '1') printIndex(numberIndexList);
        if (ans == '2') printIndex(characterIndexList);
    } while (ans == '1' || ans == '2');
}

DataNode* readData(){
    int number;
    char character;
    std::cin >> number >> character;
    DataNode* newNode = new DataNode;
    newNode->character = character;
    newNode->number = number;
    return newNode;
}

void insertDataNode(DataList* list, DataNode* newNode){
    if (list->head == nullptr){
        list->head = newNode;
        list->last = list->head;
    }
    else {
        list->last->next = newNode;
        list->last = newNode;
    }
}

void createNumberIndex(IndexList* list, DataNode* node){
    IndexNode* newNode = new IndexNode; // создаем новый узел - индекс
    newNode->dataPtr = node; // запоминаем данные
    if (list->head == nullptr){ // если спииска неt, то создаем его
        createIndexList(list, newNode);
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
        if (node->number <= curr->dataPtr->number){ // если номер узла больше  // вставляем новый узел перед текущим
            insertBeforeCurrent(list, curr, newNode); // вставляем перед текущим
            break; // выходим из цикла
        }
        curr = curr->next; // а следующий в списке - текущим
    }
}

void createCharacterIndex(IndexList* list, DataNode* node){
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
        if ((int)node->character <= (int)curr->dataPtr->character){ // если номер узла больше  // вставляем новый узел перед текущим
            insertBeforeCurrent(list, curr, newNode); // вставляем перед текущим
            break; // выходим из цикла
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

void printIndex(IndexList list){
    auto ptr = list.head;
    while (ptr){
        std::cout << "(" << ptr->dataPtr->number << ", " << ptr->dataPtr->character << ") ";
        ptr = ptr->next;
    }
    std::cout << "\n";    
}

void printData(DataList list){
    auto ptr = list.head;
    while (ptr){
        std::cout << "(" << ptr->number << ", " << ptr->character << ") ";
        ptr = ptr->next;
    }
    std::cout << "\n";
}