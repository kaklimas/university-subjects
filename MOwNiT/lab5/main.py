import pylab as plt
import numpy as np


def draw(Xa, Ai, B, function, X):
    plt.title(f"Wartości funkcji aproksymującej kwadrawtowo dla {len(Xa)} węzłów oraz {(len(B))} jednomianów")
    plt.scatter(Xa, f(Xa), label="Węzły", color="red")
    plt.plot(X, f(X), label="Funkcja", color="orange")
    plt.plot(X, function(X, Ai, B), label="Aproksymacja kwadratowa", color="green")
    plt.xlabel("Wartości x")
    plt.ylabel("Wartości y")
    plt.legend()
    plt.show()


def qudraticAproximation(x, ai, base):
    res = 0
    for j in range(m):
        res += ai[j] * x ** base[j]
    return res


def getCoeff(Xa, w, b):
    n = len(xa)
    m = len(base)
    # Ai * B = C
    Ai = np.ones(shape=(m, 1))
    B = np.zeros(shape=(m, m))
    C = np.zeros(shape=(m, 1))

    # uzupelnianie macierzy C
    for k in range(m):
        for i in range(n):
            C[k] += w[i] * f(Xa[i]) * Xa[i] ** k

    # uzupelnianie macierzy B
    for k in range(m):
        for j in range(m):
            for i in range(n):
                B[k][j] += w[i] * Xa[i] ** (k + j)

    # odwracanie macierzy B
    B = np.linalg.inv(B)

    # wynik mnożenia macierzy
    Ai = B.dot(C)

    return Ai


if __name__ == '__main__':

    # dane wejściowe
    a = 0.5
    b = 4

    # liczba punktów do rysowania wykresu funkcji
    N = 1000
    x = np.linspace(a, b, num=N)


    def f(x):
        return x * np.sin(4 * np.pi / x)


    # liczba węzłów (n)
    n = 40
    xa = np.linspace(a, b, num=n)

    # wagi punktów
    weights = [1 for _ in range(len(xa))]

    # liczba funkcji bazowych (m)
    m = 12
    base = [i for i in range(m)]

    ai = getCoeff(xa, weights, base)

    draw(xa, ai, base, qudraticAproximation, x)