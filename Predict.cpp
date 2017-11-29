
/**
 * @brief The Predict class is used to train a logistic regression algorithm using a specific lights state data to predict future scheduling state events.
 * @brief If the algorithm does not perform at a high enough level of certainty, than the Predict class will opt to use a most-frequent algorithm to decide scheduling.
 * @author Gurkiran Tatla
 * @author Jake Schindler
 * @author Justine Kim
 * @author Paul Salvatore
 * @author Timal Peramune
 */

#include "Predict.hpp"


/**
 * Predict constructor.
 * There are no parameters/variables to initalize for this class so the constructor does nothing.
 */
Predict::Predict(){
    return;
}


/**
 * Predict deconstructor.
 * There are no class variables to destroy for this class so the deconstructor does nothing.
 */
Predict::~Predict(){
    return;
}


/**
 * Maps the features X1 and X2 polynomially for more complex fitting.
 *
 * @param X1: m x 1 vector where m is number of training examples (first feature column)
 * @param X2: m x 1 vector where m is number of training examples (second feature column)
 * @return a vector of size m x 21 representing polynomial mapped features X1 and X2
 */
arma::mat Predict::mapFeature(arma::mat X1, arma::mat X2){
    arma::mat out(size(X1, 0), MAPPED_NUM);

    int column = 0;
    for(int i = 0; i <= DEGREE; i++){
        for(int j = 0; j <= i; j++){
            out.col(column++) = ( pow(X1, i-j) % pow(X2, j) );
        }
    }

    return out;
}

/**
 * Computes the sigmoid value of each training example in X parameterized with theta
 *
 * @param theta: (n + 1) x 1 vector
 * @param X: m x (n+1) vector where m is number of training examples and n is the number of features
 * @param m equivallent to the number of training examples in X
 * @return a vector of size m x 1 representing the sigmoid function applied to each value of z
 */
arma::mat Predict::sigmoid(arma::mat *theta, arma::mat *X, unsigned long m){

    arma::mat z = *theta * *X;

    arma::mat s(m, 1);

    for(int i = 0; i < m; i++){
        s(i, 0) = ( 1.0/(1.0 + exp(-1.0 * z(i, 0))) );
    }

    return s;
}


/**
 * Makes a prediction based on the sigmoid values for a weeks worth of x points.
 *
 * @param theta: (n + 1) x 1 vector
 * @param X: m x (n+1) vector where m is number of predictions to make and n is the number of features
 * @param threshold is a double representing the threshold from which to consider the output positive
 * @return the prediction for each of the values in X parameterized with theta: m x 1 vector
 */
arma::mat Predict::predict(arma::mat *theta, arma::mat *X){

    unsigned long m = arma::size(*X, 0);
    arma::mat p = arma::zeros(m, 1);

    arma::mat sigmoids = sigmoid(X, theta, m);

    for(int i = 0; i < m; i++){
        if( sigmoids(i, 0) >= THRESHOLD ){
            p(i, 0) = 1;
        }
    }

    return p;
}


/**
 * Computes the theta values for the matrix using the normal equation.
 *
 * @param X: m x (n+1) matrix, where m is number of training examples and n is the number of features
 * @param y: m x 1 vector
 * @param lambda the regularization parameter
 * @return the minimized values of theta: (n+1) x 1 vector
 */
arma::mat Predict::normalEquation(arma::mat *X, arma::mat *y){
    arma::mat X_transpose = X->t();

    unsigned long n = arma::size(*X, 1);
    arma::mat normalMatrix = arma::eye<arma::mat>(n, n);
    normalMatrix(0, 0) = 0;

    return ( arma::pinv(X_transpose * *X + LAMBDA * normalMatrix) * X_transpose * *y );
}


/**
 * Builds and returns a matrix representing the state information for a particular light.
 *
 * @param lightId: the id of the light for which we are getting all state information for
 * @return an matrix representation where the rows represent each data point, col1: day, col2: hour, col3: state (1 = on, 0 = off)
 */
arma::mat Predict::generateLightMatrix(int lightId){
    arma::mat data;
    data.load("./lightData" + std::to_string(lightId) + ".txt");
    return data;
}

/**
 * Builds a matrix of x parameters to be used for predicting the next week of state shceduling.
 *
 * @return a matrix representing a week of x values for oru scheduling prediction algorithm
 */
arma::mat Predict::generateWeekOfX(){
    arma::mat x_predict(24*7, 2);
    int pos = 0;
    for(int day = 1; day < 8; day++){
        for(int hour = 0; hour < 24; hour++) {
            x_predict(pos, 0) = day;
            x_predict(pos, 1) = hour;
            pos++;
        }
    }

    return mapFeature(x_predict.col(0), x_predict.col(1));
}


