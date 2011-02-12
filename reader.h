#ifndef _PVRINPUT_READER_H_
#define _PVRINPUT_READER_H_

class cPvrReadThread : public cThread {
private:
  cPvrDevice *parent;
  cRingBufferLinear *tsBuffer;
  uint8_t  pat_buffer[TS_SIZE];
  uint8_t  pmt_buffer[TS_SIZE];
  uint8_t  ts_buffer[TS_SIZE];
  uint8_t  video_counter;
  uint8_t  audio_counter;
  uint8_t  text_counter;
  uint8_t  pcr_counter;
  int      packet_counter;
  uint8_t  pes_buffer[64 * 1024];
  uint8_t  pes_stream_id;
  uint32_t pes_offset;
  uint32_t pes_length;
  uint32_t pes_tmp;
  bool     pes_scr_isvalid;
  uint64_t pes_scr;
  uint32_t pes_scr_ext;

  void ParseProgramStream(uint8_t *Data, uint32_t Length);
  void PesToTs(uint8_t *Data, uint32_t Length);
  int  PutData(const unsigned char *Data, int Count);
protected:
  virtual void Action(void);
public:
  cPvrReadThread(cRingBufferLinear *TsBuffer, cPvrDevice *_parent);
  virtual ~cPvrReadThread(void);
};

#endif
