#include "udev.h"
#include <linux/stddef.h>

// --- cUdevListEntry --------------------------------------------------------

cUdevListEntry::cUdevListEntry(struct udev_list_entry *ListEntry)
:listEntry(ListEntry)
{
}

cUdevListEntry::~cUdevListEntry(void)
{
}

cUdevListEntry *cUdevListEntry::GetNext(void) const
{
  if (listEntry == NULL)
     return NULL;
  struct udev_list_entry *next = udev_list_entry_get_next(listEntry);
  if (next == NULL)
     return NULL;
  return new cUdevListEntry(next);
}

const char *cUdevListEntry::GetName(void) const
{
 if (listEntry == NULL)
     return NULL;
  return udev_list_entry_get_name(listEntry);
}

const char *cUdevListEntry::GetValue(void) const
{
 if (listEntry == NULL)
     return NULL;
  return udev_list_entry_get_value(listEntry);
}

// --- cUdevDevice -----------------------------------------------------------

cUdevDevice::cUdevDevice(udev_device *Device, bool DoUnref)
:device(Device)
,doUnref(DoUnref)
{
}

cUdevDevice::~cUdevDevice(void)
{
  if (doUnref && device)
     udev_device_unref(device);
}

int cUdevDevice::Compare(const cListObject &ListObject) const
{
  const char *n1 = GetDevnode();
  const char *n2 = ((cUdevDevice*)&ListObject)->GetDevnode();
  if ((n1 != NULL) && (n2 != NULL))
     return strcmp(n1, n2);
  return 0;
}

const char  *cUdevDevice::GetAction(void) const
{
  if (device == NULL)
     return NULL;
  return udev_device_get_action(device);
}

cUdevListEntry *cUdevDevice::GetDevlinksList(void) const
{
  if (device == NULL)
     return NULL;
  struct udev_list_entry *listEntry = udev_device_get_devlinks_list_entry(device);
  if (listEntry == NULL)
     return NULL;
  return new cUdevListEntry(listEntry);
}

const char  *cUdevDevice::GetDevnode(void) const
{
  if (device == NULL)
     return false;
  return udev_device_get_devnode(device);
}

const char  *cUdevDevice::GetDevpath(void) const
{
  if (device == NULL)
     return false;
  return udev_device_get_devpath(device);
}

cUdevDevice *cUdevDevice::GetParent(void) const
{
  if (device == NULL)
     return NULL;
  struct udev_device *parent = udev_device_get_parent(device);
  if (parent == NULL)
     return NULL;
  return new cUdevDevice(parent, false);
}

const char *cUdevDevice::GetPropertyValue(const char *Key) const
{
  if (device == NULL)
     return false;
  return udev_device_get_property_value(device, Key);
}

const char *cUdevDevice::GetSubsystem(void) const
{
  if (device == NULL)
     return false;
  return udev_device_get_subsystem(device);
}

const char *cUdevDevice::GetSysname(void) const
{
  if (device == NULL)
     return false;
  return udev_device_get_sysname(device);
}

const char *cUdevDevice::GetSyspath(void) const
{
  if (device == NULL)
     return false;
  return udev_device_get_syspath(device);
}

// --- cUdev -----------------------------------------------------------------

struct udev  *cUdev::udev = NULL;

struct udev *cUdev::Init(void)
{
  if (udev == NULL)
     udev = udev_new();
  return udev;
}

void cUdev::Free(void)
{
  if (udev)
     udev_unref(udev);
  udev = NULL;
}

cUdevDevice *cUdev::GetDeviceFromDevName(const char *DevName)
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

cUdevDevice *cUdev::GetDeviceFromSysPath(const char *SysPath)
{
  if (SysPath == NULL)
     return NULL;
  udev_device *dev = udev_device_new_from_syspath(udev, SysPath);
  if (dev == NULL)
     return NULL;
  return new cUdevDevice(dev);
}

cList<cUdevDevice> *cUdev::EnumDevices(const char *Subsystem, const char *Property, const char *Value)
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
        esyslog("pvrinput: can't add subsystem %s to enum-filter: %d", Subsystem, rc);
        goto unref;
        }
     if (Property && Value && ((rc = udev_enumerate_add_match_property(e, Property, Value)) < 0)) {
        esyslog("pvrinput: can't add property %s value %s to enum-filter: %d", Property, Value, rc);
        goto unref;
        }
     if ((rc = udev_enumerate_scan_devices(e)) < 0) {
        esyslog("pvrinput: can't scan for devices: %d", rc);
        goto unref;
        }
     l = udev_enumerate_get_list_entry(e);
     if (l == NULL) {
        isyslog("pvrinput: no devices found for %s/%s=%s", Subsystem, Property, Value);
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
