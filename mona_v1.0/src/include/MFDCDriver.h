/*!
    \file MFDCDriver.h
    \brief Floppy Disk Controller driver for Multi Task

    Copyright (c) 2003 HigePon
    WITHOUT ANY WARRANTY

    \author HigePon
    \version $Revision$
    \date create:2003/02/07 update:$Date$
*/

#include<types.h>
#include<VirtualConsole.h>

#ifndef _MONA_MFDCDRIVER_
#define _MONA_MFDCDRIVER_

/*!
    Floppy Disk Controller class
*/
class MFDCDriver {
  public:
    MFDCDriver(VirtualConsole* console);
    ~MFDCDriver();

  public:
    void printStatus(const char*) const;
    void interrupt();
    bool read(byte track, byte head, byte sector);
    bool write(byte track, byte head, byte sector);
    bool writeID(byte track, byte head, byte data);
  private:
    void initilize();
    void setFDCVersion();
    void motor(const bool on);
    bool sendCommand(const byte command[], const byte length);
    bool waitInterrupt();
    bool recalibrate();
    bool checkMSR(byte expectedCondition, byte mask);
    bool checkMSR(byte expectedCondition);
    bool seek(byte track);
    bool senseInterrupt();
    void readResults();
    void startDMA();
    void stopDMA();
    void setupDMARead(dword size);
    void setupDMAWrite(dword size);
    void printStatus(const byte msr, const char*) const;
    void printDMACStatus(const byte status, const char*) const;

  private:
    byte version_;
    static byte results_[10];
    static int resultsLength_;
    static bool interrupt_;
    static VirtualConsole* console_;
    static byte* dmabuff_;
};

extern MFDCDriver* gMFDCDriver;

#endif
