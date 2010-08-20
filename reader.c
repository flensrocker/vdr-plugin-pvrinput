#include "common.h"
#include <libsi/si.h>

#define SENDPATPMT_PACKETINTERVAL 500


const short kVideoPid    = 301;
const short kAudioPid    = 300;
const short kTeletextPid = 305;
const short kPCRPid      = 101;
enum eVbiFormat { vbifmt_itv0, vbifmt_ITV0 };

const unsigned char kPAT[TS_SIZE] = {
  0x47, 0x40, 0x00, 0x10, 0x00, 0x00, 0xb0, 0x0d,
  0x00, 0x00, 0xc1, 0x00, 0x00, 0x00, 0x01, 0xe0,
  0x84, 0xcc, 0x32, 0xcc, 0x32, 0xff, 0xff, 0xff,
  0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
  0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
  0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
  0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
  0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
  0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
  0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
  0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
  0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
  0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
  0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
  0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
  0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
  0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
  0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
  0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
  0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
  0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
  0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
  0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
  0xff, 0xff, 0xff, 0xff
};

const unsigned char kPMTwithTeletext[TS_SIZE] = {
  0x47, 0x40, 0x84, 0x10, 0x00, 0x02, 0xb0, 0x24,
  0x00, 0x01, 0xc1, 0x00, 0x00, 0xe0, 0x65, 0xf0,
  0x00, 0x02, 0xe1, 0x2d, 0xf0, 0x00, 0x04, 0xe1,
  0x2c, 0xf0, 0x06, 0x0a, 0x04, 0x00, 0x00, 0x00,
  0x01, 0x06, 0xe1, 0x31, 0xf0, 0x02, 0x56, 0x00,
  0xcc, 0x32, 0xcc, 0x32, 0xff, 0xff, 0xff, 0xff,
  0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
  0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
  0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
  0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
  0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
  0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
  0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
  0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
  0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
  0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
  0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
  0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
  0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
  0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
  0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
  0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
  0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
  0xff, 0xff, 0xff, 0xff
};

const unsigned char kPMTwithoutTeletext[TS_SIZE] = {
  0x47, 0x40, 0x84, 0x10, 0x00, 0x02, 0xb0, 0x1d,
  0x00, 0x01, 0xc1, 0x00, 0x00, 0xe0, 0x65, 0xf0,
  0x00, 0x02, 0xe1, 0x2d, 0xf0, 0x00, 0x04, 0xe1,
  0x2c, 0xf0, 0x06, 0x0a, 0x04, 0x00, 0x00, 0x00,
  0x01, 0xcc, 0x32, 0xcc, 0x32, 0xff, 0xff, 0xff,
  0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
  0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
  0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
  0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
  0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
  0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
  0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
  0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
  0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
  0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
  0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
  0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
  0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
  0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
  0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
  0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
  0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
  0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
  0xff, 0xff, 0xff, 0xff
};

const unsigned char kPMTRadio[TS_SIZE] = {
  0x47, 0x40, 0x84, 0x10, 0x00, 0x02, 0xb0, 0x18,
  0x00, 0x01, 0xc1, 0x00, 0x00, 0xe0, 0x65, 0xf0,
  0x00, 0x04, 0xe1, 0x2c, 0xf0, 0x06, 0x0a, 0x04,
  0x00, 0x00, 0x00, 0x01, 0xcc, 0x32, 0xcc, 0x32,
  0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
  0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
  0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
  0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
  0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
  0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
  0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
  0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
  0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
  0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
  0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
  0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
  0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
  0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
  0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
  0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
  0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
  0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
  0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
  0xff, 0xff, 0xff, 0xff
};

