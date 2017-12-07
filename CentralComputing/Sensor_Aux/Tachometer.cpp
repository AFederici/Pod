#include "Tachometer.h"

using namespace std;

void tachometer_parse(Spi * spi , Arbitrary_Data raw) {
  Tachometer_Raw * t_raw = (Tachometer_Raw *) raw.data;
  t_raw->left = spi->get_data(XMEGA2, TACHOMETER_INDEX_1);
  t_raw->middle = spi->get_data(XMEGA2, TACHOMETER_INDEX_2);
  t_raw->right = spi->get_data(XMEGA2, TACHOMETER_INDEX_3);
  t_raw->lfront = spi->get_data(XMEGA2, TACHOMETER_INDEX_4);
  t_raw->rfront = spi->get_data(XMEGA2, TACHOMETER_INDEX_5);
  t_raw->back = spi->get_data(XMEGA2, TACHOMETER_INDEX_6);
}
