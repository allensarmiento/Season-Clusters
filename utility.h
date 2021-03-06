#ifndef UTILITY_H
#define UTILITY_H
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "structs.h"

#define ARRAYSIZE 415

// Take in a csv filename
// Return a pointer to an array of weather objects
Weather* generateData(char fname[]) {
  char* token;
  char line[30];
  const char delim[2] = ",";
  int i = 0;

  // allocate mem to hold weather data
  Weather* weatherData = (Weather *)malloc(sizeof(Weather)*ARRAYSIZE);

  FILE* instream = fopen(fname, "r");

  // Retrieve each line until eof. Each line should contain no
  // more than 30 characters.
  while (i < ARRAYSIZE && fgets(line, 30, instream)) {
    Weather w;
    token = strtok(line, delim);
    w.month = atoi(token);

    // Validate proper months
    if (w.month != 1 && w.month != 7)
      continue;

    token = strtok(NULL, delim);
    w.temp = atof(token);
    token = strtok(NULL, delim);
    w.slp = atof(token);
    weatherData[i] = w;
    i++;
  }
  fclose(instream);
  return weatherData;
}

void sortParallel(Distance* d, int size) {
  int i, j;
  Distance temp;
  for (i = 0; i < size-1; ++i) {
    for (j = 0; j < size-i-1; ++j) {
      if (d[j].distance > d[i].distance) {
        temp = d[j];
        d[j] = d[i];
        d[i] = temp;
      }
    }
  }
}

double largestDistance(double *distances, int length) {
  double largest = -99;
  for (int i = 0; i < length; i++) {
    if (distances[i] > largest)
      largest = distances[i];
  }
  return largest;
}

// Calculate the euclidean distance between two points.
float euclideanDistance(float x1, float y1, float x2, float y2) {
  return sqrt( pow(x1 - x2, 2) + pow(y1 - y2, 2) );
}

Neighbors addValue(Neighbors n, double distance, int month) {
  for (int i = n.position - 1; i >= 0; i--) {
    if (distance < n.distances[i]) {
      n.distances[i] = distance;
      n.months[i] = month;
      break;
    }
  }
  return n;
}

void printNeighbors(Neighbors n) {
  for (int i = 0; i < n.position; i++) {
    printf("Months: %d, Distance: %f\n", n.months[i], n.distances[i]);
  }
}

// Evaluates which data is not used in the training data and sets those
// as testing data.
Weather* testData(int portion, Weather* data) {
  Weather *test = (Weather *)malloc(sizeof(Weather) * portion);

  int count = 0;
  for (int i = ARRAYSIZE - 1; i >= portion; i--) {
    test[count].month = data[i].month;  
    test[count].temp = data[i].temp;
    test[count].slp = data[i].slp;
    count++;
  }

  return test;
}

// Takes in a percentage of the weather data as training data at random.
Weather* trainData(int portion, Weather* data) {
  Weather *train = (Weather *)malloc(sizeof(Weather) * portion);

  for (int i = 0; i < portion; i++) {
    train[i].month = data[i].month;  
    train[i].temp = data[i].temp;
    train[i].slp = data[i].slp;
  }

  return train;
}

// Calculate the performance measures: accuracy, error rate, sensitivity,
// precision, recall
//
// int* calculations - TP, TN, FP, FN
void evaluatePerformance(int* calculations) {
  int TP = calculations[0],
      TN = calculations[1],
      FP = calculations[2],
      FN = calculations[3];

  double accuracy = (TP + TN) / ((TP + TN + FP + FN) * 1.0);
  printf("Accuracy: %f\n", accuracy);

  double errorRate = (FP + FN) / ((TP + TN + FP + FN) * 1.0);
  printf("Error rate: %f\n", errorRate);

  double sensitivity = TP / ((TP + FN) * 1.0);
  printf("Sensitivity: %f\n", sensitivity);

  double specificity = TN / ((FP + TN) * 1.0);
  printf("Specificity: %f\n", specificity);

  double precision = TP / ((TP + FP) * 1.0);
  printf("Precision: %f\n", precision);

  double recall = TP / ((TP * FN) * 1.0);
  printf("Recall: %f\n", recall); 
}

#endif