const unsigned char kInvTab[256] = {
  0x00, 0x80, 0x40, 0xc0, 0x20, 0xa0, 0x60, 0xe0,
  0x10, 0x90, 0x50, 0xd0, 0x30, 0xb0, 0x70, 0xf0,
  0x08, 0x88, 0x48, 0xc8, 0x28, 0xa8, 0x68, 0xe8,
  0x18, 0x98, 0x58, 0xd8, 0x38, 0xb8, 0x78, 0xf8,
  0x04, 0x84, 0x44, 0xc4, 0x24, 0xa4, 0x64, 0xe4,
  0x14, 0x94, 0x54, 0xd4, 0x34, 0xb4, 0x74, 0xf4,
  0x0c, 0x8c, 0x4c, 0xcc, 0x2c, 0xac, 0x6c, 0xec,
  0x1c, 0x9c, 0x5c, 0xdc, 0x3c, 0xbc, 0x7c, 0xfc,
  0x02, 0x82, 0x42, 0xc2, 0x22, 0xa2, 0x62, 0xe2,
  0x12, 0x92, 0x52, 0xd2, 0x32, 0xb2, 0x72, 0xf2,
  0x0a, 0x8a, 0x4a, 0xca, 0x2a, 0xaa, 0x6a, 0xea,
  0x1a, 0x9a, 0x5a, 0xda, 0x3a, 0xba, 0x7a, 0xfa,
  0x06, 0x86, 0x46, 0xc6, 0x26, 0xa6, 0x66, 0xe6,
  0x16, 0x96, 0x56, 0xd6, 0x36, 0xb6, 0x76, 0xf6,
  0x0e, 0x8e, 0x4e, 0xce, 0x2e, 0xae, 0x6e, 0xee,
  0x1e, 0x9e, 0x5e, 0xde, 0x3e, 0xbe, 0x7e, 0xfe,
  0x01, 0x81, 0x41, 0xc1, 0x21, 0xa1, 0x61, 0xe1,
  0x11, 0x91, 0x51, 0xd1, 0x31, 0xb1, 0x71, 0xf1,
  0x09, 0x89, 0x49, 0xc9, 0x29, 0xa9, 0x69, 0xe9,
  0x19, 0x99, 0x59, 0xd9, 0x39, 0xb9, 0x79, 0xf9,
  0x05, 0x85, 0x45, 0xc5, 0x25, 0xa5, 0x65, 0xe5,
  0x15, 0x95, 0x55, 0xd5, 0x35, 0xb5, 0x75, 0xf5,
  0x0d, 0x8d, 0x4d, 0xcd, 0x2d, 0xad, 0x6d, 0xed,
  0x1d, 0x9d, 0x5d, 0xdd, 0x3d, 0xbd, 0x7d, 0xfd,
  0x03, 0x83, 0x43, 0xc3, 0x23, 0xa3, 0x63, 0xe3,
  0x13, 0x93, 0x53, 0xd3, 0x33, 0xb3, 0x73, 0xf3,
  0x0b, 0x8b, 0x4b, 0xcb, 0x2b, 0xab, 0x6b, 0xeb,
  0x1b, 0x9b, 0x5b, 0xdb, 0x3b, 0xbb, 0x7b, 0xfb,
  0x07, 0x87, 0x47, 0xc7, 0x27, 0xa7, 0x67, 0xe7,
  0x17, 0x97, 0x57, 0xd7, 0x37, 0xb7, 0x77, 0xf7,
  0x0f, 0x8f, 0x4f, 0xcf, 0x2f, 0xaf, 0x6f, 0xef,
  0x1f, 0x9f, 0x5f, 0xdf, 0x3f, 0xbf, 0x7f, 0xff,
};

/* helper class for protected crc32-function in libsi */
class cPvrCRC32 : public SI::CRC32 {
public:
  static inline u_int32_t crc32(const char *d, int len, u_int32_t CRCvalue) {
    return SI::CRC32::crc32(d, len, CRCvalue);
  }
};


