#include <iostream>
#include <vector>
#include <algorithm>

// c++17 with stl

double getAverage(const std::vector<int>& nums); // возвращает среднее
double getPositivePercent(const std::vector<int>& nums); // % положительных чисел (от 0 до 1)
std::pair<int, int> getRange(const std::vector<int>& nums); // Диапазон изменения чисел
int getSum2Prev(const std::vector<int>& nums); // Количество чисел равных сумме предыдущих 2-х
int aliquot5InRange(const std::vector<int>& nums); // Количество чисел кратных пяти из диапазона

int main(){
    int num = 0;
    std::vector<int> numbers;

    while (true){
        std::cin >> num;
        if (!num)
            break;
        numbers.push_back(num);
    }

    for (const auto &elem : numbers)
        std::cout << elem << " ";
    std::cout << "Size: " << numbers.size() << "\n";


    
    std::cout << "Average of input numbers: " << getAverage(numbers) << "\n";
    std::cout << "Percent of positive numbers: " << getPositivePercent(numbers) * 100 << "\n";
    {
    auto minmax = getRange(numbers);
    std::cout << "Range of numbers: from " << minmax.first << " to " << minmax.second << "\n";
    }
    std::cout << "The number of numbers equal to the sum of the previous 2: " << getSum2Prev(numbers) << "\n";
    std::cout << "Number of numbers aliquot 5 in range: " << aliquot5InRange(numbers) << std::endl;
    
}


double getAverage(const std::vector<int>& nums){
    double sum = 0;
    for (const auto &elem : nums)
        sum += elem;
    return sum / nums.size();
}

double getPositivePercent(const std::vector<int>& nums){
    double ans = 0;
    for (const auto &elem : nums)
        if (elem > 0)
            ans++;
    return ans / nums.size();   
}

std::pair<int,int> getRange(const std::vector<int>& nums){
    return std::make_pair(*std::min_element(std::begin(nums), std::end(nums)), *std::max_element(std::begin(nums), std::end(nums)));
}

int getSum2Prev(const std::vector<int>& nums){
    int ans = 0;
    if (nums.size() < 3){
        std::cout << "Vector length < 3! Exit.\n";
        return ans;
    }
    int sum = nums[0];
    for (int i = 2; i < nums.size(); i++){
        sum += nums[i-1];
        if (nums[i] == sum)
            ans++;
        sum -= nums[i-2];
    }

    return ans;
}

int aliquot5InRange(const std::vector<int>& nums){
    int ans = 0;
    int first;
    int second;
    std::cout << "Enter 2 numbers for range: ";
    std::cin >> first >> second;
    for (auto it = std::lower_bound(std::begin(nums), std::end(nums), first); it != std::lower_bound(std::begin(nums), std::end(nums), second); it++)
        if (*it % 5 == 0)   
            ans++;
    return ans;
}
