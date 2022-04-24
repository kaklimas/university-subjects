import numpy as np
import pylab as plt
from scipy.interpolate import lagrange

a = 0.5
b = 4
k = 4
N = 1000

def getAcc():
    natural_lamda1_cubic = []
    natural_lamda1_quad = []

    natural_lamda2_cubic = []
    natural_lamda2_quad = []

    periodic_lambda1_cubic = []
    periodic_lambda1_quad = []

    periodic_lambda2_cubic = []
    periodic_lambda2_quad = []

    X = np.linspace(a, b, num=1000)
    Y = f(X)

    for i in range(10, 201, 5):
        Xn = np.linspace(a, b, num=i)
        Yn = f(Xn)
        Y_q = quadraticSpline(Xn, Yn, "n")
        Y_c = cubicSpline(Xn, Yn, "n")

        Y_q_2 = quadraticSpline(Xn, Yn, "p")
        Y_c_2 = cubicSpline(Xn, Yn, "p")

        best_1 = 0
        best_2 = 0
        for j in range(0, len(Y_q)):
            best_1 = max(best_1, abs(Y_q[j] - Y[j]))
            best_2 = max(best_2, abs(Y_c[j] - Y[j]))
        natural_lamda1_quad.append(best_1)
        natural_lamda1_cubic.append(best_2)

        best_1 = 0
        best_2 = 0
        for j in range(0, len(Y_q)):
            best_1 += (Y[j] - Y_q[j]) ** 2
            best_2 += (Y[j] - Y_c[j]) ** 2
        natural_lamda2_quad.append(best_1)
        natural_lamda2_cubic.append(best_2)

        ##
        best_3 = 0
        best_4 = 0
        for j in range(0, len(Y_q)):
            best_3 = max(best_3, abs(Y_q_2[j] - Y[j]))
            best_4 = max(best_4, abs(Y_c_2[j] - Y[j]))
        periodic_lambda1_quad.append(best_3)
        periodic_lambda1_cubic.append(best_4)

        best_3 = 0
        best_4 = 0
        for j in range(0, len(Y_q)):
            best_3 += (Y[j] - Y_q_2[j]) ** 2
            best_4 += (Y[j] - Y_c_2[j]) ** 2
        natural_lamda2_quad.append(best_1)
        natural_lamda2_cubic.append(best_2)

        periodic_lambda2_quad.append(best_3)
        periodic_lambda2_cubic.append(best_4)

    #print(len(natural_lamda2_cubic))
    X = [i for i in range(10, 201, 5)]
    plt.title("Wartości lambda 1 dla naturalnego warunku")
    plt.plot(X, periodic_lambda1_quad, label="Wielomian kwadratowy", color="red")
    plt.plot(X, periodic_lambda1_cubic, label="Wielomian sześcienny", color="green")
    plt.xlabel("Ilość węzłów")
    plt.ylabel("Lambda 1")
    plt.legend()
    plt.show()
    plt.xlabel("Ilość węzłów")
    plt.ylabel("Lambda 2")
    plt.title("Wartości lambda 2 dla periodycznego warunku")
    plt.plot(X, periodic_lambda2_quad, label="Wielomian kwadratowy", color="red")
    plt.plot(X, periodic_lambda2_cubic, label="Wielomian sześcienny", color="green")
    plt.legend()
    plt.show()


def spline(n, boundary_condition_type, show_lagrange=False):
    # wezly
    Xn = np.linspace(a, b, num=n)
    Yn = f(Xn)

    # oryginalna funckja
    X = np.linspace(a, b, num=N)
    Y = f(X)

    # sklejki
    Y_iq = []
    Y_iq = quadraticSpline(Xn, Yn, boundary_condition_type)
    Y_ic = cubicSpline(Xn, Yn, boundary_condition_type)

    # potencjalny lagrange
    Y_l = []
    Y_l = lagrange(Xn, Yn)(X)
    text = ""
    if boundary_condition_type == "n":
        text = "Naturalna"
    elif boundary_condition_type == "p":
        text = "Periodyczna"
    plt.title(f"{text} interpolacja wielomianowa dla {n} węzłów")
    plt.plot(X, Y, label="Funkcja", color="orange")
    #plt.plot(X, Y_iq, label="Interpolacja kwadratowa", color="blue")
    plt.plot(X, Y_ic, label="Interpolacja sześcienna", color="purple")
    if show_lagrange:
        plt.plot(X, Y_l, label="Interpolacja Lagrange'a", color="green")
    plt.scatter(Xn, Yn, label="Węzły", color="red")
    plt.xlabel("x")
    plt.ylabel("y")
    plt.legend()
    plt.show()