cPvrReadThread::cPvrReadThread(cRingBufferLinear *TsBuffer, cPvrDevice *_parent)
: tsBuffer(TsBuffer),
  video_counter(0),
  audio_counter(0),
  text_counter(0),
  pcr_counter(0),
  packet_counter(0),
  pes_stream_id(0),
  pes_offset(0),
  pes_length(0),
  pes_tmp(0),
  pes_scr_isvalid(false),
  pes_scr(0),
  pes_scr_ext(0)
{
  log(pvrDEBUG1, "cPvrReadThread");
  parent = _parent;
  parent->readThreadRunning = true;
  SetDescription("PvrReadThread of /dev/video%d", _parent->number);
  Start();
}

cPvrReadThread::~cPvrReadThread(void)
{
  log(pvrDEBUG2, "~cPvrReadThread");
  parent->readThreadRunning = false;
  if (Running())
     Cancel(3);
}

int cPvrReadThread::PutData(const unsigned char *Data, int Count)
{
  if (!tsBuffer) {
     log(pvrINFO,"cPvrReadThread::PutData():Unable to put data into RingBuffer");
     return 0;
     }
  int bytesFree = tsBuffer->Free();
  if (bytesFree < Count) {
     log(pvrERROR,"cPvrReadThread::PutData():Unable to put data into RingBuffer, only %d bytes free, need %d", bytesFree, Count);
     return 0;
     }
  int written = tsBuffer->Put(Data, Count);
  if (written != Count) {
     log(pvrERROR,"cPvrReadThread::PutData():put incomplete data into RingBuffer, only %d bytes written, wanted %d", written, Count);
     tsBuffer->ReportOverflow(Count - written);
     }
  return written;
}


