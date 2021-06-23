#include "ann.h"

/* Creates and returns a new ann. */
ann_t *ann_create(int num_layers, int *layer_outputs)
{
  /* get input layer */
  ann_t *ann = malloc(sizeof(ann_t));

  layer_t *prev = layer_create();
  layer_init(prev, layer_outputs[0], NULL);
  ann->input_layer = prev;

  layer_t *current;

  for (int layer = 1; layer < num_layers; layer++) {

    /* Create next layer */
    current = layer_create();
    layer_init(current, layer_outputs[layer], prev);

    /* point previous layer to next */
    prev->next = current;

    /* setup next prev and current */
    prev = current;
  }

  ann->output_layer = current;

  return ann;
  /* 4 MARKS */
}

/* Frees the space allocated to ann. */
void ann_free(ann_t *ann)
{
  /**** PART 2 - QUESTION 2 ****/

  /* traverse list freeing the layers*/
  for(layer_t* layer = ann->input_layer; layer != NULL; layer = layer->next) {
    layer_free(layer);
  }

  /* free the struct */
  free(ann);

  /* 2 MARKS */
}

/* Forward run of given ann with inputs. */
void ann_predict(ann_t const *ann, double const *inputs)
{
  /**** PART 2 - QUESTION 3 ****/
  for (int j = 0; j < ann->input_layer->num_outputs; j++) {
    ann->input_layer->outputs[j] = inputs[j];
  }

  for (layer_t *layer = ann->input_layer->next; layer; layer = layer->next) {
    layer_compute_outputs(layer);
  }
  /* 2 MARKS */
}

/* Trains the ann with single backprop update. */
void ann_train(ann_t const *ann, double const *inputs, double const *targets, double l_rate)
{


  /* Sanity checks. */
  assert(ann != NULL);
  assert(inputs != NULL);
  assert(targets != NULL);
  assert(l_rate > 0);

  /* Run forward pass. */
  ann_predict(ann, inputs);

  /**** PART 2 - QUESTION 4 ****/

  /* compute output layer deltas */

  for (int j = 0; j < ann->output_layer->num_outputs; j++) {
    ann->output_layer->deltas[j] = sigmoidprime(ann->output_layer->outputs[j]) * (targets[j] - ann->output_layer->outputs[j]);
  }


  layer_update(ann->output_layer, l_rate);
  for (layer_t *layer = ann->output_layer->prev; layer->prev; layer = layer->prev) {
    layer_compute_deltas(layer);
    layer_update(layer, l_rate);
  }
  /* 3 MARKS */
}

/* something is broke, cannot figure out what, does not learn XOR */
