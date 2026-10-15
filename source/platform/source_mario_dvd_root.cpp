#include "source_mario_dvd_root.hpp"
namespace ttyd {
uint32_t sourceMarioDvdRoot(uint32_t actualSourceR2){
 // signExtend16(0x8054)=-32684; PPC register addition wraps modulo2^32.
 return actualSourceR2-0x7facu;
}
}
