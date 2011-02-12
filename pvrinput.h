#ifndef _PVRINPUT_PLUGIN_H_
#define _PVRINPUT_PLUGIN_H_

class cPluginPvrInput : public cPlugin {
private:
public:
  cPluginPvrInput(void);
  virtual ~cPluginPvrInput();
  virtual const char *Version(void);
  virtual const char *Description(void);
  virtual const char *CommandLineHelp(void);
  virtual bool ProcessArgs(int argc, char *argv[]);
  virtual bool Initialize(void);
  virtual bool Start(void);
  virtual void Stop(void);
  virtual void Housekeeping(void);
  virtual const char *MainMenuEntry(void);
  virtual cOsdObject *MainMenuAction(void);
  virtual cMenuSetupPage *SetupMenu(void);
  virtual bool SetupParse(const char *Name, const char *Value);
};

extern cPluginPvrInput *PluginPvrInput;

#endif
