#include <usb_names.h>


#define PRODUCT_NAME \
  {                                                         \
    'T', 'E', 'E', 'N', 'S', 'Y', '1', '4', 'b', 'i', 't'   \
  }                                                         \
  
#define PRODUCT_NAME_LEN 11

struct usb_string_descriptor_struct usb_string_product_name = {
        2 + PRODUCT_NAME_LEN * 2,
        3,
        PRODUCT_NAME
};