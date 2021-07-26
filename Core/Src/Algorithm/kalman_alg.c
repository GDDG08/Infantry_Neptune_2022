/*
 *  Project      : Infantry_Neptune
 * 
 *  file         : kalman_alg.c
 *  Description  : This file contains the kalman filter functions (whx designed)
 *  LastEditors  : 強秤惻卸赱強伉
 *  Date         : 2021-06-10 11:02:50
 *  LastEditTime : 2021-07-21 20:38:42
 */

#include "kalman_alg.h"
#include "string.h"
#include "stdio.h"
#include "stdlib.h"

uint16_t sizeof_float, sizeof_double;

/**
  * @brief      Initialization of Kalman filter
  * @param      kf :Structure pointer of Kalman filter
  * @param      xhatSize :State variable matrix size
  * @param      uSize :Control matrix size
  * @param      zSize :Observation matrix size
  * @retval     NULL
  */
void Kalman_FilterInit(Kalman_KalmanTypeDef* kf, uint8_t xhatSize, uint8_t uSize, uint8_t zSize) {
    sizeof_float = sizeof(float);
    sizeof_double = sizeof(double);

    kf->xhatSize = xhatSize;
    kf->uSize = uSize;
    kf->zSize = zSize;

    kf->MeasurementValidNum = 0;

    // measurement flags
    kf->MeasurementMap = (uint8_t*)malloc(sizeof(uint8_t) * zSize);
    memset(kf->MeasurementMap, 0, sizeof(uint8_t) * zSize);
    kf->MeasurementDegree = (float*)malloc(sizeof_float * zSize);
    memset(kf->MeasurementDegree, 0, sizeof_float * zSize);
    kf->MatR_DiagonalElements = (float*)malloc(sizeof_float * zSize);
    memset(kf->MatR_DiagonalElements, 0, sizeof_float * zSize);
    kf->StateMinVariance = (float*)malloc(sizeof_float * xhatSize);
    memset(kf->StateMinVariance, 0, sizeof_float * xhatSize);
    kf->temp = (uint8_t*)malloc(sizeof(uint8_t) * zSize);
    memset(kf->temp, 0, sizeof(uint8_t) * zSize);

    // filter data
    kf->FilteredValue = (float*)malloc(sizeof_float * xhatSize);
    memset(kf->FilteredValue, 0, sizeof_float * xhatSize);
    kf->MeasuredVector = (float*)malloc(sizeof_float * zSize);
    memset(kf->MeasuredVector, 0, sizeof_float * zSize);
    kf->ControlVector = (float*)malloc(sizeof_float * uSize);
    memset(kf->ControlVector, 0, sizeof_float * uSize);

    // xhat x(k|k)
    kf->xhat_data = (float*)malloc(sizeof_float * xhatSize);
    memset(kf->xhat_data, 0, sizeof_float * xhatSize);
    mat_init(&kf->xhat, kf->xhatSize, 1, (float*)kf->xhat_data);

    // xhatminus x(k|k-1)
    kf->xhatminus_data = (float*)malloc(sizeof_float * xhatSize);
    memset(kf->xhatminus_data, 0, sizeof_float * xhatSize);
    mat_init(&kf->xhatminus, kf->xhatSize, 1, (float*)kf->xhatminus_data);

    if (uSize != 0) {
        // control vector u
        kf->u_data = (float*)malloc(sizeof_float * uSize);
        memset(kf->u_data, 0, sizeof_float * uSize);
        mat_init(&kf->u, kf->uSize, 1, (float*)kf->u_data);
    }

    // measurement vector z
    kf->z_data = (float*)malloc(sizeof_float * zSize);
    memset(kf->z_data, 0, sizeof_float * zSize);
    mat_init(&kf->z, kf->zSize, 1, (float*)kf->z_data);

    // covariance matrix P(k|k)
    kf->P_data = (float*)malloc(sizeof_float * xhatSize * xhatSize);
    memset(kf->P_data, 0, sizeof_float * xhatSize * xhatSize);
    mat_init(&kf->P, kf->xhatSize, kf->xhatSize, (float*)kf->P_data);

    //create covariance matrix P(k|k-1)
    kf->Pminus_data = (float*)malloc(sizeof_float * xhatSize * xhatSize);
    memset(kf->Pminus_data, 0, sizeof_float * xhatSize * xhatSize);
    mat_init(&kf->Pminus, kf->xhatSize, kf->xhatSize, (float*)kf->Pminus_data);

    // state transition matrix F FT
    kf->F_data = (float*)malloc(sizeof_float * xhatSize * xhatSize);
    kf->FT_data = (float*)malloc(sizeof_float * xhatSize * xhatSize);
    memset(kf->F_data, 0, sizeof_float * xhatSize * xhatSize);
    memset(kf->FT_data, 0, sizeof_float * xhatSize * xhatSize);
    mat_init(&kf->F, kf->xhatSize, kf->xhatSize, (float*)kf->F_data);
    mat_init(&kf->FT, kf->xhatSize, kf->xhatSize, (float*)kf->FT_data);

    if (uSize != 0) {
        // control matrix B
        kf->B_data = (float*)malloc(sizeof_float * xhatSize * uSize);
        memset(kf->B_data, 0, sizeof_float * xhatSize * uSize);
        mat_init(&kf->B, kf->xhatSize, kf->uSize, (float*)kf->B_data);
    }

    // measurement matrix H
    kf->H_data = (float*)malloc(sizeof_float * zSize * xhatSize);
    kf->HT_data = (float*)malloc(sizeof_float * xhatSize * zSize);
    memset(kf->H_data, 0, sizeof_float * zSize * xhatSize);
    memset(kf->HT_data, 0, sizeof_float * xhatSize * zSize);
    mat_init(&kf->H, kf->zSize, kf->xhatSize, (float*)kf->H_data);
    mat_init(&kf->HT, kf->xhatSize, kf->zSize, (float*)kf->HT_data);

    // process noise covariance matrix Q
    kf->Q_data = (float*)malloc(sizeof_float * xhatSize * xhatSize);
    memset(kf->Q_data, 0, sizeof_float * xhatSize * xhatSize);
    mat_init(&kf->Q, kf->xhatSize, kf->xhatSize, (float*)kf->Q_data);

    // measurement noise covariance matrix R
    kf->R_data = (float*)malloc(sizeof_float * zSize * zSize);
    memset(kf->R_data, 0, sizeof_float * zSize * zSize);
    mat_init(&kf->R, kf->zSize, kf->zSize, (float*)kf->R_data);

    // kalman gain K
    kf->K_data = (float*)malloc(sizeof_float * xhatSize * zSize);
    memset(kf->K_data, 0, sizeof_float * xhatSize * zSize);
    mat_init(&kf->K, kf->xhatSize, kf->zSize, (float*)kf->K_data);

    kf->S_data = (float*)malloc(sizeof_float * kf->xhatSize * kf->xhatSize);
    kf->temp_matrix_data = (float*)malloc(sizeof_float * kf->xhatSize * kf->xhatSize);
    kf->temp_matrix_data1 = (float*)malloc(sizeof_float * kf->xhatSize * kf->xhatSize);
    kf->temp_vector_data = (float*)malloc(sizeof_float * kf->xhatSize);
    kf->temp_vector_data1 = (float*)malloc(sizeof_float * kf->xhatSize);
    mat_init(&kf->S, kf->xhatSize, kf->xhatSize, (float*)kf->S_data);
    mat_init(&kf->temp_matrix, kf->xhatSize, kf->xhatSize, (float*)kf->temp_matrix_data);
    mat_init(&kf->temp_matrix1, kf->xhatSize, kf->xhatSize, (float*)kf->temp_matrix_data1);
    mat_init(&kf->temp_vector, kf->xhatSize, 1, (float*)kf->temp_vector_data);
    mat_init(&kf->temp_vector1, kf->xhatSize, 1, (float*)kf->temp_vector_data1);

    kf->SkipEq1 = 0;
    kf->SkipEq2 = 0;
    kf->SkipEq3 = 0;
    kf->SkipEq4 = 0;
    kf->SkipEq5 = 0;
}

