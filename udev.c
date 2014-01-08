#include "udev.h"
#include <linux/stddef.h>

// --- cUdevListEntry --------------------------------------------------------

pvrinput::cUdevListEntry::cUdevListEntry(struct udev_list_entry *ListEntry)
:listEntry(ListEntry)
{
}

pvrinput::cUdevListEntry::~cUdevListEntry(void)
{
}

pvrinput::cUdevListEntry *pvrinput::cUdevListEntry::GetNext(void) const
{
  if (listEntry == NULL)
     return NULL;
  struct udev_list_entry *next = udev_list_entry_get_next(listEntry);
  if (next == NULL)
     return NULL;
  return new cUdevListEntry(next);
}

const char *pvrinput::cUdevListEntry::GetName(void) const
{
 if (listEntry == NULL)
     return NULL;
  return udev_list_entry_get_name(listEntry);
}

const char *pvrinput::cUdevListEntry::GetValue(void) const
{
 if (listEntry == NULL)
     return NULL;
  return udev_list_entry_get_value(listEntry);
}

// --- cUdevDevice -----------------------------------------------------------

pvrinput::cUdevDevice::cUdevDevice(udev_device *Device, bool DoUnref)
:device(Device)
,doUnref(DoUnref)
{
}

pvrinput::cUdevDevice::~cUdevDevice(void)
{
  if (doUnref && device)
     udev_device_unref(device);
}

int pvrinput::cUdevDevice::Compare(const cListObject &ListObject) const
{
  const char *n1 = GetDevnode();
  const char *n2 = ((cUdevDevice*)&ListObject)->GetDevnode();
  if ((n1 != NULL) && (n2 != NULL))
     return strcmp(n1, n2);
  return 0;
}

const char  *pvrinput::cUdevDevice::GetAction(void) const
{
  if (device == NULL)
     return NULL;
  return udev_device_get_action(device);
}

pvrinput::cUdevListEntry *pvrinput::cUdevDevice::GetDevlinksList(void) const
{
  if (device == NULL)
     return NULL;
  struct udev_list_entry *listEntry = udev_device_get_devlinks_list_entry(device);
  if (listEntry == NULL)
     return NULL;
  return new cUdevListEntry(listEntry);
}

const char  *pvrinput::cUdevDevice::GetDevnode(void) const
{
  if (device == NULL)
     return false;
  return udev_device_get_devnode(device);
}

const char  *pvrinput::cUdevDevice::GetDevpath(void) const
{
  if (device == NULL)
     return false;
  return udev_device_get_devpath(device);
}

pvrinput::cUdevDevice *pvrinput::cUdevDevice::GetParent(void) const
{
  if (device == NULL)
     return NULL;
  struct udev_device *parent = udev_device_get_parent(device);
  if (parent == NULL)
     return NULL;
  return new cUdevDevice(parent, false);
}

const char *pvrinput::cUdevDevice::GetPropertyValue(const char *Key) const
{
  if (device == NULL)
     return false;
  return udev_device_get_property_value(device, Key);
}

const char *pvrinput::cUdevDevice::GetSubsystem(void) const
{
  if (device == NULL)
     return false;
  return udev_device_get_subsystem(device);
}

const char *pvrinput::cUdevDevice::GetSysname(void) const
{
  if (device == NULL)
     return false;
  return udev_device_get_sysname(device);
}

const char *pvrinput::cUdevDevice::GetSyspath(void) const
{
  if (device == NULL)
     return false;
  return udev_device_get_syspath(device);
}

// --- cUdev -----------------------------------------------------------------

cMutex pvrinput::cUdev::udev_mutex;
int    pvrinput::cUdev::udev_refcount = 0;
struct udev  *pvrinput::cUdev::udev = NULL;

struct udev *pvrinput::cUdev::Init(void)
{
  udev_mutex.Lock();
  if (udev == NULL)
     udev = udev_new();
  else
     udev_ref(udev);
  udev_refcount++;
  udev_mutex.Unlock();
  return udev;
}

void pvrinput::cUdev::Free(void)
{
  udev_mutex.Lock();
  if (udev_refcount <= 0)
     esyslog("udev: don't call cUdev::Free before cUdev::Init!");
  else {
     udev_refcount--;
     udev_unref(udev);
     if (udev_refcount <= 0)
        udev = NULL;
     }
  udev_mutex.Unlock();
}

pvrinput::cUdevDevice *pvrinput::cUdev::GetDeviceFromDevName(const char *DevName)
{
  if (DevName == NULL)
     return NULL;
  struct stat statbuf;
  if (stat(DevName, &statbuf) < 0)
     return NULL;
  char type;
  if (S_ISBLK(statbuf.st_mode))
     type = 'b';
  else if (S_ISCHR(statbuf.st_mode))
     type = 'c';
  else
     return NULL;
  udev_device *dev = udev_device_new_from_devnum(udev, type, statbuf.st_rdev);
  if (dev == NULL)
     return NULL;
  return new cUdevDevice(dev);
}

pvrinput::cUdevDevice *pvrinput::cUdev::GetDeviceFromSysPath(const char *SysPath)
{
  if (SysPath == NULL)
     return NULL;
  udev_device *dev = udev_device_new_from_syspath(udev, SysPath);
  if (dev == NULL)
     return NULL;
  return new cUdevDevice(dev);
}

cList<pvrinput::cUdevDevice> *pvrinput::cUdev::EnumDevices(const char *Subsystem, const char *Property, const char *Value)
{
  cList<cUdevDevice> *devices = new cList<cUdevDevice>;
  struct udev_enumerate *e = udev_enumerate_new(udev);
  struct udev_list_entry *l;
  cUdevListEntry *listEntry;
  const char *path;
  cUdevDevice *dev;
  if (e != NULL) {
     int rc = 0;
     if (Subsystem && ((rc = udev_enumerate_add_match_subsystem(e, Subsystem)) < 0)) {
        esyslog("udev: can't add subsystem %s to enum-filter: %d", Subsystem, rc);
        goto unref;
        }
     if (Property && Value && ((rc = udev_enumerate_add_match_property(e, Property, Value)) < 0)) {
        esyslog("udev: can't add property %s value %s to enum-filter: %d", Property, Value, rc);
        goto unref;
        }
     if ((rc = udev_enumerate_scan_devices(e)) < 0) {
        esyslog("udev: can't scan for devices: %d", rc);
        goto unref;
        }
     l = udev_enumerate_get_list_entry(e);
     if (l == NULL) {
        isyslog("udev: no devices found for %s/%s=%s", Subsystem, Property, Value);
        goto unref;
        }
     listEntry = new cUdevListEntry(l);
     while (listEntry) {
        path = listEntry->GetName();
        if (path != NULL) {
           dev = GetDeviceFromSysPath(path);
           if (dev != NULL)
              devices->Add(dev);
           }
        cUdevListEntry *tmp = listEntry->GetNext();
        delete listEntry;
        listEntry = tmp;
        }
unref:
     udev_enumerate_unref(e);
     }
  return devices;
}
