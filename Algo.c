#include <stdio.h> /*присоединяем все нужные библиотеки*/
#include <string.h>
#include <stdlib.h>
#include <math.h>


int findmax(unsigned long long numbers[]) { /*функция для ввода 4х чисел и нахождения максимального*/
	unsigned short max = 0;
	for (short i = 0; i < 4; i++) {
		scanf_s("%llu", &numbers[i]);
		if (numbers[i] > max)
			max = numbers[i];
	}
	return max;
}

unsigned long long findopred(unsigned long long numbers[]) { /*находим определитель матрицы*/
	unsigned long long matrix[3][3] = { {numbers[0], numbers[1], 1}, {numbers[1], numbers[2], 1}, {numbers[2], numbers[3], 1} };
	return llabs(matrix[0][0] * matrix[1][1] * matrix[2][2] + matrix[0][1] * matrix[1][2] * matrix[2][0] + matrix[1][0] * matrix[2][1] * matrix[0][2] - matrix[0][2] * matrix[1][1] * matrix[2][0] - matrix[0][1] * matrix[1][0] * matrix[2][2] - matrix[0][0] * matrix[1][2] * matrix[2][1]);
}



int main(void) {
	unsigned long long numbers[4]; /*массив для хранения чисел х1, х2, х3, х4*/
	unsigned short max, check = 0, stop = 0, j = 1; /*max - максимально значение из введенных чисел, check - проверка на конгруэнтность, stop - выход из цикла при достижении j > 100, j - количество вариантов решений*/
	int lastNum; /* число х5*/
	unsigned long long opred; /*определитель матрицы*/
	printf("Enter the first 4 numbers: ");
	max = findmax(numbers); /*вводим числа и находим максимальное*/
	printf("Result:\n");
	opred = findopred(numbers); /*находим определитель матрицы*/
	for (unsigned long long m = max + 1; m <= 65535; m++) { /* число m будет больше max на 1*/
		if (opred % m == 0) { /*определитель будет кратным числу m, таким образом мы находим неизвестную переменную*/
			for (unsigned long long a = 0; a < m; a++) { /* переменные а и с находятся перебором, так как известно число m*/
				for (unsigned long long c = 0; c < m; c++) {
					for (short i = 0; i < 3; i++) { /*проверка на конгруэнтность*/
						if (numbers[i + 1] == (a * numbers[i] + c) % m) { /*условие конгруэнтности*/
							check++; /*счетчик для определения правильность а и с*/
						}
						else { /*если условие конгруэнтности ложно, обнуляем счетчик и переходим к следущей паре а и с*/
							check = 0;
							break;
						}
						if (check == 3) { /*если все 3 уравнения истинные*/
							lastNum = (a * numbers[3] + c) % m;/*задаем последнее число*/
							printf("Case %3d: a = %5llu c = %5llu m = %5llu nextNumber = %5d\n", j, a, c, m, lastNum);/*выводим номер варианта, числа а, с, m и х5*/
							j++;/*счетчик количества вариантов решения*/
							if (j > 100) {/*если количество вариантов решения превышает 100, выходим из цикла, это необходимо для тех случаев, когда определитель матрицы равен 0, например 7 6 9 0, тогда мы получаем множество решений для 10<=m<=65535*/
								stop = 1;
							}

						}

					}
					if (stop == 1) break;
				}
				if (stop == 1) break;
			}
			if (stop == 1) break;
		}
	}
	return 0;
}