/**
  * @brief      Update data with Kalman filter
  * @param      kf :Structure pointer of Kalman filter
  * @retval     Filtered data pointer
  */
float* Kalman_FilterUpdate(Kalman_KalmanTypeDef* kf) {
    // matrix H K R auto adjustment
    if (kf->UseAutoAdjustment != 0)
        Kalman_Adjustment_H_K_R(kf);
    else {
        memcpy(kf->z_data, kf->MeasuredVector, sizeof_float * kf->zSize);
        memset(kf->MeasuredVector, 0, sizeof_float * kf->zSize);
    }

    memcpy(kf->u_data, kf->ControlVector, sizeof_float * kf->uSize);

    if (kf->User_Func0_f != NULL)
        kf->User_Func0_f(kf);

    // 1. xhat'(k)= A，xhat(k-1) + B，u
    if (!kf->SkipEq1) {
        if (kf->uSize > 0) {
            kf->MatStatus = mat_mult(&kf->F, &kf->xhat, &kf->temp_vector);
            kf->temp_vector1.numRows = kf->xhatSize;
            kf->temp_vector1.numCols = 1;
            kf->MatStatus = mat_mult(&kf->B, &kf->u, &kf->temp_vector1);
            kf->MatStatus = mat_add(&kf->temp_vector, &kf->temp_vector1, &kf->xhatminus);
        } else {
            kf->MatStatus = mat_mult(&kf->F, &kf->xhat, &kf->xhatminus);
        }
    }

    if (kf->User_Func1_f != NULL)
        kf->User_Func1_f(kf);

    // 2. P'(k) = A，P(k-1)，AT + Q
    if (!kf->SkipEq2) {
        kf->MatStatus = mat_trans(&kf->F, &kf->FT);
        kf->MatStatus = mat_mult(&kf->F, &kf->P, &kf->Pminus);
        kf->temp_matrix.numRows = kf->Pminus.numRows;
        kf->temp_matrix.numCols = kf->FT.numCols;
        kf->MatStatus = mat_mult(&kf->Pminus, &kf->FT, &kf->temp_matrix);  // temp_matrix = F P(k-1) FT
        kf->MatStatus = mat_add(&kf->temp_matrix, &kf->Q, &kf->Pminus);
    }

    if (kf->User_Func2_f != NULL)
        kf->User_Func2_f(kf);

    if (kf->MeasurementValidNum != 0 || kf->UseAutoAdjustment == 0) {
        // 3. K(k) = P'(k)，HT / (H，P'(k)，HT + R)
        if (!kf->SkipEq3) {
            kf->MatStatus = mat_trans(&kf->H, &kf->HT);  //z|x => x|z
            kf->temp_matrix.numRows = kf->H.numRows;
            kf->temp_matrix.numCols = kf->Pminus.numCols;
            kf->MatStatus = mat_mult(&kf->H, &kf->Pminus, &kf->temp_matrix);  //temp_matrix = H，P'(k)
            kf->temp_matrix1.numRows = kf->temp_matrix.numRows;
            kf->temp_matrix1.numCols = kf->HT.numCols;
            kf->MatStatus = mat_mult(&kf->temp_matrix, &kf->HT, &kf->temp_matrix1);  //temp_matrix1 = H，P'(k)，HT
            kf->S.numRows = kf->R.numRows;
            kf->S.numCols = kf->R.numCols;
            kf->MatStatus = mat_add(&kf->temp_matrix1, &kf->R, &kf->S);  //S = H P'(k) HT + R
            kf->MatStatus = mat_inv(&kf->S, &kf->temp_matrix1);          //temp_matrix1 = inv(H，P'(k)，HT + R)
            kf->temp_matrix.numRows = kf->Pminus.numRows;
            kf->temp_matrix.numCols = kf->HT.numCols;
            kf->MatStatus = mat_mult(&kf->Pminus, &kf->HT, &kf->temp_matrix);  //temp_matrix = P'(k)，HT
            kf->MatStatus = mat_mult(&kf->temp_matrix, &kf->temp_matrix1, &kf->K);
        }

        if (kf->User_Func3_f != NULL)
            kf->User_Func3_f(kf);

        // 4. xhat(k) = xhat'(k) + K(k)，(z(k) - H，xhat'(k))
        if (!kf->SkipEq4) {
            kf->temp_vector.numRows = kf->H.numRows;
            kf->temp_vector.numCols = 1;
            kf->MatStatus = mat_mult(&kf->H, &kf->xhatminus, &kf->temp_vector);  //temp_vector = H xhat'(k)
            kf->temp_vector1.numRows = kf->z.numRows;
            kf->temp_vector1.numCols = 1;
            kf->MatStatus = mat_sub(&kf->z, &kf->temp_vector, &kf->temp_vector1);  //temp_vector1 = z(k) - H，xhat'(k)
            kf->temp_vector.numRows = kf->K.numRows;
            kf->temp_vector.numCols = 1;
            kf->MatStatus = mat_mult(&kf->K, &kf->temp_vector1, &kf->temp_vector);  //temp_vector = K(k)，(z(k) - H，xhat'(k))
            kf->MatStatus = mat_add(&kf->xhatminus, &kf->temp_vector, &kf->xhat);
        }

        if (kf->User_Func4_f != NULL)
            kf->User_Func4_f(kf);

        // 5. P(k) = (1-K(k)，H)，P'(k) ==> P(k) = P'(k)-K(k)，H，P'(k)
        if (!kf->SkipEq5) {
            kf->temp_matrix.numRows = kf->K.numRows;
            kf->temp_matrix.numCols = kf->H.numCols;
            kf->temp_matrix1.numRows = kf->temp_matrix.numRows;
            kf->temp_matrix1.numCols = kf->Pminus.numCols;
            kf->MatStatus = mat_mult(&kf->K, &kf->H, &kf->temp_matrix);                  //temp_matrix = K(k)，H
            kf->MatStatus = mat_mult(&kf->temp_matrix, &kf->Pminus, &kf->temp_matrix1);  //temp_matrix1 = K(k)，H，P'(k)
            kf->MatStatus = mat_sub(&kf->Pminus, &kf->temp_matrix1, &kf->P);
        }
    } else {
        // xhat(k) = xhat'(k)
        // P(k) = P'(k)
        memcpy(kf->xhat_data, kf->xhatminus_data, sizeof_float * kf->xhatSize);
        memcpy(kf->P_data, kf->Pminus_data, sizeof_float * kf->xhatSize * kf->xhatSize);
    }

    if (kf->User_Func5_f != NULL)
        kf->User_Func5_f(kf);

    // suppress filter excessive convergence
    for (uint8_t i = 0; i < kf->xhatSize; i++) {
        if (kf->P_data[i * kf->xhatSize + i] < kf->StateMinVariance[i])
            kf->P_data[i * kf->xhatSize + i] = kf->StateMinVariance[i];
    }

    if (kf->UseAutoAdjustment != 0) {
        memset(kf->R_data, 0, sizeof_float * kf->zSize * kf->zSize);
        memset(kf->H_data, 0, sizeof_float * kf->xhatSize * kf->zSize);
    }

    memcpy(kf->FilteredValue, kf->xhat_data, sizeof_float * kf->xhatSize);

    if (kf->User_Func6_f != NULL)
        kf->User_Func6_f(kf);

    return kf->FilteredValue;
}

