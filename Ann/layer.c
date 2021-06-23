#include "layer.h"

/* The sigmoid function and derivative. */
double sigmoid(double x)
{
  return 1 / (1 + exp(-x));
  /* 1 MARK */
}

double sigmoidprime(double x)
{
  return x*(1 - x);
}

/* Creates a single layer. */
layer_t *layer_create()
{
  return calloc(1, sizeof(layer_t));
  /* 2 MARKS */
}


/* 2D matrix creator */
double **get2Darray(int rows, int columns) {

    /* get an array of pointers */
    double** array2D = (double**) malloc(rows * sizeof(double *));
    
    /* if allocation failed, return null */
    if (!array2D) return NULL;

    array2D[0] = (double*) malloc(rows * columns * sizeof(double));

    /* if allocation failed, free first allocation, then return null */
    if (!array2D[0]) {
        free(array2D);
        return NULL;
    }

    /* set all subarray pointers to point to the correct part of the 2D array */
    for (int subarray = 1; subarray < rows; subarray++) {
        array2D[subarray] = array2D[subarray - 1] + columns;
    }

    return array2D;
}

/* Initialises the given layer. */
bool layer_init(layer_t *layer, int num_outputs, layer_t *prev)
{
  /* set num_outputs, allocate output array, set prev. */
  layer->num_outputs = num_outputs;

  layer->prev = prev;

  layer->outputs = calloc(num_outputs, sizeof(double));
  if (!layer->outputs) return true;

  if (prev) {

    /* if not an input layer, set input size. */
    layer->num_inputs = prev->num_outputs;

    layer->weights = get2Darray(layer->num_inputs, num_outputs);
    if (!layer->weights) return true;


    /* set all weights to a random number */
    for (int i = 0; i < num_outputs; i++) {
      for (int j = 0; j < layer->num_inputs; j++) {
        layer->weights[j][i] = ANN_RANDOM();
      }
    }



    /* allocate bias and delta for every neuron in array */
    layer->biases = calloc(num_outputs, sizeof(double));
    layer->deltas = calloc(num_outputs, sizeof(double));
  }

  return false;
}

/* Frees a given layer. */
void layer_free(layer_t *layer)
{
  free(layer->outputs);

  if (layer->prev) {
    free(layer->biases);
    free(layer->deltas);
    free(layer->weights[0]);
    free(layer->weights);
  }
  /* 2 MARKS */
}

/* Computes the outputs of the current layer. */
void layer_compute_outputs(layer_t const *layer)
{
  /**** PART 1 - QUESTION 5 ****/

  /* is an input layer */
  assert(layer->prev);

  double weighted_sum;

  for (int j = 0; j < layer->num_outputs; j++) {
    weighted_sum = 0;

    for (int i = 0; i < layer->num_inputs; i++) {
      weighted_sum += layer->weights[i][j] * layer->prev->outputs[i];
    }

    layer->outputs[j] = sigmoid(layer->biases[j] * weighted_sum);
  }
  /* objective: compute layer->outputs */

  /* 3 MARKS */
}

/* Computes the delta errors for this layer. */
void layer_compute_deltas(layer_t const *layer)
{
  double weighted_sum;

  for (int i = 0; i < layer->num_outputs; i++) {
    weighted_sum = 0;

    for (int j = 0; j < layer->next->num_outputs; j++) {
      weighted_sum += layer->next->weights[i][j] * layer->next->deltas[j];
    }

    layer->deltas[i] = sigmoidprime(layer->outputs[i]) * weighted_sum;
  }

  /**** PART 1 - QUESTION 6 ****/
  /* objective: compute layer->deltas */

  /* 2 MARKS */
}

/* Updates weights and biases according to the delta errors given learning rate. */
void layer_update(layer_t const *layer, double l_rate)
{

  /* bias altered */

  for (int j = 0; j < layer->num_outputs; j++) {
    layer->biases[j] += l_rate * layer->deltas[j];
  }

  for (int i = 0; i < layer->num_inputs; i++) {
    for (int j = 0; j < layer->num_outputs; j++) {
      layer->weights[i][j] += l_rate * layer->prev->outputs[i] * layer->deltas[j];
    }
  }
  
  /**** PART 1 - QUESTION 7 ****/
  /* objective: update layer->weights and layer->biases */

  /* 1 MARK */
}
