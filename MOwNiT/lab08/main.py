import time as t
import numpy as np
import random as r
import pandas as pd


progressive_difference = lambda A, b, x_curr, x_next, p: np.linalg.norm(x_next - x_curr) < p
solution_difference = lambda A, b, x_curr, x_next, p: np.linalg.norm(np.dot(A, x_next) - b) < p
max_error = lambda x1, x2: np.linalg.norm(x1 - x2)
spectral_ray = lambda M: max(abs(np.linalg.eigvals(M)))


def setupMatrixA(n):
    k = 10
    m = 3
    A = np.zeros((n, n), np.float64)
    for i in range(1, n + 1):
        for j in range(1, n + 1):
            if i != j:
                A[i - 1][j - 1] = m / (n - i - j + 0.5)
            else:
                A[i - 1][j - 1] = k
    return A


def getMatrixB(A):
    X = np.ones((len(A)), np.float64)
    for i in range(len(X)):
        if i % 2 == 1:
            X[i] *= -1

    return A.dot(X)


def getSpectralRay(A):
    D = np.diag(A)
    R = A - np.diagflat(D)
    S = R / D
    return spectral_ray(S)


def jacobi(A, b, start_vector, stop_criterion, p, max_iter=5000):
    start = t.time()
    D = np.diag(A)
    R = A - np.diagflat(D)
    x_curr, x_next = start_vector, start_vector

    i = 0

    while i < max_iter:
        x_next = (b - (R.dot(x_curr))) / D
        i += 1
        if stop_criterion(A, b, x_curr, x_next, p): break
        x_curr = x_next
    end = t.time()
    return x_curr, i, end - start, getSpectralRay(A)


def generateStartingVector(n, style):
    x = np.ones(n, np.float64)
    if style == 'ones':
        for i in range(n):
            if i % 2 == 0:
                x[i] *= -1
    elif style == 'zeros':
        return np.zeros(n, np.float64)
    elif style == 'big_random':
        for i in range(n):
            x[i] = r.randint(-50, 50)
    return x


if __name__ == '__main__':
    p = 1e-5
    stop_condition = progressive_difference
    starting_vector = 'random'
    N = [2, 3, 5, 8]
    N.extend([i for i in range(10, 101, 10)])
    N.extend([200, 300])

    result = []
    for n in N:
        solution = np.ones(n)
        for i in range(n):
            if i % 2 == 1:
                solution[i] *= -1
        A = setupMatrixA(n)
        b = getMatrixB(A)
        start_v = generateStartingVector(n, starting_vector)

        x1, i1, t1, r1 = jacobi(A, b, start_v, progressive_difference, p)
        x2, i2, t2, r2 = jacobi(A, b, start_v, solution_difference, p)

        result += [max_error(solution, x1), max_error(solution, x2), i1, i2, t1, t2, r1, r2]

    df = pd.DataFrame(data={"n": N,
                            "1st condition max_error": result[::8],
                            "2nd condition max_error": result[1::8],
                            "1st condition iterations": result[2::8],
                            "2nd condition iterations": result[3::8],
                            "1st condition time [s]": result[4::8],
                            "2nd condition time[s]": result[5::8],
                            "1st condition ray": result[6::8],
                            "2nd condition ray": result[7::8]})

    pd.set_option('display.max_column', 8)
    print(df)
