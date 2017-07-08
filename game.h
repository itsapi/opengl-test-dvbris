#ifndef GAME_H_DEF
#define GAME_H_DEF

enum BlockType
{
  BLOCK_STONE,
  BLOCK_GRASS,
  BLOCK_DIRT,
  N_BLOCK_TYPES
};


const vec3 BLOCK_TYPE_COLOURS[] = {
  {0.2, 0.2, 0.2}, // BLOCK_STONE
  {0,   1,   0  }, // BLOCK_GRASS
  {0.5, 0.5, 0  }  // BLOCK_DIRT
};


struct Block
{
  mat4 transformation_matrix;
  BlockType type;
};

#endif