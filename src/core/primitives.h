#pragma once

namespace juye{

template<typename _Type>
struct vec2{
  union{
    struct{
      _Type x; _Type y;
    };

    struct{
      _Type r; _Type g;
    };
  };
};

template<typename _Type>
struct vec3{
  union{
    struct{
      _Type x; _Type y; _Type z;
    };

    struct{
      _Type r; _Type g; _Type b;
    };
  };

  vec3(_Type x, _Type y, _Type z):x(x), y(y), z(z){}
};

template<typename _Type>
struct vec4{
  union{
    struct{
      _Type x; _Type y; _Type z; _Type w;
    };

    struct{
      _Type r; _Type g; _Type b; _Type a;
    };
  };
};

}

