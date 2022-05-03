import numpy as np
import pylab as plt

# Quadratic approximation for trygonomic base

a = 0.5
b = 4
N = 1000


def f(x):
    return x * np.sin(4 * np.pi / x)


def tmp(x):
    return 0


def accuracy():
    x = np.linspace(a, b, num=N)
    y = f(x)

    for m in range(1, 8):
        lambda1Values, lambda2Values, points = [], [], []

        for n in range(2 * m + 1, 2 * m + 21, 2):
            points.append(n)
            xi = np.linspace(a, b, num=n)
            Ai, Bi = coefficients(n, m, xi)
            Yi = [getValue(c, Ai, Bi) for c in x]
            l1, l2 = 0, 0
            for i in range(N):
                l1 = max(l1, abs(Yi[i] - y[i]))
                l2 += (Yi[i] - y[i]) ** 2
            lambda1Values.append(l1)
            lambda2Values.append(l2)
        plt.plot(points, lambda1Values, label=f"m={m}")
        print(f"\n--- m = {m} ---")
        print(lambda1Values)
        print(lambda2Values)
    plt.legend()
    plt.show()


def coefficients(n, m, Xi):
    Ai, Bi = [0 for _ in range(m)], [0 for _ in range(m)]
    for j in range(m):
        for i in range(n):
            Ai[j] += f(Xi[i]) * np.cos(j * Xi[i])
            Bi[j] += f(Xi[i]) * np.sin(j * Xi[i])
        Ai[j] *= 2 / n
        Bi[j] *= 2 / n
    return Ai, Bi


def getValue(x, Ai, Bi):
    result = Ai[0] / 2
    for i in range(1, len(Ai)):
        result += (Ai[i] * np.cos(i * x) + Bi[i] * np.sin(i * x))

    return result


# need: n m
def quadraticApproximation(n, m):
    X = np.linspace(a, b, num=N)
    Y = f(X)
    Xi = np.linspace(a, b, num=n)
    Ai, Bi = coefficients(n, m, Xi)

    Yi = [getValue(c, Ai, Bi) for c in X]

    plt.plot(X, Y, label="Funkcja", color="orange")
    plt.plot(X, Yi, label="Aproksymacja", color="blue")
    plt.scatter(Xi, f(Xi), label="Węzły", color="red")
    plt.title(f"Wartości aproksymacji kwadratowej dla {n} węzłów")
    plt.xlabel("Wartości x")
    plt.ylabel("Wartości y")
    plt.legend()
    plt.show()


# program rusyje wyrkes aproksymacji dla zadanych wartości
if __name__ == '__main__':
    n = 10
    m = 2

    quadraticApproximation(n, m)
    # accuracy()