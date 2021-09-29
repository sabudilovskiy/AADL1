#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <ctime>
using namespace std;

//Compiler version g++ 6.3.0

char getDigit(int num) {
    switch (num)
    {
        case 0:
            return '0';
        case 1:
            return '1';
        case 2:
            return '2';
        case 3:
            return '3';
        case 4:
            return '4';
        case 5:
            return '5';
        case 6:
            return '6';
        case 7:
            return '7';
        case 8:
            return '8';
        case 9:
            return '9';
        default:
            return 0;
    }
}
int getNeed(int index) {
    static int base[3] = { 3,5,7 };
    return base[index];
}
int getOptional(int index){
    static int base[7] = {0,1,2,4,6,8,9};
    return base[index];
}
bool nextPermutation(std::vector<int>& a) {
    int index = a.size() - 2;
    while (index > -1 && a[index] >= a[index + 1]) {
        index--;
    }
    if (index > -1) {
        int min = INT_MAX;
        int min_index;
        for (int i = index + 1; i < a.size(); i++) {
            if (a[i] > a[index] && a[i] < min) {
                min = a[i];
                min_index = i;
            }
        }
        swap(a[index], a[min_index]);
        for (int i = 0; i < a.size() - index - 2; i++) {
            for (int j = index + 1; j < a.size() - 1 - i; j++) {
                if (a[j] > a[j + 1]) swap(a[j], a[j + 1]);
            }
        }
        return true;
    }
    else return false;
}
bool nextComb(std::vector<int>& a, int k){
    int index = a.size() -1 ;
    while (index > -1 && a[index] == k + index - a.size() + 1) index--;
    if (index != -1) {
        a[index]++;
        for (int i = index + 1; i < a.size(); i++) a[i] = a[i-1] + 1;
        return true;
    }
    else return false;
}
string sets_to_number(const vector<int>& optionalIndex,const vector<int>& need, int k) {
    std::string temp;
    for (int i = 0; i < k; i++) temp.push_back('_');
    for (int i = 0; i < 3; i++) temp[need[i]] = getDigit(getNeed(i));
    int j = 0, i = 0;
    while (i < k - 3){
        if (temp[j]=='_'){
            temp[j] = getDigit(getOptional(optionalIndex[i]));
            i++;
        }
        j++;
    }
    return temp;
}
int main()
{
    setlocale(LC_ALL, "Russian");
    int k;
    std::string out;
    std::cout << "Введите k: ";
    std::cin >> out;
    bool error = false;
    try {
        k  = std::stoi (out);
        if (k < 3 || k > 10) error = true;
    }
    catch (std::exception) {error = true;}
    while (error){
        std::cout << "Допущена ошибка при вводе. Повторите ввод: ";
        std::cin >> out;
        error = false;
        try {
            k  = std::stoi (out);
            if (k < 3 || k > 10) error = true;
        }
        catch (std::exception) {error = true;}
    }
    std::cout << "Введите путь для выходного файла: ";
    out = "";
    std::cin >> out;
    std::ofstream file_out(out);
    if (file_out.is_open()){
        time_t sec_start = time(0);
        tm* start = localtime(&sec_start);
        std::vector<int> combIndexOptional;
        int count = 1;
        for (int i = 0; i < k - 3; i++) combIndexOptional.push_back(i);
        do {
            std::string debug_set;
            for (int i = 0; i < combIndexOptional.size(); i++) debug_set+= getDigit(getOptional(combIndexOptional[i]));
            file_out << "_____текущий сет необязательных: " << debug_set <<"_____\n";
            std::vector<int> permIndexOptional = combIndexOptional;
            do {
                std::vector<int> combIndexNeed;
                for (int i = 0; i < 3; i++) combIndexNeed.push_back(i);
                do {
                    std::string debug_set_need;
                    for (int i = 0; i < combIndexNeed.size(); i++) debug_set_need+= getDigit(combIndexNeed[i]);
                    file_out << "-----текущий сет обязательных: " << debug_set_need <<"-----" << std::endl;
                    std::vector<int> permIndexNeed = combIndexNeed;
                    do {
                        std::string temp = sets_to_number(permIndexOptional, permIndexNeed, k);
                        if (temp[0] != '0'){
                            file_out << count << ") " << temp << '\n';
                            count++;
                        }
                    } while (nextPermutation(permIndexNeed));
                } while (nextComb(combIndexNeed, k-1));
            } while (nextPermutation(permIndexOptional));

        }
        while (nextComb(combIndexOptional,6));
        time_t sec_end = time(0);
        tm* end = localtime(&sec_end);
        time_t difference = sec_end - sec_start;
        std::cout << "Запись произведена\n";
        std::cout << "Найдено " << count - 1 << " чисел за ";
        if (difference > 0){
            if (difference>3600*24){
                std::cout << difference / (24*3600) << " дней ";
                difference%=(3600*24);
            }
            if (difference>3600) {
                std::cout << difference / 3600 << " часов ";
                difference%=3600;
            }
            if (difference > 60){
                std::cout << difference / 60 << " минут ";
                difference%=60;
            }
            if (difference > 0){
                std::cout << difference << " секунд ";
            }
        }
        else std::cout << "1 секунду.";
        cin.ignore(1, '\n');
        std::getline(cin, out);
    }
    else std::cout << "Нет доступа к файлу.";

}