void cPvrReadThread::PesToTs(uint8_t *Data, uint32_t Length)
{
  uint8_t stream_id;
  bool first = true;
  uint32_t i;
  const short *pid = &kVideoPid;
  uint8_t *counter = &video_counter;
  const short PayloadSize = TS_SIZE - 4;
  uint32_t Payload_Count  = Length / PayloadSize;
  uint32_t Payload_Rest   = Length % PayloadSize;
  stream_id = Data[3];

  if (packet_counter <= 0) { // time to send PAT and PMT
     // increase continuity counter
     pat_buffer[ 3] = (pat_buffer[ 3] & 0xF0) | (((pat_buffer[ 3] & 0x0F) + 1) & 0x0F);
     pmt_buffer[ 3] = (pmt_buffer[ 3] & 0xF0) | (((pmt_buffer[ 3] & 0x0F) + 1) & 0x0F);
     PutData(pat_buffer, TS_SIZE);
     PutData(pmt_buffer, TS_SIZE);
     packet_counter = SENDPATPMT_PACKETINTERVAL;
     }

  if (pes_scr_isvalid && (stream_id != 0xBD)) { // send PCR packet but not PCR of vbi data
     ts_buffer[0] = TS_SYNC_BYTE;
     ts_buffer[1] = kPCRPid >> 8;
     ts_buffer[2] = kPCRPid & 0xFF;
     ts_buffer[3] = 0x20 | pcr_counter;
     ts_buffer[4] = 0xB7;
     ts_buffer[5] = 0x10;
     ts_buffer[6] = (pes_scr & 0x01FE000000ull) >> 25; // 33 bits SCR base
     ts_buffer[7] = (pes_scr & 0x01FE0000) >> 17;
     ts_buffer[8] = (pes_scr & 0x01FE00) >> 9;
     ts_buffer[9] = (pes_scr & 0x01FE) >> 1;
     ts_buffer[10] = (pes_scr & 0x01) << 7;
     ts_buffer[10] |= 0x7E; // 6 bits between SCR and SCR extension
     ts_buffer[10] |= (pes_scr_ext & 0x0100) >> 8; // 9 bits SCR extension
     ts_buffer[11] = (pes_scr_ext & 0xFF);
     memset(ts_buffer + 12, 0xFF, TS_SIZE - 12);
     PutData(ts_buffer, TS_SIZE);
     pcr_counter = (pcr_counter + 1) & 15;
     pes_scr_isvalid = false;
     }

  switch (stream_id) {
    case 0xC0 ... 0xEF:
      if (stream_id < 0xE0) {
         pid = &kAudioPid;
         counter = &audio_counter;
         }
      else if (parent->CurrentInputType == eRadio)
         return;
      for (i = 0; i < Payload_Count; i++) {
        ts_buffer[0] = TS_SYNC_BYTE;
        ts_buffer[1] = (first ? 0x40 : 0x00) | (*pid >> 8);
        ts_buffer[2] = *pid & 0xFF;
        ts_buffer[3] = 0x10 | *counter;
        memcpy(ts_buffer + 4, Data + i * PayloadSize, PayloadSize);
        PutData(ts_buffer, TS_SIZE);
        packet_counter--;
        *counter = (*counter + 1) & 15; //uint8_t
        first = false;
        } // end: for (i = 0; i < Payload_Count; i++)
      if (Payload_Rest > 0) {
        ts_buffer[0] = TS_SYNC_BYTE;
        ts_buffer[1] = (first ? 0x40 : 0x00) | (*pid >> 8);
        ts_buffer[2] = *pid & 0xFF;
        ts_buffer[3] = 0x30 | *counter;
        ts_buffer[4] = PayloadSize - Payload_Rest - 1;
        if (ts_buffer[4] > 0) {
          ts_buffer[5] = 0x00;
          memset(ts_buffer + 6, 0xFF, ts_buffer[4] - 1);
          } //end: if (ts_buffer[4] > 0)
        memcpy(ts_buffer + 5 + ts_buffer[4], Data + i * PayloadSize, Payload_Rest);
        PutData(ts_buffer, TS_SIZE);
        packet_counter--;
        *counter = (*counter + 1) & 15;
        first = false;
        } // end: if (Payload_Rest > 0)
      break; // end: case 0xE0..0xEF:
    case 0xBD: {
      v4l2_mpeg_vbi_fmt_ivtv *vbi_fmt = (v4l2_mpeg_vbi_fmt_ivtv*)(Data + 9 + Data[8]);
      eVbiFormat fmt;
      if (memcmp(vbi_fmt->magic, V4L2_MPEG_VBI_IVTV_MAGIC0, 4) == 0)
         fmt = vbifmt_itv0;
      else if (memcmp(vbi_fmt->magic, V4L2_MPEG_VBI_IVTV_MAGIC1, 4) == 0)
         fmt = vbifmt_ITV0;
      else
         return; // no known format
      
      uint16_t needed_size = 0;
      switch (fmt) {
        case vbifmt_itv0: {
          uint32_t bit = 1;
          int lm_nr = 0;
          int line_nr = 0;
          while ((lm_nr != 1) || (bit != 0x10)) {
                if (vbi_fmt->itv0.linemask[lm_nr] & bit) {
                   if ((line_nr < 35) && ((vbi_fmt->itv0.line[line_nr].id == V4L2_MPEG_VBI_IVTV_TELETEXT_B)
                       || (vbi_fmt->itv0.line[line_nr].id == V4L2_MPEG_VBI_IVTV_WSS_625)
                       //|| (vbi_fmt->itv0.line[line_nr].id == V4L2_MPEG_VBI_IVTV_VPS)
                       ))
                      needed_size += 46;
                   line_nr++;
                   }
                bit <<= 1;
                if (bit == 0) {
                   lm_nr++;
                   bit = 1;
                   }
                }
          break;
          }
        case vbifmt_ITV0: {
          if ((36 * sizeof(v4l2_mpeg_vbi_itv0_line) + 9 + Data[8] + 4) > Length)
             return; // not enough data
          for (int vbiNr = 0; vbiNr < 36; vbiNr++) {
              if ((vbi_fmt->ITV0.line[vbiNr].id == V4L2_MPEG_VBI_IVTV_TELETEXT_B)
                  || (vbi_fmt->ITV0.line[vbiNr].id == V4L2_MPEG_VBI_IVTV_WSS_625)
                  //|| (vbi_fmt->ITV0.line[vbiNr].id == V4L2_MPEG_VBI_IVTV_VPS)
                  )
                 needed_size += 46;
              }
          break;
          }
        }
      if (needed_size == 0)
         return; // no lines for translation
      needed_size += 46; // for PES header
      uint16_t mod = needed_size % 184;
      if (mod > 0)
         needed_size += 184 - mod; // fill the last packet with stuffing
      memset(ts_buffer, 0xFF, TS_SIZE);
      ts_buffer[0] = TS_SYNC_BYTE;
      ts_buffer[1] = (first ? 0x40 : 0x00) | (kTeletextPid >> 8);
      ts_buffer[2] = kTeletextPid & 0xFF;
      ts_buffer[3] = 0x10 | text_counter;
      memcpy(ts_buffer + 4, Data, 9 + Data[8]);
      ts_buffer[8] = ((needed_size - 6) >> 8) & 0xFF; // decrease by length of "00 00 01 bd xx xx"
      ts_buffer[9] = (needed_size - 6) & 0xFF;
      ts_buffer[12] = 0x24;
      ts_buffer[49] = 0x10; // data identifier for EBU data
      uint8_t  ts_line_nr = 1;
      uint8_t  ITV0_vbiLineNr = 0;
      uint8_t  itv0_vbiLineNr = 0;
      uint32_t itv0_bit = 1;
      uint8_t  itv0_linemaskNr = 0;
      uint8_t  itv0_field_parity = 1;
      uint8_t  itv0_line_offset = 6;

      uint8_t data_unit_id = 0;
      uint8_t framing_code = 0;
      uint8_t field_parity = 0;
      uint8_t line_offset = 0;
      uint8_t copy_vbi_bytes = 0;
      uint8_t vbi_bytes[10] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
      bool    copy_vbi_line = false;
      v4l2_mpeg_vbi_itv0_line *vbi_line = 0;
      while (true) {
            data_unit_id = 0;
            framing_code = 0;
            field_parity = 0;
            line_offset = 0;
            copy_vbi_bytes = 0;
            copy_vbi_line = false;
            vbi_line = 0;
            switch (fmt) {
              case vbifmt_itv0: {
                if ((itv0_vbiLineNr < 35) && (vbi_fmt->itv0.linemask[itv0_linemaskNr] & itv0_bit)) {
                   switch (vbi_fmt->itv0.line[itv0_vbiLineNr].id) {
                     case V4L2_MPEG_VBI_IVTV_TELETEXT_B: {
                       data_unit_id = 0x02;
                       framing_code = 0xE4;
                       copy_vbi_line = true;
                       break;
                       }
                     case V4L2_MPEG_VBI_IVTV_WSS_625: {
                       data_unit_id = 0xC4;
                       framing_code = kInvTab[vbi_fmt->itv0.line[itv0_vbiLineNr].data[0]];
                       vbi_bytes[0] = kInvTab[vbi_fmt->itv0.line[itv0_vbiLineNr].data[1]];
                       copy_vbi_bytes = 1;
                       break;
                       }
                     case V4L2_MPEG_VBI_IVTV_VPS: {
                       break;
                       }
                     }
                   if (data_unit_id != 0) {
                      vbi_line = &vbi_fmt->itv0.line[itv0_vbiLineNr];
                      field_parity = itv0_field_parity;
                      line_offset = itv0_line_offset;
                      }
                   itv0_vbiLineNr++;
                   }
                itv0_bit <<= 1;
                if ((itv0_linemaskNr == 0) && (itv0_bit == 0)) {
                   itv0_bit = 1;
                   itv0_linemaskNr++;
                   }
                itv0_line_offset++;
                if (itv0_line_offset == 24) {
                   itv0_field_parity = 0;
                   itv0_line_offset = 6;
                   }
                break;
                }
              case vbifmt_ITV0: {
                switch (vbi_fmt->ITV0.line[ITV0_vbiLineNr].id) {
                  case V4L2_MPEG_VBI_IVTV_TELETEXT_B: {
                    data_unit_id = 0x02;
                    framing_code = 0xE4;
                    copy_vbi_line = true;
                    break;
                    }
                  case V4L2_MPEG_VBI_IVTV_WSS_625: {
                    data_unit_id = 0xC4;
                    framing_code = kInvTab[vbi_fmt->ITV0.line[ITV0_vbiLineNr].data[0]];
                    vbi_bytes[0] = kInvTab[vbi_fmt->ITV0.line[ITV0_vbiLineNr].data[1]];
                    copy_vbi_bytes = 1;
                    break;
                    }
                  case V4L2_MPEG_VBI_IVTV_VPS: {
                    break;
                    }
                  }
                if (data_unit_id != 0) {
                   vbi_line = &vbi_fmt->ITV0.line[ITV0_vbiLineNr];
                   field_parity = (ITV0_vbiLineNr < 18) ? 1 : 0;
                   line_offset = (ITV0_vbiLineNr < 18) ? (ITV0_vbiLineNr + 6) : (ITV0_vbiLineNr - 12);
                   }
                ITV0_vbiLineNr++;
                break;
                }
              }
            
            if ((vbi_line != 0) && (copy_vbi_line || copy_vbi_bytes)) {
                if (ts_line_nr == 0) { // send current packet and prepare next one
                   PutData(ts_buffer, TS_SIZE);
                   first = false;
                   packet_counter--;
                   text_counter = (text_counter + 1) & 15;
                   memset(ts_buffer, 0xFF, TS_SIZE);
                   ts_buffer[0] = TS_SYNC_BYTE;
                   ts_buffer[1] = (first ? 0x40 : 0x00) | (kTeletextPid >> 8);
                   ts_buffer[2] = kTeletextPid & 0xFF;
                   ts_buffer[3] = 0x10 | text_counter;
                   }
                ts_buffer[4 + ts_line_nr * 46 + 0] = data_unit_id;
                ts_buffer[4 + ts_line_nr * 46 + 1] = 0x2C;
                ts_buffer[4 + ts_line_nr * 46 + 2] = 0xC0 | (field_parity << 5) | (line_offset & 0x1f);
                ts_buffer[4 + ts_line_nr * 46 + 3] = framing_code;
                if (copy_vbi_line) {
                   for (int datNr = 0; datNr < 42; datNr++)
                       ts_buffer[4 + ts_line_nr * 46 + 4 + datNr] = kInvTab[vbi_line->data[datNr]];
                   }
                else if (copy_vbi_bytes > 0) {
                   for (int datNr = 0; datNr < copy_vbi_bytes; datNr++)
                       ts_buffer[4 + ts_line_nr * 46 + 4 + datNr] = vbi_bytes[datNr];
                   }
                ts_line_nr++;
                if (ts_line_nr == 4)
                   ts_line_nr = 0;
               }
            if (((fmt == vbifmt_itv0) && (itv0_bit == 0x10) && (itv0_linemaskNr == 1))
               || ((fmt == vbifmt_ITV0) && (ITV0_vbiLineNr == 36)))
               break;
            }
      while ((ts_line_nr > 0) && (ts_line_nr < 4)) { // ts_buffer is initialized with 0xFF, so just set the data unit length for the stuffing lines
            ts_buffer[4 + ts_line_nr * 46 + 1] = 0x2C;
            ts_line_nr++;
            }
      PutData(ts_buffer, TS_SIZE);
      break; // end: case 0xBD:
      }
    } // end: switch (stream_id)
}

