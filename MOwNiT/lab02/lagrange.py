import numpy as np
import pylab as plt

a = 0.5
b = 4
k = 4

res1c = []
res2c = []
res1e = []
res2e = []


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


def get_accuracy(Y, IY):
    N = len(Y)
    acc1 = 0
    acc2 = 0

    for i in range(N):
        if acc1 < abs(Y[i] - IY[i]):
            acc1 = abs(Y[i] - IY[i])

    for i in range(N):
        acc2 += (Y[i] - IY[i]) ** 2

    return (acc1, acc2)


def draw(X, Y, Xn, Yn, IYn, p, n):
    if p == "c":
        plt.title(f"Lagrange interpolation for {n} Chebyshev's nodes")
    else:
        plt.title(f"Lagrange interpolation for {n} equidistant nodes")

    plt.plot(X, Y, label="Original", color="orange")
    plt.plot(X, IYn, label="Lagrange", color="blue")

    plt.scatter(Xn, Yn, label="nodes", color="red")
    plt.xlabel("x")
    plt.ylabel("y")
    plt.legend()
    plt.show()


def visualize_acc():
    # number of nodes
    X = [i for i in range(3, 31)]
    plt.plot(X, res1c, label="Chebyshev's nodes", color="red")
    plt.title("Lambda 1 values")
    plt.xlabel("Number of nodes")
    plt.ylabel("Value of lambda 1")

    plt.plot(X, res1e, label=" Equidistant nodes", color="blue")
    plt.xlabel("Number of nodes")
    plt.ylabel("Value of lambda 1")
    plt.legend()
    plt.show()

    plt.plot(X, res2c, label="Chebyshev's nodes", color="red")
    plt.title("Lambda 2 values")
    plt.xlabel("Number of nodes")
    plt.ylabel("Value of lambda 2")

    plt.plot(X, res2e, label="Equidistant nodes", color="blue")
    plt.xlabel("Number of nodes")
    plt.ylabel("Value of lambda 2")
    plt.legend()
    plt.show()


def visualize_lagrange(n, type):
    Xn = []
    X = []
    IYn = []
    if type == "c":
        Xn = chebyshev_distribution(a, b, n)
        X = chebyshev_distribution(a, b, 1000)
    else:
        X = even_distribution(a, b, 1000)
        Xn = even_distribution(a, b, n)

    Yn = f(Xn)
    Y = f(X)

    for xp in X:
        yp = 0
        for i in range(n):
            p = 1
            for j in range(n):
                if i != j:
                    p = p * (xp - Xn[j]) / (Xn[i] - Xn[j])
            yp = yp + p * Yn[i]
        IYn.append(yp)

    a1, a2 = get_accuracy(Y, IYn)
    if type == "c":
        res1c.append(a1)
        res2c.append(a2)
    else:
        res1e.append(a1)
        res2e.append(a2)

    draw(X, Y, Xn, Yn, IYn, type, n)


# rysuje wykresy dla (*) oraz (**) dla wezlow w przedziale od 21 do 30
# for i in range(3, 31):
#     visualize_lagrange(i, "c")
#     visualize_lagrange(i, "e")
visualize_lagrange(60, "e")
# rysuje wykres zależności lambda 1 oraz lambda 2 od ilości węzłów
# visualize_acc()
