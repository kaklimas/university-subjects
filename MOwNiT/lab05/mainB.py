from copy import deepcopy
import pylab as plt

import numpy as np

N = 1000
a = 0.5
b = 4


class SquareApproximation:
    def __init__(self, X, Y, n, m, start, stop):
        if m > np.floor((n - 1) / 2):
            raise Exception("Podane wartości są nieprawidłowe! M powinno być większe od floor z (n-1)/2")
        self.X = X
        self.Y = Y
        self.n = n
        self.m = m
        self.start = start
        self.stop = stop
        self.A = np.zeros(self.n)
        self.B = np.zeros(self.n)

        def scale_to_2pi(self):
            range_length = self.stop - self.start
            for i in range(len(self.X)):
                self.X[i] /= range_length
                self.X[i] *= 2 * np.pi
                self.X[i] += -np.pi - (2 * np.pi * self.start / range_length)

        def compute_A_and_B(self):
            for i in range(self.n):
                ai = sum(self.Y[j] * np.cos(i * self.X[j]) for j in range(self.n))
                bi = sum(self.Y[j] * np.sin(i * self.X[j]) for j in range(self.n))
                self.A[i] = 2 * ai / self.n
                self.B[i] = 2 * bi / self.n

        def scale_from_2pi(self):
            range_length = self.stop - self.start
            for i in range(len(self.X)):
                self.X[i] -= -np.pi - (2 * np.pi * self.start / range_length)
                self.X[i] /= 2 * np.pi
                self.X[i] *= range_length

        scale_to_2pi(self)
        compute_A_and_B(self)
        scale_from_2pi(self)

    def scale_point_to_2pi(self, x):
        range_length = self.stop - self.start
        x /= range_length
        x *= 2 * np.pi
        x += -np.pi - (2 * np.pi * self.start / range_length)
        return x

    def approximate(self):
        points = []
        Xi = np.linspace(a, b, num=N)
        for x in Xi:
            cp_x = deepcopy(x)
            cp_x = self.scale_point_to_2pi(cp_x)
            approximated_x = 1 / 2 * self.A[0] + sum(self.A[j] * np.cos(j * cp_x) + self.B[j] * np.sin(j * cp_x)
                                                     for j in range(1, self.m + 1))
            points.append(approximated_x)
        return points


def f(x):
    return x * np.sin(4 * np.pi / x)


def getAccuracy():
    X = np.linspace(a, b, num=N)
    Y = f(X)
    for m in range(3, 13):
        L1, L2 = [], []
        for n in range(m * 2 + 1, m * 2 + 41):
            l1, l2 = 0, 0
            trigonometric_approximation = SquareApproximation(X, Y, n, m, a, b)
            Yi = trigonometric_approximation.approximate()
            for i in range(N):
                l1 = max(l1, abs(Y[i] - Yi[i]))
                l2 += (Y[i] - Yi[i]) ** 2
            L1.append(l1)
            L2.append(l2)
        plt.plot([c for c in range(m * 2 + 1, m * 2 + 41)], L1, label=f"m = {m}")
        print("Current m = ", m)
        if m % 3 == 0:
            for c in range(len(L1)):
                if c % 10 == 0:
                    print(f"index={m*2+1+c} value: ", L1[c])
        print()
    plt.title(f"Wartości lambda 1 dla różnych m w zależności od liczby węzłów")
    plt.xlabel("liczba węzłów")
    plt.ylabel("wartości")
    plt.legend()
    plt.show()


def visualize(x, y, start, stop, n, m, approximated_points):
    plt.figure(figsize=(8, 6))
    X = np.linspace(start, stop, num=N)
    Y = f(X)
    plt.plot(X, Y, label="Funkcja", color="orange")
    plt.scatter(x, y, label="Węzły", color="red")
    plt.plot(X, approximated_points, label="Aproksymacja", color="green")
    plt.title(f"Aproksymacja trygonometryczna dla n = {n} oraz m = {m}")
    plt.xlabel("x")
    plt.ylabel("y")
    plt.legend()
    plt.show()


def approximation(start, stop, n, m):
    X = np.linspace(start, stop, num=n)
    Y = f(X)

    trigonometric_approximation = SquareApproximation(X, Y, n, m, start, stop)

    visualize(X, Y, start, stop, n, m, trigonometric_approximation.approximate())


approximation(a, b, 70, 30)
# getAccuracy()