def quadraticCoeff(X, Y, bct):
    n = len(X) - 1
    numOfCoeffs = 3

    # fi(x) = F(x)
    A = np.zeros(shape=(numOfCoeffs * n, numOfCoeffs * n))
    for i in range(0, n * 2, 2):
        idx = i // 2
        # print(i, idx*n)
        A[i][idx * numOfCoeffs] = X[idx] ** 2
        A[i][idx * numOfCoeffs + 1] = X[idx]
        A[i][idx * numOfCoeffs + 2] = 1

        A[i + 1][idx * numOfCoeffs] = X[idx + 1] ** 2
        A[i + 1][idx * numOfCoeffs + 1] = X[idx + 1]
        A[i + 1][idx * numOfCoeffs + 2] = 1

    # pierwsze pochodne
    for i in range(n * 2, n * 3 - 1):
        j = i - n * 2
        idx = j + 1

        A[i][j * numOfCoeffs] = 2 * X[idx]
        A[i][j * numOfCoeffs + numOfCoeffs] = -2 * X[idx]

        A[i][j * numOfCoeffs + 1] = 1
        A[i][j * numOfCoeffs + 1 + numOfCoeffs] = -1

    # warunki graniczne
    if bct == "n":
        A[-1][-numOfCoeffs] = 2
    else:
        A[-1][0] = 2 * X[0]
        A[-1][1] = 1

        A[-1][-numOfCoeffs] = -2 * X[-1]
        A[-1][-numOfCoeffs + 1] = 1

    # druga strona równania
    F = np.zeros(shape=(numOfCoeffs * n, 1))
    F[0] = Y[0]
    F[n * 2 - 1] = Y[-1]
    for i in range(1, n * 2 - 1, 2):
        F[i] = Y[i // 2 + 1]
        F[i + 1] = Y[i // 2 + 1]

    reversedA = np.linalg.inv(A)

    # współczynniki odpowiednich wielomianów (coefficients)

    coeff = reversedA.dot(F)
    A = []
    B = []
    C = []
    for i in range(numOfCoeffs * n):
        if i % numOfCoeffs == 0:
            A.append(coeff[i][0])
        elif i % numOfCoeffs == 1:
            B.append(coeff[i][0])
        elif i % numOfCoeffs == 2:
            C.append(coeff[i][0])

    return A, B, C


def quadraticSpline(X, Y, bct):
    A, B, C = quadraticCoeff(X, Y, bct)
    step = X[1] - X[0]
    XI = np.linspace(0.5, 4, num=N)
    YI = []
    for xx in XI:
        i = int((xx - X[0]) // step)
        if i == len(A):
            i -= 1
        YI.append(A[i] * xx ** 2 + B[i] * xx + C[i])

    return YI


def cubicSpline(X, Y, bct):
    A, B, C, D = cubicCoefficients(X, Y, bct)
    step = X[1] - X[0]
    XI = np.linspace(0.5, 4, num=N)
    YI = []
    for xx in XI:
        i = int((xx - X[0]) // step)
        if i == len(A):
            i -= 1
        YI.append(A[i] * xx ** 3 + B[i] * xx ** 2 + C[i] * xx + D[i])

    return YI


def cubicCoefficients(X, Y, bct):
    n = len(X) - 1
    numOfCoeffs = 4
    # fi(x) = F(x)
    A = np.zeros(shape=(4 * n, 4 * n))
    for i in range(0, n * 2, 2):
        idx = i // 2
        A[i][idx * numOfCoeffs] = X[idx] ** 3
        A[i][idx * numOfCoeffs + 1] = X[idx] ** 2
        A[i][idx * numOfCoeffs + 2] = X[idx]
        A[i][idx * numOfCoeffs + 3] = 1

        A[i + 1][idx * numOfCoeffs] = X[idx + 1] ** 3
        A[i + 1][idx * numOfCoeffs + 1] = X[idx + 1] ** 2
        A[i + 1][idx * numOfCoeffs + 2] = X[idx + 1]
        A[i + 1][idx * numOfCoeffs + 3] = 1

    # pierwsze pochodne
    for i in range(n * 2, n * 3 - 1):
        j = i - n * 2
        idx = j + 1

        A[i][j * numOfCoeffs] = 3 * X[idx] ** 2
        A[i][j * numOfCoeffs + numOfCoeffs] = -3 * X[idx] ** 2

        A[i][j * numOfCoeffs + 1] = 2 * X[idx]
        A[i][j * numOfCoeffs + 1 + numOfCoeffs] = -2 * X[idx]

        A[i][j * numOfCoeffs + 2] = 1
        A[i][j * numOfCoeffs + 2 + numOfCoeffs] = -1

    # drugie pochodne
    for i in range(n * 3 - 1, n * 4 - 2):
        j = i - (n * 3 - 1)
        idx = j + 1

        A[i][j * numOfCoeffs] = 6 * X[idx]
        A[i][j * numOfCoeffs + numOfCoeffs] = -6 * X[idx]

        A[i][j * numOfCoeffs + 1] = 2
        A[i][j * numOfCoeffs + 1 + numOfCoeffs] = -2

    # warunki graniczne

    if bct == "n":  # naturalny
        A[-2][0] = 6 * X[0]
        A[-2][1] = 2

        A[-1][-numOfCoeffs] = 6 * X[-1]
        A[-1][-numOfCoeffs + 1] = 2

    elif bct == "p":  # periodyczny
        A[-2][0] = 3 * X[0] ** 2
        A[-2][1] = 2 * X[0]
        A[-2][2] = 1

        A[-1][-numOfCoeffs] = 3 * X[-1] ** 2
        A[-1][-numOfCoeffs + 1] = 2 * X[-1]
        A[-1][-numOfCoeffs + 2] = 1

    # współczynniki odpowiednich wielomianów (coefficients)
    coeff = np.ones(shape=(4 * n, 1))

    # druga strona równania
    F = np.zeros(shape=(4 * n, 1))
    F[0] = Y[0]
    F[n * 2 - 1] = Y[-1]
    for i in range(1, n * 2 - 1, 2):
        F[i] = Y[i // 2 + 1]
        F[i + 1] = Y[i // 2 + 1]

    reversedA = np.linalg.inv(A)

    coeff = reversedA.dot(F)
    A = []
    B = []
    C = []
    D = []
    for i in range(4 * n):
        if i % numOfCoeffs == 0:
            A.append(coeff[i][0])
        elif i % numOfCoeffs == 1:
            B.append(coeff[i][0])
        elif i % numOfCoeffs == 2:
            C.append(coeff[i][0])
        elif i % numOfCoeffs == 3:
            D.append(coeff[i][0])
    return A, B, C, D


def f(x):
    return x * np.sin(4 * np.pi / x)


spline(5, "n")
