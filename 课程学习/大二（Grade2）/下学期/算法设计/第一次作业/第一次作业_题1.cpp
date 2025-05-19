#include <iostream>
#include <vector>

void primeFactors(int num, int divisor, std::vector<int>& result) {
    if (num == 1) {
        return;
    }

    if (num % divisor == 0) {
        result.push_back(divisor);
        primeFactors(num / divisor, divisor, result);
    }
    else {
        primeFactors(num, divisor + 1, result);
    }
}

std::vector<int> decomposePrimeFactors(int num) {
    std::vector<int> result;
    primeFactors(num, 2, result);
    return result;
}

int main() {
    int num;

    std::cin >> num;

    if (num <= 1) {
        std::cout << "请输入大于1的正整数！" << std::endl;
        return 1;
    }

    std::vector<int> factors = decomposePrimeFactors(num);

    for (int factor : factors) {
        std::cout << factor << " ";
    }
    std::cout << std::endl;

    return 0;
}