/*
 * Predicts a weeks worth of scheduling based ont he most frequent past states for each time.
 * To be used when the logisitic regression algorithm is not confident enough.
 *
 * @param data: the matrix representation of the state data for the light which we want to predict
 * @return a matrix representing a week of scheduling
 */
arma::mat Predict::predictByFrequency(arma::mat *data){
	arma::mat positive = arma::zeros(24 * 7, 1);
	arma::mat negative = arma::zeros(24 * 7, 1);
	arma::mat prediction = arma::zeros(24 * 7, 1);
	
	for(int i = 0; i < arma::size(*data, 0); i++){
		if((*data).at(i, 2) == 1){
			positive.at((((*data).at(i, 0)) - 1) * 24 + (*data).at(i, 1), 0) += 1;
		} else {
			negative.at((((*data).at(i, 0)) - 1) * 24 + (*data).at(i, 1), 0) += 1;
		}
	}

	for(int i = 0; i < 24 * 7; i++){
		if(positive.at(i, 0) >  negative.at(i, 0)){
			prediction.at(i, 0) = 1;
		}
	}

	return prediction;
}

/**
 * Decides which prediction algorithm to use and exeutes the prediction.
 *
 * @param lightId the id of a light to collect all data for, train theta, and predict a weeks worth of scheduling
 * @return a matrix representing a week of scheduling
 */
arma::mat Predict::trainAndPredict(int lightId, int machineLearning){

	// Data does not need to be randomized since we are using all of it
    arma::mat data = generateLightMatrix(lightId);

	// We predict using logistic regression dependant on our confidence in our predictions
    if(machineLearning){

		arma::mat x_predict = generateWeekOfX();

    	// train and predict and then schedule
   		arma::mat y = data.col(2);
    	arma::mat X = mapFeature(data.col(0), data.col(1));
    	
    	arma::mat theta = normalEquation(&X, &y);

   		return predict(&theta, &x_predict);
    } else {

    	// predict based on most frequent for that day
    	return predictByFrequency(&data); 
    }
}

/**
 * Computes the F score for the test data to determine accuracy.
 *
 * @param the id of the light that we are checking the confidence for
 * @return an integer 1 or 0, respectively representing if our f score surpasses our confidence threshold
 */
double Predict::fscore(arma::mat *y_predicted, arma::mat *y_test){
	double true_postive = 0;
	double pred_positive = 0;
	double actual_positive = 0;
	
	// Counting the number of true, predicted, and actual positive values
	for(int i = 0; i < arma::size((*y_predicted), 0); i++){
		if((*y_predicted).at(i, 0) == 1){
			if((*y_test).at(i, 0) == 1){
				true_postive++;
				actual_positive++;
			}
			pred_positive++;
		} else if ((*y_test).at(i, 0) == 1){
			actual_positive++;
		}
	}

	// Returning lowest possible f score to avoid dividing by 0 error
	if(actual_positive == 0 || true_postive == 0){
		return 0;
	}

	double recall = true_postive / actual_positive;
	double precision = true_postive / pred_positive;

	// F score calculation
	return 2 * ( (precision * recall) / (precision + recall) );
}

/**
 * Splits a lights data into a training and test set to compute the accuracy of our
 * training algorithm with the available data.
 *
 * @param the id of the light that we are checking the confidence for
 * @return an integer 1 or 0, respectively representing if our f score surpasses our confidence threshold
 */
int Predict::checkConfidence(int lightId){

	arma::mat data = generateLightMatrix(lightId);

	// Randomizing the data for training
	data = arma::shuffle(data, 1);

	// Dividing into train (70%) and test (30%) sets
	int rowSize = arma::size(data, 0);
    arma::mat y_train = data.col(2).rows(0, std::floor(rowSize * 0.7));
    arma::mat y_test = data.col(2).rows(std::floor(rowSize * 0.7) + 1,  rowSize - 1);
    
    arma::mat X = mapFeature(data.col(0), data.col(1));
    arma::mat X_train = X.rows(0, std::floor(rowSize * 0.7));
    arma::mat X_test = X.rows(std::floor(rowSize * 0.7) + 1,  rowSize - 1);

    // Training theta with the training set
   	arma::mat theta = normalEquation(&X_train, &y_train);
   
   	// Predicting values for the test set
    arma::mat predicted =  predict(&theta, &X_test);

    // Checking how confidence we are
    return fscore(&predicted, &y_test) > CERTAINTY_THRESHOLD;
}