/**
  * @brief      Auto adjust H K R matrix
  * @param      kf :Structure pointer of Kalman filter
  * @retval     NULL
  */
static void Kalman_Adjustment_H_K_R(Kalman_KalmanTypeDef* kf) {
    kf->MeasurementValidNum = 0;

    memcpy(kf->z_data, kf->MeasuredVector, sizeof_float * kf->zSize);
    memset(kf->MeasuredVector, 0, sizeof_float * kf->zSize);

    // recognize measurement validity and adjust matrices H R K
    for (uint8_t i = 0; i < kf->zSize; i++) {
        if (kf->z_data[i] != 0) {
            // rebuild vector z
            kf->z_data[kf->MeasurementValidNum] = kf->z_data[i];
            kf->temp[kf->MeasurementValidNum] = i;
            // rebuild matrix H
            kf->H_data[kf->xhatSize * kf->MeasurementValidNum + kf->MeasurementMap[i] - 1] = kf->MeasurementDegree[i];
            kf->MeasurementValidNum++;
        }
    }
    for (uint8_t i = 0; i < kf->MeasurementValidNum; i++) {
        // rebuild matrix R
        kf->R_data[i * kf->MeasurementValidNum + i] = kf->MatR_DiagonalElements[kf->temp[i]];
    }

    // adjust the dimensions of system matrices
    kf->H.numRows = kf->MeasurementValidNum;
    kf->H.numCols = kf->xhatSize;
    kf->HT.numRows = kf->xhatSize;
    kf->HT.numCols = kf->MeasurementValidNum;
    kf->R.numRows = kf->MeasurementValidNum;
    kf->R.numCols = kf->MeasurementValidNum;
    kf->K.numRows = kf->xhatSize;
    kf->K.numCols = kf->MeasurementValidNum;
    kf->z.numRows = kf->MeasurementValidNum;
}
