#include "common.h"

cPvrSectionFilter::cPvrSectionFilter(u_short Pid, u_char Tid, u_char Mask)
{
  filterData.pid = Pid;
  filterData.tid = Tid;
  filterData.mask = Mask;

  handle[0] = handle[1] = -1;
  if (socketpair(AF_UNIX, SOCK_DGRAM, 0, handle) != 0) {
     log(pvrERROR, "cPvrSectionFilter(): can't open socketpair");
     Close();
     }
  else if ((fcntl(handle[0], F_SETFL, O_NONBLOCK) != 0) || (fcntl(handle[1], F_SETFL, O_NONBLOCK) != 0)) {
     log(pvrERROR, "cPvrSectionFilter(): can't switch socketpair to unblocked mode");
     Close();
     }
}

cPvrSectionFilter::~cPvrSectionFilter(void)
{
  Close();
}

void cPvrSectionFilter::Close(void)
{
  int tmp = handle[1];
  handle[1] = -1;
  if (tmp >= 0)
     close(tmp);
  tmp = handle[0];
  handle[0] = -1;
  if (tmp >= 0)
     close(tmp);
}

int  cPvrSectionFilter::GetHandle() const
{
  return handle[0];
}

cPvrSectionHandler::cPvrSectionHandler()
{
}

cPvrSectionHandler::~cPvrSectionHandler()
{
  filters.Clear();
}

int   cPvrSectionHandler::AddFilter(u_short Pid, u_char Tid, u_char Mask)
{
  cPvrSectionFilter *filter = new cPvrSectionFilter(Pid, Tid, Mask);
  int handle = filter->GetHandle();
  if (handle < 0)
     delete filter;
  else
     filters.Add(filter);
  return handle;
}

void  cPvrSectionHandler::RemoveFilter(int Handle)
{
  cPvrSectionFilter *filter = filters.First();
  while (filter) {
        if (filter->GetHandle() == Handle) {
           filters.Del(filter, true);
           break;
           }
        filter = filters.Next(filter);
        }
}

void  cPvrSectionHandler::ProcessTSPacket(const u_char *Data)
{
  if ((filters.Count() == 0) || (Data == 0))
     return;
  uint8_t section_len = ((Data[7] & 0x0F) << 8) + Data[7];
  if (section_len == 0)
     return;
  if ((Data[1] & 0x40) == 0)
     return;
  u_short pid = ((Data[1] & 0x1F) << 8) + Data[2];
  u_char  tid = Data[5];
  size_t  written = 0;
  uint8_t section_start = Data[4] + 5;
  section_len += 3;
  cPvrSectionFilter  *filter = filters.First();
  while (filter) {
        if (filter->filterData.Matches(pid, tid)) {
           written = write(filter->handle[1], Data + section_start, section_len);
           if (written != section_len)
              log(pvrERROR, "cPvrSectionHandler::ProcessTSPacket(): written only %d instead of %d", written, section_len);
           }
        filter = filters.Next(filter);
        }
}
