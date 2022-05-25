import pylab as plt
import numpy as np

# global values
a, b, N = 0.2, 2, 1000

stop_condition1 = lambda x_curr, x_prev, p: abs(x_curr - x_prev) < p
stop_condition2 = lambda f_curr, p: abs(f_curr) < p


def f(x):
    return x ** 15 - (1 - x) ** 10


def f_(x):
    return 15 * x ** 14 + 10 * (1 - x)


def draw_function():
    X = np.linspace(a, b, num=N)
    Y = f(X)

    plt.plot(X, Y, label="Przebieg funkcji")
    plt.xlabel("Wartości x")
    plt.ylabel("Wartości y")
    plt.legend()
    plt.show()


def get_acc(array):
    n = len(array)
    s = 0
    for num in array:
        s += num
    return round(s / n, 10)


def calculate_euler(p):
    x1, x2 = a, b
    S1, I1, S2, I2 = [], [], [], []
    X = np.arange(a, b, 0.1)
    while x1 < x2:
        res = euler(x1, x2, p)
        s1, i1, s2, i2 = res
        S1.append(s1)
        S2.append(s2)
        I1.append(i1)
        I2.append(i2)
        print(i1)
        x2 -= 0.1
        x2 = round(x2, 3)


    print("Średnia 1: ", get_acc(S1))
    print("Średnia 2: ", get_acc(S2))



    plt.plot(X, I1[::-1], label="Liczba iteracji dla pierwszego kryt.")
    plt.plot(X, I2[::-1], label="Liczba iteracji dla drugiego kryt.")
    plt.title('Liczba iteracji w zależności od wartości x2')
    plt.xlabel('Wartości początkowe')
    plt.ylabel('Liczba iteracji')
    plt.legend()
    plt.show()


def calculate_newton(p):
    x0 = a
    S1, I1, S2, I2 = [], [], [], []
    X = np.arange(a, b + 0.1, 0.1)
    while x0 <= b:
        res = newton(x0, p)
        s1, i1, s2, i2 = res
        print(s1)
        S1.append(s1)
        S2.append(s2)
        I1.append(i1)
        I2.append(i2)

        x0 += 0.1
        x0 = round(x0, 3)
    print("Średnia 1: ", get_acc(S1))
    print("Średnia 2: ", get_acc(S2))
    plt.plot(X, I1, label="Liczba iteracji dla pierwszego kryt.")
    plt.plot(X, I2, label="Liczba iteracji dla drugiego kryt.")
    plt.title('Liczba iteracji w zależności od wartości początkowej x0')
    plt.xlabel('Wartości początkowe')
    plt.ylabel('Liczba iteracji')
    plt.legend()
    plt.show()


def newton(x, p):
    iterations1, iterations2 = 0, 0
    sol1, sol2 = 0, 0
    x = round(x, 3)

    x0 = x
    x1 = x0 - f(x0) / f_(x0)
    while not stop_condition1(x1, x0, p):
        x0 = x1
        x1 = x0 - f(x0) / f_(x0)
        iterations1 += 1
    sol1 = x1

    while not stop_condition2(f(x), p):
        x = x - f(x) / f_(x)
        iterations2 += 1
    sol2 = x

    return round(sol1, 10), iterations1, round(sol2, 10), iterations2


def euler(x0, x1, p):
    iterations1, iterations2 = 0, 0
    sol1, sol2 = 0, 0
    a0, a1 = round(x0, 3), round(x1, 3)

    while not stop_condition1(a1, a0, p):
        sth = f(a1) - f(a0)
        if sth == 0:
            print("Division by zero error")
            break
        a0, a1 = a1, a1 - (a1 - a0) * f(a1) / sth
        iterations1 += 1
    sol1 = a1

    a0, a1 = round(x0, 3), round(x1, 3)
    while not stop_condition2(f(a1), p):
        sth = f(a1) - f(a0)
        if sth == 0:
            print("Division by zero error")
            break
        a0, a1 = a1, a1 - (a1 - a0) * f(a1) / sth
        iterations2 += 1
    sol2 = a1

    return round(sol1, 10), iterations1, round(sol2, 10), iterations2


if __name__ == '__main__':
    # calculate_newton(0.0000001)
    # draw_function()
    # print(euler(1.9, 2, 0.00000001))
    calculate_euler(0.0001)
    # print(euler(0.2, 2, 0.0000001))
