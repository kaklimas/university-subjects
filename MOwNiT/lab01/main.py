import numpy as np
import pylab


def original_func_float():
    k_val = [np.float32(i) for i in range(1, 31)]
    xk_val = [np.float32(4)]
    xk = 4

    print("------------------------------\nFloat representation (original function)")
    print("1: 4.0")
    for k in k_val[:29]:
        xk = np.float32((2 ** (2 * (k + 1) + 1)) * ((np.sqrt(1 + (xk ** 2) / (2 ** (2 * (k + 1)))) - 1) / xk))
        xk_val.append(xk)
        if (int(k + 1) % 5 == 0):
            print(int(k + 1), ": ", xk)
    return k_val, xk_val


def original_func_double():
    k_val = [np.float64(i) for i in range(1, 31)]
    xk_val = [np.float64(4)]
    xk = 4

    print("------------------------------\nDouble representation (original function)")
    print("1: 4.0")
    for k in k_val[:29]:
        xk = np.float64(2 ** (2 * (k + 1) + 1) * (np.sqrt(1 + (xk ** 2 / 2 ** (2 * (k + 1)))) - 1) / xk)
        xk_val.append(xk)
        if (int(k + 1) % 5 == 0):
            print(int(k + 1), ": ", xk)
    return k_val, xk_val


def original_func_longdobule():
    k_val = [np.longdouble(i) for i in range(1, 31)]
    xk_val = [np.longdouble(4)]
    xk = 4

    print("------------------------------\nLong double representation (original function)")
    print("1: 4.0")
    for k in k_val[:29]:
        xk = np.longdouble(2 ** (2 * (k + 1) + 1) * (np.sqrt(1 + (xk ** 2 / 2 ** (2 * (k + 1)))) - 1) / xk)
        xk_val.append(xk)
        if (int(k + 1) % 5 == 0):
            print(int(k + 1), ": ", xk)
    return k_val, xk_val


def changed_float():
    k_val = [np.float32(i) for i in range(1, 31)]
    xk_val = [np.float32(4)]
    xk = 4
    print("------------------------------\nFloat representation (changed function)")
    print("1: 4.0")
    for k in k_val[:29]:
        xk = np.float32(2 * xk / (np.sqrt(1 + (xk ** 2 / 2 ** (2 * (k + 1)))) + 1))
        xk_val.append(xk)
        if (int(k + 1) % 5 == 0):
            print(int(k + 1), ": ", xk)

    return xk_val[29]


def changed_double():
    k_val = [np.float64(i) for i in range(1, 31)]
    xk_val = [np.float64(4)]
    xk = 4
    print("------------------------------\nDouble representation (changed function)")
    print("1: 4.0")
    for k in k_val[:29]:
        xk = np.float64(2 * xk / (np.sqrt(1 + (xk ** 2 / 2 ** (2 * (k + 1)))) + 1))
        xk_val.append(xk)
        if (int(k + 1) % 5 == 0):
            print(int(k + 1), ": ", xk)

    return xk_val[29]


def changed_longdouble():
    k_val = [np.longdouble(i) for i in range(1, 31)]
    xk_val = [np.longdouble(4)]
    xk = 4
    print("------------------------------\nLong double representation (changed function)")
    print("1: 4.0")
    for k in k_val[:29]:
        xk = np.longdouble(2 * xk / (np.sqrt(1 + (xk ** 2 / 2 ** (2 * (k + 1)))) + 1))
        xk_val.append(xk)
        if (int(k + 1) % 5 == 0):
            print(int(k + 1), ": ", xk)
    return xk_val[29]


def draw_graph(x, y):
    pylab.plot(x, y)
    pylab.show()


if __name__ == '__main__':
    # original_func_float()
    # original_func_double()
    # original_func_longdobule()
    #
    # print("\n----- modify function -----")
    # changed_float()
    # changed_double()
    # changed_longdouble()
    draw_graph([1, 2, 3], [1, 2, 3])
