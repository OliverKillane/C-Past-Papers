#include "region.h"
#include "image.h"
#include "typedefs.h"
#include "list.h"
#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include <assert.h>

/////ALL THESE FUNCTIONS ARE PROVIDED FOR YOU/////////////////////
/////DO NOT MODIFY THEM///////////////////////////////////////////

// Our colour function of choice
uint8_t region_colour(const region_t *region)
{
  const unsigned depth = region->depth;
  return (depth * 1879u) % 255;
}

// Resets a point to (x,y)
void init_point(point_t *point, int x, int y)
{
  point->x = x;
  point->y = y;
}

// Resets an extent to (width, height)
void init_extent(extent_t *extent, int width, int height)
{
  extent->width = width;
  extent->height = height;
}


// Allocates a region. You may assume that the return value is non-NULL.
region_t *region_allocate(void)
{
  region_t *new_region = malloc(sizeof(region_t));

  if (new_region == NULL)
  {
    perror("region_allocate");
    exit(EXIT_FAILURE);
  }

  return new_region;
}

// Prints a textual description of a region to the supplied FILE*.
void print_region(FILE *out, const region_t *region)
{
  assert(out != NULL);
  assert(region != NULL);

  fprintf(out, "Region of depth %i at (%i, %i) of extent (%i, %i)\n",
      region->depth, region->position.x, region->position.y,
      region->extent.width, region->extent.height);
}

// Finds all regions located in "image" and adds them to "regions".
// Regions are added so that ordering according to the
// comparison function region_compare() is preserved.
void find_regions(list_t *regions, image_t* image)
{
  region_t *image_region = region_allocate();

  image_region->depth = 0;
  init_point(&image_region->position, 0, 0);
  init_extent(&image_region->extent, image->width, image->height);

  list_insert_ascending(regions, image_region);
  find_sub_regions(regions, image, image_region);
}

///////////////////////////////////////////////////////////////////

/////////////////////////USEFUL FUNCTIONS///////////////////////////
///////////////////////////////////////////////////////////////////

// Compares two points lexicographically.
// Returns 1 if first is less than second, otherwise 0.
// Ordering of comparison is [y, x].
//
// You are NOT required to implement this function. However you may find
// it helpful to do so for Part I, Q1.
int point_compare_less(const point_t first, const point_t second)
{
  return first.y < second.y || (first.y == second.y && first.x < second.x);
}

// Deallocates a region.
//
// You are NOT required to implement this function. However, you may
// find it helpful to do so for Part I, Q3.
void region_destroy(region_t *region)
{
  free(region);
}
///////////////////////////////////////////////////////////////////

////////////////TO BE IMPLEMENTED///////////////////////////////////
////////////////////////////////////////////////////////////////////

// Compares two regions.
// Returns 1 if position of first is lexicographically less than that
// of the second, otherwise returns 0.
// Ordering of the position comparison is [y, x].
int region_compare(const region_t *r1, const region_t *r2)
{
  return point_compare_less(r1->position, r2->position);
}

// Prints all regions in "regions" to "out".
// print_region (above) prints a textual description of a region
// to the supplied FILE*
void print_regions(FILE *out, list_t *regions)
{
  for (list_iter r = regions->header->next; r->next; r = r->next) print_region(out, r->region);
}

//
// Sets the specified region of image "image" to the intensity value "value".
//
void image_fill_region(image_t *image, const region_t *region, uint8_t value)
{
  for (int x_extent = 0; x_extent < region->extent.width; x_extent++) {
    for (int y_extent = 0; y_extent < region->extent.height; y_extent++) {
      set_pixel(image, region->position.x + x_extent, region->position.y + y_extent, value);
    }
  }
}

// Determines the extent of a region.
// position: the location of the top-left-hand corner of the region.
// image: the image to be searched.
// extent: this will be populated with the width and height of a region.
void find_extent(extent_t *extent, image_t *image, const point_t *position)
{
  uint8_t colour = get_pixel(image, position->x, position->y);

  int x = 0;
  while (x + position->x < image->width && (get_pixel(image, x + position->x, position->y) == colour)) x++;
  /* x now at extent*/

  int y = 0;
  while (y + position->y < image->height && (get_pixel(image,  position->x, y + position->y) == colour)) y++;
  /* y now at extent*/

  init_extent(extent, x, y);
}

// Finds all regions located in the region "current" of "image" and adds them
// to "regions".  Regions are added so that ordering according to the
// comparison function region_compare() is preserved.
void find_sub_regions(list_t* regions, image_t *image, const region_t *current)
{
  uint8_t colour = get_pixel(image, current->position.x, current->position.y);

  for (int y = current->position.y; y < current->position.y + current->extent.height; y++) {
    for (int x = current->position.x; x < current->position.x + current->extent.width; x++) {
      if (get_pixel(image, x, y) != colour) {
        /* found the top=left */
        region_t *new_region = region_allocate();

        /* set new region members */
        init_point(&(new_region->position), x, y);
        new_region->depth = current->depth + 1;

        /* get its extent */
        find_extent(&(new_region->extent), image, &(new_region->position));

        /* find its sub regions */
        find_sub_regions(regions, image, new_region);

        /* add to the region list*/
        list_insert_ascending(regions, new_region);

        /* fill in the region */
        image_fill_region(image, new_region, colour);
      }
    }
  }

}

// Renders all regions to an image using the supplied colour_function_t
// (declared in typedefs.h) to select pixel intensity.
void render_regions(image_t *image, list_t *regions,
                    colour_function_t get_colour)
{
  for (list_iter iter = list_begin(regions); iter->next; iter = list_iter_next(iter)) {
    image_fill_region(image, iter->region, get_colour(iter->region));
  }
}
///////////////////////////////////////////////////////////////////////////////
/* Note: written on windows, tested with wsl. Output to console matches reference 
output txts, but unable to view .pgm files without irfan (cba to install rn)*/
