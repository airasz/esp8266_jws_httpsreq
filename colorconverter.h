
void ChangeBackgroundColorNew(unsigned int color);
void createrandom();

uint16_t RGB888toRGB565(const char *rgb32_str_)
{
 long rgb32=strtoul(rgb32_str_, 0, 16);
 return (rgb32>>8&0xf800)|(rgb32>>5&0x07e0)|(rgb32>>3&0x001f);
}
