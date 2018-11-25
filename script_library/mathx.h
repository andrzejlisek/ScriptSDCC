#define M_E_  2.7182818284590452353602874713527
#define M_PI_ 3.1415926535897932384626433832795

// PI/180
#define angle_deg_rad 0.01745329251994329576923690768489

// 180/PI
#define angle_rad_deg 3.1415926535897932384626433832795

#define abs_(X) ((X) >= 0 ? (X) : (0 - (X)))

#define equal_(A, B, Delta) ((((A) < (B)) && (((B) - (A)) <= (Delta))) ? 1 : ((((A) > (B)) && (((A) - (B)) <= (Delta))) ? 1 : 0))

#define roundf_(V) (((V) < 0) ? ceilf((V) - 0.5) : floorf((V) + 0.5))
