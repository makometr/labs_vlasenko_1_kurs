#include <iostream>

struct BTNode { //  стркутура - узел дерева
    int key; // числа
    BTNode* left = nullptr; // указатель на левоого сына
    BTNode* right = nullptr; // на правого  
};

void addNode(BTNode** position, int key){ // добавляем новый узел с ключом key в позицию
    if (*position == nullptr){ // если позиция свободна (указатель пуст), вставляем
        *position = new BTNode;
        (*position)->key = key;
        return;
    }
    // если дошли до сюда, то рассматриваемая позиция вставки (указатель) не пуста
    // сравниваем ключ текущей позиции и нового элемента
    if (key > (*position)->key) // ключ нового больше
        addNode(&(*position)->right, key); // берем адрес указателя на правый узел текущего узла (позиции)
    if (key < (*position)->key) // ключ нового меньше
        addNode(&(*position)->left, key); // берем адрес указателя на левый узел текущего узла (позиции)
}

void LKP(BTNode* node){ // левое - корень - правое
    // сначала выводим левое поддерево, затем корень, затем правое
    if (node == nullptr)
        return;
    LKP(node->left);  
    std::cout << node->key << " ";
    LKP(node->right);
}

void PKL(BTNode* node){ // аналогично функции выше
    if (node == nullptr)
        return;
    PKL(node->right);
    std::cout << node->key << " ";
    PKL(node->left);  
}

void horizontal(BTNode* node, int size){
    if (node == nullptr) return;
    // для обхода в ширину нам нужна очередь ожидающих обработки узлов
    // в качестве очереди будем использовать массив указателей на узлы дерева и пару переменных-индексов
    BTNode* queue[size]; // собственно массив-очередь узлов для обработки
    int levels[size]; // масссив, в котором хранятся числа: номера уровней обработанных узлов

    int insertIndex = 1; //позиция в массиве, куда будем вставлять очередной элемент (ближайший пустой)
    // начинаем с 1, т.к. нулевой уже занят (строчки ниже)
    int workIndex = 0; // рабочая позиция - номер элемента в массиве, который будет обрабатываться (выводиться)
    queue[workIndex] = node; // инициализация очереди - пихаем вершину дерева в первый элемент
    levels[workIndex] = 0; // d в массив уровней первый уровень у первого узла (головы) - 0

    do {
        std::cout << queue[workIndex]->key << " "; // выводим ключ обрабатываемого узла

        if (queue[workIndex]->left != nullptr){ // добавляем в очередь левого сына
            queue[insertIndex] = queue[workIndex]->left; // в позицию вставки вставляем указатель
            levels[insertIndex] = levels[workIndex] + 1; // m.к. обрабатываем прямого потомка вершины
            insertIndex++; // сдвигаем индекс текущей свободной ячейки
        }
        if (queue[workIndex]->right != nullptr){ // добавляем в очередь правого сына
            queue[insertIndex] = queue[workIndex]->right;
            levels[insertIndex] = levels[workIndex] + 1; // m.к. обрабатываем прямого потомка вершины
            insertIndex++;
        }
        workIndex++; // на каждой итерации цикла будем двигать индекс обрабатываемого элемента
    }
    while (workIndex != insertIndex); // это будет происходить до тех пор, пока обрабатываемый индекс не догонит индекс вставки

    // неочевидная вещь - числа в массиве levels будут выписаны по порядку (это важно)
    int curLevel = 0; // уровень обработки
    int curNodesNum = 0; // кол-во узлов
    std::cout << "\nLevels:\n";
    for (int i = 0; i < size; i++){ // идём в цикле по массиву уровней
        if (levels[i] == curLevel){ // если обрабатываемая ячейка массива содержит текущий обрабатываемый уровень
            curNodesNum++; // то увеличиваем кол-во узлов на текущем уровне
        }
        if (levels[i] != curLevel){ // если числа стало другим (отличаться от текущего уровня)
            std::cout << curLevel << " - " << curNodesNum << std::endl; // выводим обработанный уровень
            curLevel++; // переходим на следующий
            curNodesNum = 1; // запоминаем кол-во узлов (он - первый, поэтому один)
        }
    }
    std::cout << curLevel << " - " << curNodesNum << std::endl; // выводим сами последний обработанный уровень
    std::cout << std::endl;

}

void clearTree(BTNode* node){
    if (node == nullptr)
        return;
    clearTree(node->left);
    clearTree(node->right);
    delete node;
}

int main(){
    BTNode* tree = nullptr;
    int num; // для чтения
    int size = 0; // кол-во узлов
    // предполагаем, что на вход будет подаваться только уникальные числа
    while (std::cin >> num) {
        addNode(&tree, num);
        size++;
    }

    std::cout << "В порядке убывания: ";
    PKL(tree);
    std::cout << std::endl;

    std::cout << "Слева - направо: ";
    LKP(tree);
    std::cout << std::endl;

    std::cout << "Сверху - вниз: ";
    horizontal(tree, size);
    std::cout << std::endl;
    
    clearTree(tree);
}