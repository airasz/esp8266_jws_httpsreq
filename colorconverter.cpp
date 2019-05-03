String chex[16]={"0","1","2","3","4","5","6","7","8","9","A","B","C","D","E","F"};
const char col;
void createrandom(){
  for(int i=0; i<7; i++){
    col+=chex[random(0, 17)].c_str();
  }
  return RGB888toRGB565(col);
}


uint16_t RGB888toRGB565(const char *rgb32_str_)
{
 long rgb32=strtoul(rgb32_str_, 0, 16);
 return (rgb32>>8&0xf800)|(rgb32>>5&0x07e0)|(rgb32>>3&0x001f);
}