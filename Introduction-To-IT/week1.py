"""
Set 1: Simple programs with loops part 1 (required excercises)

Exercise 1. Write a program that reads a natural number from the keyboard and answers the question whether
this number is the product of any two terms of the Fibonacci sequence.

Exercise 2. Write a program that reads three natural numbers a, b, n and prints a decimal expansion
fraction of a / b with an accuracy of n places after the decimal point. (n is on the order of 100)

Exercise 3. Write a program that reads a natural number from the keyboard and answers the question whether
the natural number is a palindrome, then is it a palindrome in binary.

Exercise 4. The two-three-friday number in the breakdown into prime factors has no other factors than
2,3,5. One is also such a number. Write a program that calculates how many such numbers are in the range
from 1 to N inclusive.

Exercise 5. Given is a natural number with unique digits among which there is no zero. How much
different numbers divisible e.g. by 7 can be obtained by crossing out any digits in that number. E.g.
for 2315 it will be 21, 35, 231, 315.
"""

def zad1():
    x = int(input("Write a natural number: \n>>> "))
    a, b = 1, 1
    success = False
    tup = (0, 0)
    while a * a < x:
        c, d = a, b
        while a * c < x:
            c, d = d, c + d
        if a * c == x:
            success = True
            tup = (a, c)
            break
        a, b = b, a + b
    if success:
        print(success)
        print(tup)
    else:
        if x == 1:
            print(True)
            print(1, 1)
        else:
            print(success)


def zad2():
    a, b, n = int(input()), int(input()), int(input())
    counter = 0
    print(a // b, end=",")
    a %= b
    a *= 10
    while counter < n:
        if a < b:
            print(0, end="")
        else:
            print(a // b, end="")
            a %= b
        a *= 10
        counter += 1

def zad3():
    number = int(input())
    x1 = number
    x2 = 0
    result = False
    while number != 0:
        x2 = x2 * 10 + number % 10
        number //= 10
    if x1 == x2:
        result = True
    print(result)

# Instead of checking all numbers in range(1, N), we generate them using three nested loops
def zad4():
    N = int(input())
    for k in range(N):
        if 5 ** k > N:
            break

        for j in range(N):
            if (5 ** k) * (3 ** j) > N:
                break

            for i in range(N):
                if (5 ** k) * (3 ** j) * (2 ** i) > N:
                    break
                else:
                    print((5 ** k) * (3 ** j) * (2 ** i))

def zad5(num):
    counter = 0

    def get_length(num):
        return int(log10(num)) + 1

    # function that crosses out digits from number using tmp (in tmp's binary notation 1 means that we take digit
    # 0 - we dont). Max tmp's value is 2**get_length(num) - 1
    def create_number(num, tmp):
        rev_num = 0  # reversed number that we want to get
        c_num = 0  # number we want to get

        while num > 0:
            if tmp % 2 == 1:
                rev_num = rev_num * 10 + num % 10
                num //= 10
            tmp //= 2

        # reverse rev_num
        while rev_num > 0:
            c_num = c_num * 10 + rev_num % 10
            rev_num //= 10

        return c_num

    for i in range(1, 2 ** get_length(num)):
        if create_number(num, i) % 7 == 0:
            counter += 1

    return counter