void cPvrReadThread::ParseProgramStream(uint8_t *Data, uint32_t Length)
{
  uint32_t pos = 0;
  while (pos < Length) {
    switch(pes_offset)  {
      case 0:
      case 1:
        if (Data[pos] == 0x00)
          pes_offset++;
        else
          pes_offset = 0;
        pos++;
        break;
      case 2:
        if (Data[pos] == 0x01)  {
          pes_offset++;
          pos++;
          }
        else
          pes_offset = 0;
        break;
      case 3:
        pes_stream_id = Data[pos];
        pes_offset++;
        pos++;
        break;
      default:
        switch (pes_stream_id) {
          case 0xBA:
            switch (pes_offset) {
              case 4:
                pes_scr = (uint64_t)(Data[pos] & 0x38) << 27;
                pes_scr |= (uint64_t)(Data[pos] & 3) << 28;
                pes_offset++;
                pos++;
                break;
              case 5:
                pes_scr |= (uint64_t)(Data[pos]) << 20;
                pes_offset++;
                pos++;
                break;
              case 6:
                pes_scr |= (uint64_t)(Data[pos] & 0xf8) << 12;
                pes_scr |= (uint64_t)(Data[pos] & 3) << 13;
                pes_offset++;
                pos++;
                break;
              case 7:
                pes_scr |= (uint64_t)(Data[pos]) << 5;
                pes_offset++;
                pos++;
                break;
              case 8:
                pes_scr |= (uint64_t)(Data[pos] & 0xf8) >> 3;
                pes_scr_ext = (uint64_t)(Data[pos] & 3) << 7;
                pes_offset++;
                pos++;
                break;
              case 9:
                pes_scr_ext |= (Data[pos] & 0xfe) >> 1;
                pes_scr_isvalid = true;
                pes_offset++;
                pos++;
                break;
              case 10 ... 12:
                pes_offset++;
                pos++;
                break;
              case 13:
                pes_tmp = Data[pos] & 7;
                pes_offset++;
                pos++;
                break;
              default:
                if (pes_tmp > 0) {
                  pes_tmp--;
                  pos++;
                  }
                else
                  pes_offset = 0;
                break;
              }
            break;
          case 0xBB:
            switch (pes_offset) {
              case 4 ... 11:
                pes_offset++;
                pos++;
                break;
              default:
                if (pes_tmp > 0) {
                  pes_tmp--;
                  pos++;
                  }
                else
                  if (Data[pos] & 0x80) {
                    pes_tmp = 3;
                    }
                  else {
                    pes_offset = 0;
                    }
                break;
              } // end: switch (pes_offset)
            break; // end: case 0xBB
          case 0xBD ... 0xEF:
            switch (pes_offset) {
              case 4:
                pes_length = Data[pos] << 8;
                pes_offset++;
                pos++;
                break;
              case 5:
                pes_length += Data[pos];
                pes_buffer[0] = 0x00;
                pes_buffer[1] = 0x00;
                pes_buffer[2] = 0x01;
                pes_buffer[3] = pes_stream_id;
                pes_buffer[4] = pes_length >> 8;
                pes_buffer[5] = pes_length & 0xFF;
                pes_length += 6;
                pes_offset++;
                pos++;
                break;
              default: {
                uint32_t rest = pes_length - pes_offset;
                if (pos + rest <= Length) {
                  memcpy(pes_buffer + pes_offset, Data + pos, rest);
                  pos += rest;
                  PesToTs(pes_buffer, pes_length);
                  pes_offset = 0;
                  }
                else {
                  memcpy(pes_buffer + pes_offset, Data + pos, Length - pos);
                  pes_offset += Length - pos;
                  pos += Length - pos;
                  }
                }
                break;
              }
            break;
          default:
            // unexpected PES Stream id, most probably garbage data.
            pes_offset = 0;
            return;
            break;
          } // end: switch (pes_stream_id)
        } // end: switch(pes_offset)
    }  // end: while (pos < Length)
}

