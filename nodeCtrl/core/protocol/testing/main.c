#include <stdint.h>
#include <malloc.h>
#include <protocol.h>

int main()
{
	uint8_t status = 0;
  struct Protocol* obj = (struct Protocol*)malloc(sizeof(struct Protocol*));
  uint8_t* buf = (uint8_t*)malloc(100);
  uint8_t* response = (uint8_t*)malloc(100);

  Protocol_init(obj);
  buf[0] = CMD_READ_REG; //CMD
  buf[1] = 0x2; //size
  buf[2] = 0x40; //payload
  buf[3] = 0x40; //payload
  buf[4] = 0 - (buf[0] + buf[1] + buf[2] + buf[3]);
  Protocol_parse_packet(obj, buf, response);
  return status;
}
