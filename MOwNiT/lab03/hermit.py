import numpy as np
import pylab as plt

a = 0.5
b = 4
k = 4

res1c = []
res2c = []
res1e = []
res2e = []


def get_accuracy(Y, IY):
    N = len(Y)
    acc1 = 0
    acc2 = 0

    for i in range(N):
        if acc1 < abs(Y[i] - IY[i]):
            acc1 = abs(Y[i] - IY[i])

    for i in range(N):
        acc2 += (Y[i] - IY[i]) ** 2

    return acc1, acc2


def visualize_acc():
    res2e.append(0)
    res1e.append(0)
    res2c.append(0)
    res1c.append(0)

    X = [i for i in range(3, 25)]
    plt.plot(X, res1c, label="Węzły Czebyszewa", color="red")
    plt.title("Wartości lambda 1 w zależności od ilości węzłów")
    plt.xlabel("Liczba węzłów")
    plt.ylabel("Wartość lambdy 1")

    plt.plot(X, res1e, label="Węzły równoodległe", color="blue")
    plt.legend()
    plt.show()

    plt.plot(X, res2c, label="Węzły Czebyszewa", color="red")
    plt.title("Wartości lambda 2 w zależności od ilości węzłów")
    plt.xlabel("Liczba węzłów")
    plt.ylabel("Wartości lambdy")

    plt.plot(X, res2e, label="Węzły równoodległe", color="blue")
    plt.xlabel("Liczba węzłów")
    plt.ylabel("Wartości lambdy")
    plt.legend()
    plt.show()


def even_distribution(start, stop, n):
    return np.linspace(start, stop, num=n)


def chebyshev_distribution(start, stop, n):
    chebyt_roots = []
    for k in range(1, n + 1):
        cosin = np.cos((2 * k - 1) / (2 * n) * np.pi)
        curr_result = 0.5 * (start + stop) + 0.5 * (stop - start) * cosin
        chebyt_roots.append(curr_result)
    return np.array(chebyt_roots)


def f(x):
    return x * np.sin(k * np.pi / x)


def f_(x):
    return np.sin(k * np.pi / x) - ((k * np.pi) / x) * np.cos(k * np.pi / x)


def hermit_interpolate(X, Xn, Yn):
    n = len(Xn)
    points = np.zeros(shape=(2 * n + 1, 2 * n + 1))
    Y = []

    for i in range(0, 2 * n, 2):
        points[i][0] = Xn[i // 2]
        points[i + 1][0] = Xn[i // 2]
        points[i][1] = Yn[i // 2]
        points[i + 1][1] = Yn[i // 2]

    for i in range(2, 2 * n + 1):
        for j in range(1 + (i - 2), 2 * n):
            if i == 2 and j % 2 == 1:
                points[j][i] = f_(Xn[j // 2])

            else:
                points[j][i] = (points[j][i - 1] - points[j - 1][i - 1]) / (
                        points[j][0] - points[(j - 1) - (i - 2)][0])

    def result_polynomial(xpoint):  # here is function to calculate value for given x
        val = 0
        for i in range(0, 2 * n):
            factor = 1.
            j = 0
            while j < i:
                factor *= (xpoint - Xn[j // 2])
                if j + 1 != i:
                    factor *= (xpoint - Xn[j // 2])
                    j += 1
                j += 1
            val += factor * points[i][i + 1]
        return val

    for x0 in X:
        Y.append(result_polynomial(x0))
    return Y


def visualize(n, p):
    X = []
    Xn = []
    if p == "c":
        X = chebyshev_distribution(a, b, 1000)
        Xn = chebyshev_distribution(a, b, n)
        plt.title(f"Interpolacja Hermita dla {n} węzłów Czebyszewa")
    else:
        X = even_distribution(a, b, 1000)
        Xn = even_distribution(a, b, n)
        plt.title(f"Interpolacja Hermita dla {n} równoodległych węzłów")

    Y = f(X)
    Yn = f(Xn)

    Y_ = hermit_interpolate(X, Xn, Yn)

    a1, a2 = get_accuracy(Y, Y_)

    if p == "c":
        res1c.append(a1)
        res2c.append(a2)
    else:
        res1e.append(a1)
        res2e.append(a2)

    plt.plot(X, Y, label="Funkcja", color="orange")
    plt.plot(X, Y_, label="Hermit", color="blue")
    plt.scatter(Xn, Yn, label="Węzły", color="red")
    plt.xlabel("x")
    plt.ylabel("y")
    plt.legend()
    plt.show()


if __name__ == '__main__':
    # rysuje wykresy dla wezlow od 3 - 24 dla roznych rodzajow wezlow
    for i in range(3, 25):
        visualize(i, "c")

    # rysuje zaleznosc dokladnosci dla roznych rodzajow wezlow
    visualize_acc()