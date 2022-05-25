import numpy as np
import pandas as pd


def f(X):
    result = np.zeros(3)
    result[0] = X[0] ** 2 + X[1] ** 2 + X[2]
    result[1] = 2 * X[0] ** 2 - X[1] ** 2 - 4 * X[2] ** 2
    result[2] = X[0] ** 2 + X[1] + X[2]

    return result


def jacobian(X):
    return [
        [2 * X[0], 2 * X[1], 1],
        [2 * X[0], -2 * X[1], -12 * X[2]],
        [2 * X[0], 1, 1]
    ]


def newton_matrix(func, jacob, ends, epsilon, stop_condition):
    X = np.array(ends)
    max_iter = 0
    for i in range(500):
        X_copy = np.copy(X)
        try:
            S = np.linalg.solve(jacob(X), func(X))
        except np.linalg.LinAlgError:
            return None, None
        X = X - S
        max_iter += 1
        if stop_condition == "diff" and np.linalg.norm(X - X_copy) < epsilon:
            return X, max_iter
        elif stop_condition == "func" and np.linalg.norm(func(X)) < epsilon:
            return X, max_iter
    return None, None


def newton(stop_condition):
    vectors = [-1, -0.6, -0.2, 0.2, 0.6, -10]
    epsilon = [0.0001, 0.00001, 0.000001]
    result = []
    for end_1 in vectors:
        for end_2 in vectors:
            for end_3 in vectors:
                for e in epsilon:
                    x, max_iter = newton_matrix(f, jacobian, [end_1, end_2, end_3], e, stop_condition)
                    print(x, end_1, end_2, end_3, f"iter: {max_iter}")
                    result += [end_1, end_2, end_3, e, max_iter]
    df = pd.DataFrame(data={"end 1": result[::5],
                            "end 2": result[1::5],
                            "end 3": result[2::5],
                            "epsilon": result[3::5],
                            "num of iterations": result[4::5]})
    return df


def get_sum(a, b):
    result = a + b
    if a == b:
        return a
    elif a < b:
        for i in range(a + 1, b):
            result += i
    else:
        for i in range(b + 1, a):
            result += i
    return result


def row_sum_odd_numbers(n):
    if n == 1:
        return 1

    bottom_bound, result = 1, 0
    for i in range(n - 1):
        bottom_bound += 2 * (i + 1)
    for i in range(bottom_bound, 2 * (n - 1) + bottom_bound + 1, 2):
        result += i

    return result


import re


def to_camel_case(text):
    splitted_text = re.split('-|_', text)
    result = splitted_text[0]
    for letter in splitted_text[1:]:
        result += letter.capitalize()
    return result


def create_phone_number(n):
    return f'({n[0]}{n[1]}{n[2]}) {n[3]}{n[4]}{n[5]}-{n[6]}{n[7]}{n[8]}'


def duplicate_encode(word):
    word = word.lower()
    counters = [0 for _ in range(ord('z') - ord('a'))]
    for letter in word:
        counters[ord(letter) - ord('a')] += 1
    result = ''
    for i in range(len(word)):
        if counters[ord(word[i]) - ord('a')] > 1:
            result += ')'
        else:
            result += '('
    return result


def accum(s):
    result = ''
    for i, letter in enumerate(s):
        for j in range(i + 1):
            if j == 0:
                result += letter.upper()
            else:
                result += letter
        if i != len(s) - 1:
            result += '-'
    return result


if __name__ == '__main__':
    a = 'ZpglnRxqenU'
    print(accum(a))
