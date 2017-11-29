/**
 * @brief A header for the predict class which contains logistic regression training and prediction methods
 * @author Gurkiran Tatla
 * @author Jake Schindler
 * @author Justine Kim
 * @author Paul Salvatore
 * @author Timal Peramune
 */

#ifndef PREDICT_H
#define PREDICT_H

#include <iostream>
#include <armadillo>
#include <stdlib.h>

#define LAMBDA 1
#define THRESHOLD 0.6
#define CERTAINTY_THRESHOLD 0.7
#define DEGREE 5
#define MAPPED_NUM 21

class Predict {
    public:
        Predict();

        virtual ~Predict();
        
        static arma::mat trainAndPredict(int lightId, int machineLearning);

        static int checkConfidence(int lightId);

    private:
    	static arma::mat mapFeature(arma::mat X1, arma::mat X2);

		static arma::mat sigmoid(arma::mat *theta, arma::mat *X, unsigned long m);

		static arma::mat predict(arma::mat *theta, arma::mat *X);

		static arma::mat normalEquation(arma::mat *X, arma::mat *y);

		static arma::mat generateLightMatrix(int lightId);

		static arma::mat generateWeekOfX();

		static arma::mat predictByFrequency(arma::mat *data);

		static double fscore(arma::mat *y_predicted, arma::mat *y_test);
};

#endif

