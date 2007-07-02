#include <pci/Pci.h>
#include "NicFactory.h"
#include "NE2000.h"

// based on MonesLoader by Yamami

using namespace mones;

Nic* NicFactory::create()
{
    PciInf *pciinfo;
    Pci* pcilib = new Pci();
    Nic* nic;

    //TODO 一時しのぎなので、マジックナンバーを用いる
    //QEMU上の、NE2000を探す
    //pciinfo = pcilib->CheckPciExist(0x10EC,0x8029);
    pcilib->CheckPciExist(0x10EC,0x8029, pciinfo);

    if (pciinfo->Exist == 0)
    {
        //見つかれば
        //NE2000のロード
        nic = new NE2000();

        //QEMU設定
        nic->setIRQ(9);
        nic->setIOBase(0xC100);
    }
    else{
        //見つからなければ
        //NE2000のロード
        nic = new NE2000();

        //Bochs設定
        nic->setIRQ(3);
        nic->setIOBase(0x240);
    }

    //NIC初期化
    if (nic->init()) {
	return NULL;
    }

    return nic;
}