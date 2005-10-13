// DO NOT EDIT THIS FILE - it is machine generated -*- c++ -*-

#ifndef __org_monaos_Message__
#define __org_monaos_Message__

#pragma interface

#include <java/lang/Object.h>

extern "Java"
{
  namespace org
  {
    namespace monaos
    {
      class Message;
      class MessageInfo;
    }
  }
}

class org::monaos::Message : public ::java::lang::Object
{
public:
  static jint getServerThreadId (jint);
  static jint send (jint, ::org::monaos::MessageInfo *);
  static jint send (jint, jint, jint, jint, jint);
  static jint receive (::org::monaos::MessageInfo *);
  static jint reply (::org::monaos::MessageInfo *, jint, jint);
  static jint peek (::org::monaos::MessageInfo *, jint);
  static jboolean exist ();
  static jint sendReceive (::org::monaos::MessageInfo *, jint, jint, jint, jint, jint);
  Message ();
  static const jint ID_MOUSE_SERVER = 0L;
  static const jint ID_KEYBOARD_SERVER = 1L;
  static const jint ID_FILE_SERVER = 2L;
  static const jint ID_GUI_SERVER = 3L;
  static const jint ID_ELF_SERVER = 4L;
  static const jint ID_PROCESS_SERVER = 5L;
  static const jint ID_PE_SERVER = 6L;
  static const jint ID_MONITOR_SERVER = 7L;
  static const jint ID_NUMBER_OF_SERVERS = 8L;

  static ::java::lang::Class class$;
};

#endif /* __org_monaos_Message__ */