#ifndef H2FRAME_H
#define H2FRAME_H
#include <stdint.h>

//
// http2 header definitions
//
typedef struct h2frame_t {
	uint8_t len[3];
#define H2_TYPE_DATA          0x00
#define H2_TYPE_HEADERS          0x01
#define H2_TYPE_PRIORITY          0x02
#define H2_TYPE_RESET          0x03
#define H2_TYPE_SETTINGS          0x04
	uint8_t type;
#define H2_FLAG_END_STREAM       0x01
#define H2_FLAG_END_HEADERS      0x04
#define H2_FLAG_PADDED           0x08
#define H2_FLAG_PRIORITY         0x20
	uint8_t flag;
	uint8_t stream[4];
} H2Frame;

static inline char *h2frame_type2str(uint8_t type) {
	switch (type) {
		case 0:
			return "DATA";
		case 1:
			return "HEADERS";
		case 2:
			return "PRIORITY";
		case 3:
			return "RESET";
		case 4:
			return "SETTINGS";
	};
	return "UNKNOWN";
}

static inline uint32_t h2frame_extract_stream(H2Frame *frm) {
	uint32_t rv = frm->stream[0] << 24 | frm->stream[1] << 16 | frm->stream[2] << 8 | frm->stream[3];
	return rv;
}

static inline uint32_t h2frame_extract_length(H2Frame *frm) {
	uint32_t rv = frm->len[0] << 16 | frm->len[1] << 8 | frm->len[2];
	return rv;
}

static inline void h2frame_set_stream(H2Frame *frm, uint32_t stream) {
	frm->stream[3] = stream & 0xFF;
	frm->stream[2] = (stream >> 8) & 0xFF;
	frm->stream[1] = (stream >> 16) & 0xFF;
	frm->stream[0] = (stream >> 24) & 0x7F;
}

static inline void h2frame_set_length(H2Frame *frm, uint32_t length) {
	frm->len[2] = length & 0xFF;
	frm->len[1] = (length >> 8) & 0xFF;
	frm->len[0]  = (length >> 16) & 0xFF;
}

static inline void h2frame_print(H2Frame *frm) {
	uint32_t len = h2frame_extract_length(frm);
	uint32_t stream = h2frame_extract_stream(frm);
	printf("stream %u, len %u, type 0x%02u %s, flags 0x%02u (",
		stream,
		len,
		frm->type, h2frame_type2str(frm->type),
		frm->flag);
#define H2_FLAG_END_STREAM       0x01
#define H2_FLAG_END_HEADERS      0x04
#define H2_FLAG_PADDED           0x08
#define H2_FLAG_PRIORITY         0x20
	if (frm->flag & H2_FLAG_END_STREAM)
		printf("end stream,");
	if (frm->flag & H2_FLAG_END_HEADERS)
		printf("end headers,");
	if (frm->flag & H2_FLAG_PADDED)
		printf("padded,");
	if (frm->flag & H2_FLAG_PRIORITY)
		printf("priority,");
	printf(")\n");
}

#endif
