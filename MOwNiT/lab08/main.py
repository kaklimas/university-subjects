import time as t
import pylab as plt
import numpy as np

A = [200, 1000, 5000, 10000, 20000]
B = [0.003, 0.01, 0.04, 0.102, 0.193]
C = [0.015, 0.23, 4.611, 23.086, 124.88]

plt.plot(A, B, label="Algorytm Gaussa")
plt.plot(A, C, label="Metoda Gaussa")
plt.title("Czas wykonywania się metody w zależnosci od metody oraz wielkości macierzy")
plt.xlabel("Wartość n")
plt.ylabel("Czas w sekundach")
plt.legend()
plt.show()