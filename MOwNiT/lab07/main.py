import numpy as np
import pylab as plt
import time as t


def gaussianElimination(a, b):
    n = len(b)

    for k in range(0, n - 1):
        for i in range(k + 1, n):
            if a[i][k] != 0.0:
                lam = a[i][k] / a[k][k]
                a[i][k + 1:n] -= lam * a[k][k + 1:n]
                b[i] -= lam * b[k]

    for k in range(n - 1, -1, -1):
        b[k] = (b[k] - np.dot(a[k][k + 1:n], b[k + 1:n])) / a[k][k]
    return b


def maximumError(x1, x2):
    n = len(x1)
    max_error = 0
    for i in range(n):
        max_error = max(max_error, abs(x1[i] - x2[i]))
    return max_error


def solve_zad1():
    # data types
    dt = [np.float16, np.float32, np.float64]

    for data_type in dt[2:]:
        # data for graph
        x = [i for i in range(2, 21)]
        y = []
        for n in range(2, 21):
            # setup A matrix
            A = np.ones(shape=(n, n), dtype=data_type)
            for i in range(1, n + 1):
                for j in range(2, n + 1):
                    A[i - 1][j - 1] = 1 / (i + j - 1)

            # setup starting x1 values
            x1 = np.ones(shape=(n, 1), dtype=data_type)
            for i in range(n):
                if i % 2 == 1:
                    x1[i] *= -1

            # calculate B matrix
            B = np.array(A.dot(x1), dtype=data_type)

            # get new x values
            x2 = gaussianElimination(A, B)
            # calculate maximum error
            max_error = maximumError(x1, x2)
            y.append(max_error)

        plt.plot(x, y, label=f"Błąd dla {data_type.__name__}")

        plt.xlabel("Wartość n")
        plt.ylabel("Wartość błędu")
        plt.xticks(np.arange(2, 21, step=2))

    plt.title(f"Wartość maksymalnego błędu w zależności od wielkości macierzy")
    plt.legend()
    plt.show()


def solve_zad2(data_type):
    # data for graph
    x = [i for i in range(2, 203)]
    y = []
    times = []
    for n in range(2, 203):
        # setup A matrix
        A = np.ones(shape=(n, n), dtype=data_type)
        for i in range(1, n + 1):
            for j in range(1, n + 1):
                if i > j:
                    A[i - 1][j - 1] = A[j - 1][i - 1]
                else:
                    A[i - 1][j - 1] = 2 * i / j

        # setup starting x1 values
        x1 = np.ones(shape=(n, 1), dtype=data_type)
        for i in range(n):
            if i % 2 == 1:
                x1[i] *= -1

        # calculate B matrix
        B = np.array(A.dot(x1), dtype=data_type)

        # setup time
        start = t.time()

        # get new x values
        x2 = gaussianElimination(A, B)

        # get time
        end = t.time()
        # calculate maximum error
        max_error = maximumError(x1, x2)
        if n % 20 == 0 or n == 2:
            print(max_error)
        y.append(max_error)
        times.append(end - start)

    plt.plot(x, y)
    plt.title(f"Maksymalny błąd w zależności od n dla {data_type.__name__}")
    plt.xlabel("Wielkość macierzy n x n")
    plt.ylabel("Wartość maksymalnego błędu")
    plt.xticks(np.arange(2, 203, step=20))

    plt.show()

    plt.plot(x, times)
    plt.title(f"Czasy w zależności od n dla {data_type.__name__}")
    plt.xlabel("Wielkość macierzy n x n")
    plt.ylabel("Czas w sekundach")
    plt.xticks(np.arange(2, 203, step=20))

    plt.show()


def thomasAlgorithm(A, n):
    # setup starting x matrix
    X = np.ones(shape=(n, 1), dtype=np.float64)
    for i in range(n):
        if i % 2 == 1:
            X[i] *= -1

    # get B matrix
    B = A.dot(X)

    # setup two help arrays
    C, D = np.zeros(shape=n, dtype=np.float64), np.zeros(shape=n, dtype=np.float64)

    start = t.time()
    # algorithm
    for i in range(n):
        # fill C
        if i == 0:
            C[i] = A[i][i + 1] / A[i][i]
            D[i] = B[i] / A[i][i]
        else:
            if i < n - 1:
                C[i] = A[i][i + 1] / (A[i][i] - A[i + 1][i] * C[i - 1])
            D[i] = (B[i] - A[i][i-1] * D[i-1]) / (A[i][i] - A[i][i-1] * C[i-1])
        # fill D

    Xn = np.zeros(shape=n, dtype=np.float64)
    for i in range(n-1, -1, -1):
        if i == n - 1:
            Xn[i] = D[i]
        else:
            Xn[i] = D[i] - C[i]*Xn[i+1]
    end = t.time()
    return maximumError(X, Xn), end - start


def gaussianMethod():
    data_type = np.float64
    bot_bound, top_bound, step = 19999, 203, 20
    # data for graph
    x = [i for i in range(2, top_bound)]
    y = []
    times = []
    for n in range(2, top_bound):
        # setup A matrix
        A = setupMatrix3(n, 7, 4)

        # setup starting x1 values
        x1 = np.ones(shape=(n, 1), dtype=data_type)
        for i in range(n):
            if i % 2 == 1:
                x1[i] *= -1

        # calculate B matrix
        B = np.array(A.dot(x1), dtype=data_type)

        # setup time
        start = t.time()

        # get new x values
        x2 = gaussianElimination(A, B)

        # get time
        end = t.time()
        # if n % step == 0:
        #     print(f'n = {n} | time: {round(end-start, 10)} seconds')
        if n == 2 or n % 40 == 0:
            print(end - start)
        # # calculate maximum error
        max_error = maximumError(x1, x2)
        y.append(max_error)
        times.append(end - start)

    plt.plot(x, y)
    plt.title(f"Maksymalny błąd w zależności od wielkości macierzy")
    plt.xlabel("Wielkość macierzy n x n")
    plt.ylabel("Wartość maksymalnego błędu")
    plt.xticks(np.arange(2, top_bound, step=step))

    plt.show()

    plt.plot(x, times)
    plt.title(f"Czasy w zależności od wielkości macierzy")
    plt.xlabel("Wielkość macierzy n x n")
    plt.ylabel("Czas w sekundach")
    plt.xticks(np.arange(2, top_bound, step=step))

    plt.show()


def setupMatrix3(n, k, m):
    A = np.zeros(shape=(n, n), dtype=np.float64)
    for i in range(1, n + 1):
        for j in range(1, n + 1):
            if i == j:
                A[i - 1][j - 1] = k
            elif j == i + 1:
                A[i - 1][j - 1] = 1 / (i + m)
            elif i > 1 and j == i - 1:
                A[i - 1][j - 1] = k / (i + m + 1)
            else:
                A[i - 1][j - 1] = 0
    return A


def solve_zad3():
    # print("Gaussian Elimination method:")
    gaussianMethod()

    n = 20000
    A = setupMatrix3(n, 7, 4)
    max_error, time = thomasAlgorithm(A, n)
    print(f'Max error: {max_error} | time: {time}')



if __name__ == '__main__':
    # solve_zad1()

    # choose data type: np.float16, 32, 64
    # solve_zad2(np.float64)

    solve_zad3()
