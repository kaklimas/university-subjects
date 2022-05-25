import pylab as plt
import numpy as np

# dane wejściowe
a = 0.5
b = 4

# liczba punktów do rysowania wykresu funkcji
N = 1000


def f(x):
    return x * np.sin(4 * np.pi / x)


# get accuracy for nodes
def getAccuracy(baseLength):
    xParams = np.linspace(a, b, num=N)
    yParams = f(xParams)

    B = [i for i in range(baseLength)]

    l1, l2 = [], []

    for n_no in range(5, 51, 5):
        xNodes = np.linspace(a, b, num=n_no)
        W = [1 for _ in range(n_no)]
        aCoeffs = getCoeff(xNodes, W, B)

        yApproximate = [qudraticAproximation(k, aCoeffs, B) for k in xParams]

        # lambda 1 values
        lambda1 = 0
        for i in range(N):
            lambda1 = max(abs(yParams[i] - yApproximate[i]))
        l1.append(lambda1)

        lambda2 = 0
        for i in range(N):
            lambda2 += (yParams[i] - yApproximate[i]) ** 2
        l2.append(lambda2)

    return l1, l2


def drawAccuracy():
    tmp = [i for i in range(5, 51, 5)]
    for m in range(1, 7):
        l1, l2 = getAccuracy(m)

        plt.plot(tmp, l1, label=f"m = {m}")
    plt.title("Wartości lambda 1")
    plt.xlabel("Ilość węzłów")
    plt.ylabel("Wartości niedokładności")
    plt.legend()
    plt.show()

    for m in range(1, 7):
        l1, l2 = getAccuracy(m)
        plt.plot(tmp, l2, label=f"m = {m}")
    plt.title("Wartości lambda 2")
    plt.xlabel("Ilość węzłów")
    plt.ylabel("Wartości niedokładności")
    plt.legend()
    plt.show()


def draw(Xa, Ai, B, function, X):
    plt.title(f"Wartości aproksymacji kwadratowej dla {len(Xa)} węzłów")
    plt.scatter(Xa, f(Xa), label="Węzły", color="red")
    plt.plot(X, f(X), label="Funkcja", color="orange")
    plt.plot(X, function(X, Ai, B), label="Aproksymacja kwadratowa", color="green")
    plt.xlabel("Wartości x")
    plt.ylabel("Wartości y")
    plt.legend()
    plt.show()


def qudraticAproximation(x, ai, base):
    res = 0
    m = len(base)
    for j in range(m):
        res += ai[j] * x ** base[j]
    return res


def getCoeff(Xa, w, b):
    n = len(Xa)
    m = len(b)
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
    x = np.linspace(a, b, num=N)

    # liczba węzłów (n)
    n = 30
    xa = np.linspace(a, b, num=n)

    # wagi punktów
    weights = [1 for _ in range(len(xa))]

    # liczba funkcji bazowych (m)
    m = 6
    base = [i for i in range(m)]

    ai = getCoeff(xa, weights, base)

    draw(xa, ai, base, qudraticAproximation, x)
    # drawAccuracy()

