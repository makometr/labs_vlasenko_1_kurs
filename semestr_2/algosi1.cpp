#include <iostream>
#include <queue>
#include <algorithm>

class Table
{
private:
	int** Board;
	int size;
	//int b;
	int square_num;

	//Конструктор
public:
	explicit Table(int N)
		: size(N), square_num(1), Board(new int*[N])
	{
		for (int i = 0; i < size; i++)
		{
			Board[i] = new int[size];
			for (int j = 0; j < size; j++)
			{
				Board[i][j] = 0;

			}
		}
	}
	//Констркуктор коп.
	Table(Table const & table)
		: size(table.size), square_num(table.square_num), Board(new int*[table.size])
	{
		for (int i = 0; i < size; i++)
		{
			Board[i] = new int[size];
			for (int j = 0; j < size; j++)
			{
				Board[i][j] = table.Board[i][j];

			}
		}
	}
	//Оператор присваивания
	Table & operator = (Table const& table)
	{
		if (this != &table)
		{
			size = table.size;
			//b = table.b;
			square_num = table.square_num;
			for (int i = 0; i < size; i++)
			{
				delete[] Board[i];
			}
			delete[] Board;
			Board = new int*[size];
			for (int i = 0; i < size; i++)
			{
				Board[i] = new int[size];
				for (int j = 0; j < size; j++)
				{
					Board[i][j] = table.Board[i][j];

				}
			}
		}
	}
	int getcolor(int i, int j)
	{
		return Board[i][j];
	}

	//счет квадратов
	void resquare(int i)
	{
		square_num += i;
	}

	//оптимизация при кратности 2м,3м и 5ти
	void resize(int N)
	{
		//this->b = N;
		this->size = N;
	}

	/*int getb()
	{
		return b;
	}*/
	int getsize()
	{
		return size;
	}
	int getsquarenum()
	{
		return square_num;
	}
	void printBoard()
	{
		for (int i = 0; i < size; i++)
		{
			for (int j = 0; j < size; j++)
				std::cout << Board[i][j] << " ";
			std::cout << std::endl;
		}

	}

	//удаление квадратов
	void removeSquare()
	{

		for (int i = 0; i < size; i++)
		{
			for (int j = 0; j < size; j++)
			{
				if (Board[i][j] == square_num)
					Board[i][j] = 0;
			}
		}
	}

	//проверка, можно ли поместить квадрат
	bool setSquare(int x, int y, int size)
	{
		if ((y + size > this->size) || (x + size > this->size))
			return false;

		for (int i = y; i < y + size; i++)
			for (int j = x; j < x + size; j++)
				if (Board[i][j] != 0)
					return false;

		for (int i = y; i < y + size; i++)
			for (int j = x; j < x + size; j++)
				Board[i][j] = square_num;
		return true;
	}

	//поиск пустых клеток
	bool square_is_empty(int* x, int* y)
	{
		for (int i = 0; i < size; i++)
			for (int j = 0; j < size; j++)
				if (Board[i][j] == 0)
				{
					*x = j;
					*y = i;
					return true;
				}
		return false;
	}
	~Table()
	{
		for (int i = 0; i < size; i++)
		{
			delete[] Board[i];
		}
		delete[] Board;
	}
};

void printres(int multi, int ** Result, int BestNumber)
{
	for (int i = 0; i < BestNumber; i++)
		std::cout << Result[i][0] * multi + 1 << " " << Result[i][1] * multi + 1 << " " << Result[i][2] * multi << std::endl;
}

void WriteRes(Table & desk, int BestNumber, int ** Result)
{
	int k;
	int s = 0;
	//std::set<int> set_of_colors;
	std::vector<int> set_of_colors;
	for (int i = 1; i <= BestNumber; i++)
	{
		set_of_colors.push_back(i);
	}
	for (int i = 0; i < desk.getsize(); i++)
		for (int j = 0; j < desk.getsize(); j++)
		{
			if (std::find(set_of_colors.begin(),set_of_colors.end(),desk.getcolor(i, j)) != set_of_colors.end())
			{
				k = j + 1;
				while ((desk.getcolor(i, k) == desk.getcolor(i, j)) && (k < desk.getsize()))
					k++;

				Result[s][0] = i;
				Result[s][1] = j;
				Result[s][2] = k - j;
				s++;
				set_of_colors.erase(std::find(set_of_colors.begin(), set_of_colors.end(), desk.getcolor(i, j)));


			}
		}
}