void cPvrReadThread::Action(void)
{
  int bufferSize = PvrSetup.ReadBufferSizeKB * 1024;
  uint8_t *buffer = new uint8_t[bufferSize];
  int r;
  int retries = 3;
  int reopen_retries = 5;
  struct timeval selTimeout;
  fd_set selSet;

  log(pvrDEBUG1,"cPvrReadThread::Action(): Entering Action()");
  // A derived cThread class must check Running()
  // repeatedly to see whether it's time to stop.
  // see VDR/thread.h
  packet_counter = 0;
  // prepare PAT and PMT
  if (parent->streamType != V4L2_MPEG_STREAM_TYPE_MPEG2_TS) {
    memcpy(pat_buffer, kPAT, TS_SIZE);
    int sid = parent->CurrentChannel.Sid();
    int tid = parent->CurrentChannel.Tid();
    pat_buffer[8] = (tid >> 8) & 0xFF;
    pat_buffer[9] = tid & 0xFF;
    pat_buffer[13] = (sid >> 8) & 0xFF;
    pat_buffer[14] = sid & 0xFF;
    int crc = cPvrCRC32::crc32((const char*)(pat_buffer + 5), 12, 0xFFFFFFFF);
    pat_buffer[17] = crc >> 24;
    pat_buffer[18] = crc >> 16;
    pat_buffer[19] = crc >> 8;
    pat_buffer[20] = crc;

    int crc_offset = 0;
    if (parent->CurrentInputType == eRadio) {
      memcpy(pmt_buffer, kPMTRadio, TS_SIZE);
      crc_offset = 28;
      }
    else if ((PvrSetup.SliceVBI != 0) && (cPvrDevice::VBIDeviceCount > 0)) {
      memcpy(pmt_buffer, kPMTwithTeletext, TS_SIZE);
      crc_offset = 40;
      }
    else {
      memcpy(pmt_buffer, kPMTwithoutTeletext, TS_SIZE);
      crc_offset = 33;
      }
    pmt_buffer[8] = (sid >> 8) & 0xFF;
    pmt_buffer[9] = sid & 0xFF;
    crc = cPvrCRC32::crc32((const char*)(pmt_buffer + 5), crc_offset - 5, 0xFFFFFFFF);
    pmt_buffer[crc_offset] = crc >> 24;
    pmt_buffer[crc_offset + 1] = crc >> 16;
    pmt_buffer[crc_offset + 2] = crc >> 8;
    pmt_buffer[crc_offset + 3] = crc;
    }
  retry:
  while (Running() && parent->readThreadRunning) {
    selTimeout.tv_sec = 0;
    selTimeout.tv_usec = 200000;
    FD_ZERO(&selSet);
    FD_SET(parent->v4l2_fd, &selSet);
    r = select(parent->v4l2_fd + 1, &selSet, 0, 0, &selTimeout);
    if ((r == 0) && (errno == 0)) {
       log(pvrDEBUG1, "cPvrReadThread::Action():timeout on select from /dev/video%d: %d:%s %s",
           parent->number, errno, strerror(errno), (retries > 0) ? " - retrying" : "");
       }
    else if ((r < 0) || (errno != 0)) {
       log(pvrERROR, "cPvrReadThread::Action():error on select from /dev/video%d: %d:%s %s",
           parent->number, errno, strerror(errno), (retries > 0) ? " - retrying" : "");
       retries--;
       if (retries > 0) {
          usleep(100);
          goto retry;
          }
       while (reopen_retries > 0) {
          reopen_retries--;
          if (parent->ReOpen() > 0) {
             retries = 3;
             goto retry;
             }
          }
       break;
       }
    else if (FD_ISSET(parent->v4l2_fd, &selSet)) {
       r = read(parent->v4l2_fd, buffer, bufferSize);
       if (r < 0) {
         log(pvrERROR, "cPvrReadThread::Action():error reading from /dev/video%d: %d:%s %s",
             parent->number, errno, strerror(errno), (retries > 0) ? " - retrying" : "");
         retries--;
         if (retries > 0) {
            usleep(100);
            goto retry;
            }
         break;
         }
       if (r > 0) {
         if (parent->streamType == V4L2_MPEG_STREAM_TYPE_MPEG2_TS)
           PutData(buffer, r);
         else
           ParseProgramStream(buffer, r);
         }
      }
    }
  delete [] buffer;
  log(errno ? pvrERROR : pvrDEBUG2, "cPvrReadThread::Action() %s on /dev/video%d ",
      errno ? "failed" : "stopped", parent->number);
}
