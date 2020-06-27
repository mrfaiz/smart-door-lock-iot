struct packet{
  uint8_t status;
  unsigned short packet_type; // 1 = forward packet, 2 = backward packet
  uint16_t node_id;
  unsigned short packet_id;
};

struct packet pkt;