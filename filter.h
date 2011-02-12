#ifndef _PVRINPUT_FILTER_H_
#define _PVRINPUT_FILTER_H_

class cPvrSectionFilter : public cListObject {
  friend class cPvrSectionHandler;
private:
  cFilterData      filterData;
  int              handle[2]; // first handle will be returned by OpenFilter

public:
  cPvrSectionFilter(u_short Pid, u_char Tid, u_char Mask);
  ~cPvrSectionFilter(void);

  void Close(void);
  int  GetHandle() const;
};

class cPvrSectionHandler {
private:
  cList<cPvrSectionFilter>  filters;

public:
  cPvrSectionHandler();
  ~cPvrSectionHandler();

  int   AddFilter(u_short Pid, u_char Tid, u_char Mask);
  void  RemoveFilter(int Handle);
  void  ProcessTSPacket(const u_char *Data);
};

#endif