void FindSolution(Table & desk)
{
	int ** Result = new int*[desk.getsize()*desk.getsize()];
	int BestNumber = desk.getsize()*desk.getsize() + 1;
	for (int i = 0; i < desk.getsize()*desk.getsize(); i++)
	{
		Result[i] = new int[3];
		for (int j = 0; j < 3; j++)
		{
			Result[i][j] = 0;
		}
	}
	int multi = 1;

	// Проверка на кратность
	while ((desk.getsize() % 2 == 0) && (desk.getsize() > 2))
	{
		multi *= 2;
		desk.resize(desk.getsize() / 2);
	}
	while ((desk.getsize() % 3 == 0) && (desk.getsize() > 3))
	{
		multi *= 3;
		desk.resize(desk.getsize() / 3);
	}
	while ((desk.getsize() % 5 == 0) && (desk.getsize() > 5) )
	{
		multi *= 5;
		desk.resize(desk.getsize() / 5);
	}

	std::queue<Table> Queue;
	Queue.push(desk);
	int x = 0;
	int y = 0;
	int count = 1;

	while (!Queue.empty())
	{
		Table S = Queue.front();
		std::cout << "Delete item" << std::endl;
		std::cout << std::endl;
		//S.printBoard();
		Queue.pop();
		if (S.getsquarenum() > BestNumber)
			continue;
		for (int i = (S.getsize() - 1); i >= 1; i--) // Смотрим квадраты по убыванию размера
		{
			S.square_is_empty(&x, &y);
			if (S.setSquare(x, y, i)) // Если можно поставить - ставим
			{
				S.resquare(1);
				if (S.square_is_empty(&x, &y))  // Ищем свободное поле
				{
					std::cout << "put in a queue" << std::endl;
					//S.printBoard();
					std::cout << std::endl;
					Queue.push(S);
				}
				else // Если свободных мест нет
				{
					S.resquare(-1);
					if (S.getsquarenum() == BestNumber)
					{
						count++;
						std::cout << "Found a similar option, now " << count << " solutions" << std::endl;
						//S.printBoard();
						std::cout << std::endl;
					}
					else if (S.getsquarenum() < BestNumber)
					{
						std::cout << "Found a better option with " << S.getsquarenum() << " squares" << std::endl;
						//S.printBoard();
						std::cout << std::endl;
						count = 1;
						BestNumber = S.getsquarenum();
						WriteRes(S, BestNumber, Result);
					}

					S.removeSquare();
					continue;
				}
				S.resquare(-1);
				S.removeSquare();
			}
		}
	}

	std::cout << "Minimum number of squares " << BestNumber << std::endl;
	std::cout << "Number of coverage options " << count << std::endl;
	std::cout << "Coordinates " << std::endl;
	printres(multi, Result, BestNumber);
	for (int i = 0; i < desk.getsize()*desk.getsize(); i++)
	{
		delete[] Result[i];
	}
	delete[] Result;

}

int main()
{
	int N;
	std::cin >> N;

	//оптимизация для четных N
	if (!(N % 2)) {
		std::cout << 4 << std::endl;
		std::cout << 1 << " " << 1 << " " << N / 2 << std::endl;
		std::cout << 1 << " " << N / 2 + 1 << " " << N / 2 << std::endl;
		std::cout << N / 2 + 1 << " " << 1 << " " << N / 2 << std::endl;
		std::cout << N / 2 + 1 << " " << N / 2 + 1 << " " << N / 2 << std::endl;
		return 0;
	}
	Table desk(N);
	FindSolution(desk);
	return 0;
}