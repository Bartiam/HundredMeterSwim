#include <iostream>
#include <thread>
#include <string>
#include <vector>
#include <ctime>
#include <mutex>

// Проверка на корректность ввода имени;
bool is_correct_input_name(const std::string& name)
{
	if (name[0] < 'A' || name[0] > 'Z') return false;

	for (int i = 1; i < name.length(); ++i)
		if (name[i] < 'a' || name[i] > 'z') return false;

	return true;
}
//Вектор пар для сохранения имени и времени заплыва;
std::vector<std::pair<int, std::string>> swimCompletionTime;
std::mutex mtx;
//Функция симуляции заплыва;
void a_hundred_meter_swim(const std::string& name, const double& speed, const std::time_t& currentTime)
{
	const double SWIMLENGTH = 100;
	double currentPosition = 0;

	while (currentPosition < SWIMLENGTH)
	{
		std::this_thread::sleep_for(std::chrono::seconds(1));
		currentPosition += speed;
		mtx.lock();
		if (currentPosition > 100) currentPosition = 100;
		std::this_thread::sleep_for(std::chrono::milliseconds(1));
		std::cout << "Swimmer: " << name << ". The swimmer swam: " << currentPosition << std::endl;
		mtx.unlock();
	}

	mtx.lock();
	std::time_t finishTime = std::time(NULL);
	swimCompletionTime.push_back(std::make_pair((finishTime - currentTime), name));
	mtx.unlock();
}
// Функция вывода таблицы и победителя;
void print_table()
{
	system("cls");
	for (int i = 0; i < swimCompletionTime.size(); ++i)
	{
		std::cout << "Swimmer's name: " << swimCompletionTime[i].second <<
			"\nTime spent on the swim(in sec): " << swimCompletionTime[i].first << std::endl;
	}

	std::cout << "The winner is: " << swimCompletionTime[0].second << "!" << std::endl;
}

int main()
{
	// Вектор пловцов;
	std::vector<std::pair<std::string, double>> swimmers(6);

	//Цикл для ввода данных;
	for (int i = 0; i < swimmers.size(); ++i)
	{
		std::cout << "Enter the swimmer's name: ";
		std::cin >> swimmers[i].first;

		if (!is_correct_input_name(swimmers[i].first))
		{
			std::cerr << "Error! Incorrect input name. Try again. " << std::endl;
			--i;
			continue;
		}

		while (true)
		{
			std::cout << "Enter the swimming speed: ";
			std::cin >> swimmers[i].second;

			if (swimmers[i].second <= 0.000001) std::cerr << "Error! Incorrect speed data. Try again. " << std::endl;
			else break;
		}
	}
	// Удаление ввода данных пользователем из консоли;
	system("cls");
	// Вывод времени начала заплыва;
	std::time_t currentTime = std::time(NULL);
	std::tm startSwim = *std::localtime(&currentTime);
	std::cout << "Start time of the swim: \n" << std::asctime(&startSwim);

	// Создание 6 потоков;
	std::thread firstSwimmer(a_hundred_meter_swim, swimmers[0].first, swimmers[0].second, currentTime);
	std::thread secondSwimmer(a_hundred_meter_swim, swimmers[1].first, swimmers[1].second, currentTime);
	std::thread thirdSwimmer(a_hundred_meter_swim, swimmers[2].first, swimmers[2].second, currentTime);
	std::thread fourthSwimmer(a_hundred_meter_swim, swimmers[3].first, swimmers[3].second, currentTime);
	std::thread fifthSwimmer(a_hundred_meter_swim, swimmers[4].first, swimmers[4].second, currentTime);
	std::thread sixthSwimmer(a_hundred_meter_swim, swimmers[5].first, swimmers[5].second, currentTime);
	
	firstSwimmer.join();
	secondSwimmer.join();
	thirdSwimmer.join();
	fourthSwimmer.join();
	fifthSwimmer.join();
	sixthSwimmer.join();
	// Вызов таблицы по окончанию заплыва;
	print_table();

	return 0;
}
