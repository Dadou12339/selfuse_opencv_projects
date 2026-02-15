#include <iostream>
#include <Eigen/Dense>
#include <cstdio>
#include <vector>

using namespace Eigen;

void readData(const char *path, std::vector<float> &datas){
    FILE *file = fopen(path, "r");

    char buffer[256];
    while(fgets(buffer, sizeof(buffer), file)){
        float num = atof(buffer);
        datas.push_back(num);
    }

    fclose(file);
}

class KalmanFilter{
public:
    KalmanFilter(){
        x = Matrix<float, 2, 1>::Zero();
        A << 1.0f, 1.0f,
             0.0f, 1.0f;

        P = Matrix<float, 2, 2>::Identity();
        Q = Matrix<float, 2, 2>::Identity() * 0.1f;

        H << 1.0f, 0.0f;
        R << 1.0f;
    }

    MatrixXf predict(){
        x = A * x;
        P = A * P * A.transpose() + Q;

        return x;
    }

    MatrixXf update(float z){
        Matrix<float, 2, 1> K = P * H.transpose() * (H * P * H.transpose() + R).inverse();
        x = x + K * (z - H * x);
        Matrix<float, 2, 2> I = Matrix<float, 2, 2>::Identity();
        P = (I - K * H) * P;

        return x;
    }

private:
    Matrix<float, 2, 1> x;
    Matrix<float, 2, 2> A;

    Matrix<float, 2, 2> P;
    Matrix<float, 2, 2> Q;

    Matrix<float, 2, 1> K;
    Matrix<float, 1, 2> H;
    Matrix<float, 1, 1> R;
};

int main(){
    std::vector<float> data;
    readData("../data/dollar.txt", data);

    KalmanFilter kalman;
    std::vector<float> data_est;
    for (int i=0, sz=data.size(); i<sz; i++){
        float num = kalman.predict()(0);
        data_est.push_back(num);
        kalman.update(data[i]);
    }

    for (int i=0; i<10; i++){
        float num = kalman.predict()(0);

        std::cout << num << " ";

        data_est.push_back(num);
    }

    return 0;
}