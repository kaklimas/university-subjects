import numpy as np
import matplotlib.pyplot as plt

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


def draw(x, y, function, n, p):
    X = even_distribution(a, b, 1000)
    Y = f(X)
    plt.scatter(x, y, label="data", color="red")
    plt.plot(X, function(X, x, y), label="Newton", color="blue")
    a1, a2 = get_accuracy(Y, function(X, x, y))
    if p == "Chebyshev's":
        res1c.append(a1)
        res2c.append(a2)
    else:
        res1e.append(a1)
        res2e.append(a2)
    plt.plot(X, Y, label="original", color="orange")
    plt.title(f"Newton’s Polynomial Interpolation on {n} {p} nodes")
    plt.xlabel("x")
    plt.ylabel("y")
    plt.legend()
    # plt.show()


def newton_interpolation(X, x, y):
    m = [[0 for _ in range(len(x))] for _ in range(len(x))]
    Y = [0 for _ in range(len(x))]

    for i in range(len(x)):
        m[i][0] = y[i]
    for j in range(1, len(x)):
        for i in range(len(x) - j):
            m[i][j] = (m[i + 1][j - 1] - m[i][j - 1]) / (x[i + j] - x[i])

    xterm = 1
    Y[0] = m[0][0]
    for i in range(1, len(x)):
        xterm *= (X - x[i - 1])
        Y[i] = Y[i - 1] + m[0][i] * xterm

    return Y[-1]


def f(x):
    return x * np.sin(k * np.pi / x)


def visualize_newton(start, stop, n):
    # even distribution
    even_x = even_distribution(start, stop, n)
    even_y = f(even_x)
    draw(even_x, even_y, newton_interpolation, n, "equidistant")

    # chebyshev distribution
    chebyt_x = chebyshev_distribution(start, stop, n)
    chebyt_y = f(chebyt_x)
    draw(chebyt_x, chebyt_y, newton_interpolation, n, "Chebyshev's")


def visualize_acc():
    X = [i for i in range(3, 30)]
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


# rysuje wykresy dla (*) oraz (**) dla wezlow w przedziale od 21 do 30
for i in range(3, 70, 1):
    visualize_newton(a, b, i)

# rysuje wykres zależności lambda 1 oraz lambda 2 od ilości węzłów
# visualize_acc()
