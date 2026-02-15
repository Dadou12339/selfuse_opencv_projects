import pandas as pd
import matplotlib.pyplot as plt
import numpy as np
import numpy.linalg as linalg

data = pd.read_csv("../data/dollar.txt", header=None)
dollar = np.array(data.iloc[:, 0], dtype=np.float32)
day = np.arange(1, 31, dtype=np.uint32)

class KalmanFilter:
    def __init__(self):
        self.x = np.array([dollar[0], 0], dtype=np.float32).reshape(2, 1)
        self.A = np.array([[1, 1], [0, 1]], dtype=np.float32)
        self.B = np.array([[0, 0], [0, 0]], dtype = np.float32)

        self.P = np.eye(2, dtype=np.float32)
        self.Q = np.eye(2, dtype=np.float32) * 0.1
        self.R = np.array([0.1], dtype=np.float32).reshape(1, 1)
        self.H = np.array([1, 0], dtype=np.float32).reshape(1, 2)

    def predict(self, u=0):
        self.x = self.A @ self.x
        self.P = self.A @ self.P @ self.A.T + self.Q
        return self.x

    def update(self, z):
        K = self.P @ self.H.T @ linalg.inv(self.H @ self.P @ self.H.T + self.R)
        self.x = self.x + K @ (z - self.H @ self.x)
        I = np.eye(2, dtype=np.float32)
        self.P = (I - K @ self.H) @ self.P

def main():
    kalman_filter = KalmanFilter()

    dollar_est = []
    for z in dollar:
        kalman_filter.predict()
        kalman_filter.update(z)
        dollar_est.append(kalman_filter.x[0, 0])

    # for i in range(10):
    #     kalman_filter.predict()
    #     dollar_est.append(kalman_filter.x[0, 0])

    dollar_est = np.array(dollar_est, dtype=np.float32)

    plt.plot(day, dollar, color="red", label="observe")
    plt.plot(day, dollar_est, color="green", label="estimate")
    plt.xlabel("day")
    plt.ylabel("dollar")

    plt.legend(loc="best")
    plt.show()

if __name__ == "__main__":
    main()