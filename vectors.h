struct vec2
{
  union
  {
    float elem[2];
    struct
    {
      float x;
      float y;
    };
  };
};


struct vec3
{
  union
  {
    float elem[3];
    struct
    {
      float x;
      float y;
      float z;
    };
    struct
    {
      vec2 xy;
      float _1;
    };
    struct
    {
      float _2;
      vec2 yz;
    };
  };
};


struct vec4
{
  union
  {
    float elem[4];
    struct
    {
      float x;
      float y;
      float z;
      float w;
    };
    struct
    {
      vec2 xy;
      vec2 zw;
    };
    struct
    {
      float _1;
      vec2 yz;
      float _2;
    };
    struct
    {
      vec3 xyz;
      float _3;
    };
    struct
    {
      float _4;
      vec3 yzw;
    };
  };
};


struct mat2
{
  union
  {
    vec2 rs[2];
    float es[4];
    struct
    {
      vec2 a;
      vec2 b;
    };
  };
};


struct mat3
{
  union
  {
    vec3 rs[3];
    float es[9];
    struct
    {
      vec3 a;
      vec3 b;
      vec3 c;
    };
  };
};


struct mat4
{
  union
  {
    vec4 rs[4];
    float es[16];
    struct
    {
      vec4 a;
      vec4 b;
      vec4 c;
      vec4 d;
    };
  };
};
